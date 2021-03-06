#include "fan.h"

u8  Fan_pwm    = 0;//控制量转速0
u16 Fan_speed = 0;//会读量转速
u16 speed;
/*******************************************************************************
* Function Name  : TIM5_Init
* Description    : 定时器5定时中断初始化
* Input          : arr,psc
* Output         : none
* Return         : none
*******************************************************************************/
void TIM5_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//时钟使能
	//定时器TIM5初始化
	TIM_TimeBaseStructure.TIM_Period=arr;//设置在下一个更新时间装入活动的自动重装载寄存器周期的值�
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//设置用来作为时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置锁相环时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//定时器向上计数
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	//中断优先级NVIC设置
	//NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5,ENABLE);
}
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : 定时器5中断服务程序，产生PWM控制风扇
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u16 static  Fan_delay;
void TIM5_IRQHandler(void)
{
	static u16 count=0,numb=0;
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)//检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除定时中断标志
		if (count<Fan_pwm)
		{
			 FAN_PWM_CON = 0;                        
		}
		else
		{
			FAN_PWM_CON = 1 ;
		}		
		count++;
		Fan_delay++;
		if(count==100)//2.5ms
		{
				count=0;
				numb++;
				if(numb==400)//1s
				{	
						numb = 0;
						Fan_speed = speed;//得到转速
						speed = 0;
				}
		}
	}
}
/*******************************************************************************
* Function Name  : EXTIX_Init
* Description    : 外部中断初始化中断线PC8 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void EXTIX_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIOC时钟
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //Pc8 清除之前设置  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //Pc8 输入 shang la
		GPIO_Init(GPIOC, &GPIO_InitStructure);

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
		//GPIOC.8 中断线以及中断初始化配置  上升沿触发	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能PC8所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//抢占优先级3， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
 
}

void delay(vu32 n)
{  u16 i=1000;
	for(; n != 0; n--)
	{
		while(i--);
	}
}

//外部中断0服务程序 
void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)//流量计B
	{
		Fan_delay = 0;
		while(1)
		{
			if(Fan_delay==100)
			{
					break;				
			}
		}
		if(FAN_SPEED_SIG==1)
		{
				speed++;
		}
		EXTI_ClearITPendingBit(EXTI_Line8); //清除LINE8上的中断标志位  
	}	
}

/*******************************************************************************
* Function Name  : Fan_init
* Description    : 风扇控制初始化，包含定时器，中断。IO
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Fan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM5_Init(180,9);//初始化定时器25us pwm频率40赫
	EXTIX_Init();//测速中断初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		//推完输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		//IO口速度为50MHz

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;				//PD11.12端口配置
	GPIO_Init(GPIOD,&GPIO_InitStructure);				//初始化GPIOD.13
	FAN_POWER_CON =		0;	
	FAN_PWM_CON   =   1;
}
/*******************************************************************************
* Function Name  : Fan_Control
* Description    : 风扇速度控制和转速检测//20ms调用一次
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u8 Fan_Control(void)
{	
	float speed;
		static u16 Fun_err = 0;//风扇错误计数
		speed   = 1000;

		Fan_pwm = (u8)speed;
		if(Fan_pwm>100)
			Fan_pwm = 100;
		
		if(Fan_speed<20)
		{
				Fun_err++;
				if(Fun_err>200)
				{	
					Fun_err = 200;
						return 1;
				}
				return 0;				
		}
		else
		{
			Fun_err = 0;
			return 0;
		}	
}



















