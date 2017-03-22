#define SYS_GLOBALS
#include "sys.h"

Data_dev Set_Volt;
Data_dev Set_Curr;
Data_dev Set_Tdly;//延时
Data_dev Set_Tinvl;//间隔
Data_dev Tem_Data;
u16 ShadowTdly = 0;//当启动后，Set_Tdly，Set_Tinvl自减，自减完后要恢复，此变量做保存用
u16 ShadowTinvl = 0;
uDef_state SYS_Flags={0};
u16 Dis_Volt = 123;
u16 Dis_Curr = 4567;
u16 Set_Vmax =29500;
u16 Set_Vmin =1000;
u16 Set_Imax =29500;
u16 Set_Imin =1000;
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
/*******************************************************
*函数名 ：Sys_GPIOInit                                 *
*形参   ：void                                         *
*返回值 ：none                                         *
*描述   ：初始化系统用到的IO引脚                      *
************************************** ******************/
void Sys_GPIOInit(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_SPI1, ENABLE );	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 , ENABLE);
	/***********************SPI1*******************************/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//SPI_CLK|SPI_MIOS|SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	/***********************SPI2*******************************/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉
	/***********************can********************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
  GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	/************************OLED*******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_5|GPIO_Pin_4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_0| GPIO_Pin_1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	/************************键盘/LED IO*******************************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_8;				//端口配置
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		//推完输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOD,&GPIO_InitStructure);				//初始化GPIOD
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	/************************控制信号*******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}
/*******************************************************************************
* Function Name  : Data_Init
* Description    : 初始化各个设定值 并给Tem_Data赋值
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Data_Init(void)
{
	
	Set_Volt.Xpos=204;//Set_Volt第一位的x坐标
	Set_Volt.Ypos=7;//Set_Volt第一位的y坐标
	Set_Volt.number=1234;//Set_Volt的值
	
	Set_Curr.Xpos=204;
	Set_Curr.Ypos=20;
	Set_Curr.number=1234;
	
	Set_Tdly.Xpos=180;
	Set_Tdly.Ypos=60;
	Set_Tdly.number=10;
	
	Set_Tinvl.Xpos=220;
	Set_Tinvl.Ypos=60;
	Set_Tinvl.number=2;
	StructCopy(Tem_Data,Set_Volt);//把Set_Volt赋给Tem_Data，以便管理。
}
