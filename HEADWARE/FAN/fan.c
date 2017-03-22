#include "fan.h"

u8  Fan_pwm    = 0;//������ת��0
u16 Fan_speed = 0;//�����ת��
u16 speed;
/*******************************************************************************
* Function Name  : TIM5_Init
* Description    : ��ʱ��5��ʱ�жϳ�ʼ��
* Input          : arr,psc
* Output         : none
* Return         : none
*******************************************************************************/
void TIM5_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//ʱ��ʹ��
	//��ʱ��TIM5��ʼ��
	TIM_TimeBaseStructure.TIM_Period=arr;//��������һ������ʱ��װ�����Զ���װ�ؼĴ������ڵ�ֵ�
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//����������Ϊʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//�������໷ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//��ʱ�����ϼ���
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	//�ж����ȼ�NVIC����
	//NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM5,ENABLE);
}
/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : ��ʱ��5�жϷ�����򣬲���PWM���Ʒ���
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u16 static  Fan_delay;
void TIM5_IRQHandler(void)
{
	static u16 count=0,numb=0;
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)//���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//�����ʱ�жϱ�־
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
						Fan_speed = speed;//�õ�ת��
						speed = 0;
				}
		}
	}
}
/*******************************************************************************
* Function Name  : EXTIX_Init
* Description    : �ⲿ�жϳ�ʼ���ж���PC8 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void EXTIX_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;  //Pc8 ���֮ǰ����  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //Pc8 ���� shang la
		GPIO_Init(GPIOC, &GPIO_InitStructure);

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
		//GPIOC.8 �ж����Լ��жϳ�ʼ������  �����ش���	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource8);
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 	
  	EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ��PC8���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ�3�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
 
}

void delay(vu32 n)
{  u16 i=1000;
	for(; n != 0; n--)
	{
		while(i--);
	}
}

//�ⲿ�ж�0������� 
void EXTI9_5_IRQHandler(void)
{
   if(EXTI_GetITStatus(EXTI_Line8) != RESET)//������B
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
		EXTI_ClearITPendingBit(EXTI_Line8); //���LINE8�ϵ��жϱ�־λ  
	}	
}

/*******************************************************************************
* Function Name  : Fan_init
* Description    : ���ȿ��Ƴ�ʼ����������ʱ�����жϡ�IO
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Fan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM5_Init(180,9);//��ʼ����ʱ��25us pwmƵ��40��
	EXTIX_Init();//�����жϳ�ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;				//PD11.12�˿�����
	GPIO_Init(GPIOD,&GPIO_InitStructure);				//��ʼ��GPIOD.13
	FAN_POWER_CON =		0;	
	FAN_PWM_CON   =   1;
}
/*******************************************************************************
* Function Name  : Fan_Control
* Description    : �����ٶȿ��ƺ�ת�ټ��//20ms����һ��
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u8 Fan_Control(void)
{	
	float speed;
		static u16 Fun_err = 0;//���ȴ������
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



















