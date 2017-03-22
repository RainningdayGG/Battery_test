#include "fan.h"

u8  Fan_pwm    = 0;//¿ØÖÆÁ¿×ªËÙ0
u16 Fan_speed = 0;//»á¶ÁÁ¿×ªËÙ
u16 speed;
/*******************************************************************************
* Function Name  : TIM5_Init
* Description    : ¶¨Ê±Æ÷5¶¨Ê±ÖĞ¶Ï³õÊ¼»¯
* Input          : arr,psc
* Output         : none
* Return         : none
*******************************************************************************/
void TIM5_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//Ê±ÖÓÊ¹ÄÜ
	//¶¨Ê±Æ÷TIM5³õÊ¼»¯
	TIM_TimeBaseStructure.TIM_Period=arr;//ÉèÖÃÔÚÏÂÒ»¸ö¸üĞÂÊ±¼ä×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµØ
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//ÉèÖÃÓÃÀ´×÷ÎªÊ±ÖÓÆµÂÊ³ıÊıµÄÔ¤·ÖÆµÖµ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//ÉèÖÃËøÏà»·Ê±ÖÓ·Ö¸î
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//¶¨Ê±Æ÷ÏòÉÏ¼ÆÊı
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	//ÖĞ¶ÏÓÅÏÈ¼¶NVICÉèÖÃ
	//NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5,ENABLE);
}
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : ¶¨Ê±Æ÷5ÖĞ¶Ï·şÎñ³ÌĞò£¬²úÉúPWM¿ØÖÆ·çÉÈ
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u16 static  Fan_delay;
void TIM5_IRQHandler(void)
{
	static u16 count=0,numb=0;
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)//¼ì²éTIM5¸üĞÂÖĞ¶Ï·¢ÉúÓë·ñ
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//Çå³ı¶¨Ê±ÖĞ¶Ï±êÖ¾
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
						Fan_speed = speed;//µÃµ½×ªËÙ
						speed = 0;
				}
		}
	}
}
/*******************************************************************************
* Function Name  : EXTIX_Init
* Description    : Íâ²¿ÖĞ¶Ï³õÊ¼»¯ÖĞ¶ÏÏßPC8 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void EXTIX_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //Ê¹ÄÜGPIOCÊ±ÖÓ
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //Pc8 Çå³ıÖ®Ç°ÉèÖÃ  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //Pc8 ÊäÈë shang la
		GPIO_Init(GPIOC, &GPIO_InitStructure);

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//Ê¹ÄÜ¸´ÓÃ¹¦ÄÜÊ±ÖÓ
		//GPIOC.8 ÖĞ¶ÏÏßÒÔ¼°ÖĞ¶Ï³õÊ¼»¯ÅäÖÃ  ÉÏÉıÑØ´¥·¢	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//¸ù¾İEXTI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèEXTI¼Ä´æÆ÷

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//Ê¹ÄÜPC8ËùÔÚµÄÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//ÇÀÕ¼ÓÅÏÈ¼¶3£¬ 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//×ÓÓÅÏÈ¼¶3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//Ê¹ÄÜÍâ²¿ÖĞ¶ÏÍ¨µÀ
  	NVIC_Init(&NVIC_InitStructure); 
 
}

void delay(vu32 n)
{  u16 i=1000;
	for(; n != 0; n--)
	{
		while(i--);
	}
}

//Íâ²¿ÖĞ¶Ï0·şÎñ³ÌĞò 
void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)//Á÷Á¿¼ÆB
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
		EXTI_ClearITPendingBit(EXTI_Line8); //Çå³ıLINE8ÉÏµÄÖĞ¶Ï±êÖ¾Î»  
	}	
}

/*******************************************************************************
* Function Name  : Fan_init
* Description    : ·çÉÈ¿ØÖÆ³õÊ¼»¯£¬°üº¬¶¨Ê±Æ÷£¬ÖĞ¶Ï¡£IO
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Fan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM5_Init(180,9);//³õÊ¼»¯¶¨Ê±Æ÷25us pwmÆµÂÊ40ºÕ
	EXTIX_Init();//²âËÙÖĞ¶Ï³õÊ¼»¯
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		//ÍÆÍêÊä³ö
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		//IO¿ÚËÙ¶ÈÎª50MHz

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;				//PD11.12¶Ë¿ÚÅäÖÃ
	GPIO_Init(GPIOD,&GPIO_InitStructure);				//³õÊ¼»¯GPIOD.13
	FAN_POWER_CON =		0;	
	FAN_PWM_CON   =   1;
}
/*******************************************************************************
* Function Name  : Fan_Control
* Description    : ·çÉÈËÙ¶È¿ØÖÆºÍ×ªËÙ¼ì²â//20msµ÷ÓÃÒ»´Î
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u8 Fan_Control(void)
{	
	float speed;
		static u16 Fun_err = 0;//·çÉÈ´íÎó¼ÆÊı
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



















