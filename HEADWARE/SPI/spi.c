#include "spi.h"

void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工 SPI_Direction_2Lines_FullDuplex
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//选择了串行时钟的稳态:时钟悬空高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//数据捕获于第二个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//定义波特率预分频的值:波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输		 
}   
  
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			 	
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	while((SPI1->SR&0X80)==SET);//等待这个字节发送完成
	return 1;
}

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;      //串行同步时钟的空闲状态为低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//定义波特率预分频的值:36M/2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位（高位）还是LSB位（低位）开始:数据传输从MSB高（高位）开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);         //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI2, ENABLE); //使能SPI外设
}
/*******************************************************************************
* Function Name  : SPI2_ReadWriteByte
* Description    : SPI 写入一个字节，返回读取的一个字节.
* Input          : TxData: 要写入的字节 .
* Output         : 无
* Return         : 读取到的一个字节
*******************************************************************************/
u16 SPI2_ReadWriteByte(u16 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
		{					//检查指定的SPI标志位设置与否:发送缓存空标志位		
			retry++;
			if(retry>200)
				return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 
		{				   //检查指定的SPI标志位设置与否:接受缓存非空标志位
			retry++;
			if(retry>200)
				return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}
/*******************************************************************************
* Function Name  : Faceplate
* Description    : 前面板LED和按键值刷新.	耗时 us
* Input          : *data: 指定数据 .u8 *2
* Output         : 无
* Return         : 无
*******************************************************************************/
//u8 Led_Key(BIT16_DEF disp_panel,BIT32_DEF *dis_data,BIT32_DEF *key_data)
//{	  
//  KEY_LD  = 1; //过非门后 输出 0  装载166
//	LED_RCK = 0; //输出0   
//  SPI2_ReadWriteByte(disp_panel.halfword);  //发送数据到595	
//	LED_RCK = 1;//输出1 
//	KEY_LD = 0; //过非门后 输出 1    装载166完毕
//	RCK_LED_KEY = 1;//过非门0 控制键盘灯
//	key_data->half.high = ~SPI2_ReadWriteByte(dis_data->half.high); //发送数据到595读取166数据
//	key_data->half.low = ~SPI2_ReadWriteByte(dis_data->half.low); //发送数据到595读取166数据	
//	RCK_LED_KEY = 0;//过非门1 控制键盘灯
//  return 1;  //返回SPI数据
//}



































