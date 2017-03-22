#include "includes.h"

/////////////////////////UCOSII��������///////////////////////////////////
//START ����
#define START_TASK_PRIO      			10//��ʼ��������ȼ�����Ϊ���
#define START_STK_SIZE  				35
static OS_STK start_task_stk[START_STK_SIZE];
void start_task(void *p_arg);	
 	
#define Comm_canTaskPrio            5
#define Comm_canTasksize  		    	200
static OS_STK Comm_canTaskstk[Comm_canTasksize];
void Communication_Can_task(void *p_arg);

#define RefDat_Task_PRIO           9
#define RefDat_STK_SIZE  		    	 200
static OS_STK RefDat_Task_stk[RefDat_STK_SIZE];
void RefDat_task(void *p_arg);

#define KEY_Task_PRIO           8
#define KEY_STK_SIZE  		    	 200
static OS_STK KEY_Task_stk[KEY_STK_SIZE];
void Key_task(void *p_arg);

#define Time_Task_PRIO           6
#define Time_STK_SIZE  		    	 128
static OS_STK Time_Task_stk[Time_STK_SIZE];
void Contrl_Time_Task(void *p_arg);
//////////////////////////////////////////////////////////////////////////////

int main()
{
	 delay_init();
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	 Sys_GPIOInit();
 	 uart_init(9600);
 	 SPI1_Init();
	 SPI2_Init();
 	 OLED_Init();
	 GUI_Init();
   OSInit();
	 OSTaskCreate(start_task,(void *)0,&start_task_stk[START_STK_SIZE-1],START_TASK_PRIO);
   OSStart();
   return 0;
}
void start_task(void *p_arg)
{
	OS_CPU_SR cpu_sr=0;
	p_arg=p_arg;
	OS_ENTER_CRITICAL();//�����ٽ���(�޷����жϴ��)
	OSTaskCreate(Communication_Can_task,(void *)0,&Comm_canTaskstk[Comm_canTasksize-1],Comm_canTaskPrio);	
	OSTaskCreate(RefDat_task,(void *)0,&RefDat_Task_stk[RefDat_STK_SIZE-1],RefDat_Task_PRIO);		
  OSTaskCreate(Key_task,(void *)0,&KEY_Task_stk[KEY_STK_SIZE-1],KEY_Task_PRIO);	
	OSTaskCreate(Contrl_Time_Task,(void *)0,&Time_Task_stk[Time_STK_SIZE-1],Time_Task_PRIO);	
	OSTaskSuspend(Time_Task_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//������ʼ����.
	OS_EXIT_CRITICAL();	//�˳��ٽ���(���Ա��жϴ��)
}

void RefDat_task(void *p_arg)//����ˢ������
{
	p_arg = p_arg;
	Data_Init();
	while(1)
	{
		Refresh_Data();
		delay_ms(20);
	}
}

void Key_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		Led_Key(led_panel,&led_key,&key_buff);
		KEY_Scan(key_buff.word);
		key_exe();
		delay_ms(20);
	}
}
void Communication_Can_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{	           //                   |------У׼״̬
		Comm_Can();//�����˽��պͷ���---
		delay_ms(20);//                 |------��У׼״̬
	}
}
/*******************************************************************************
* Function Name  : Contrl_Time_Task
* Description    : Set_Tdly��Set_Tinvl���Լ���Set_Tdly���Լ�������Ϊ0ʱ��Set_Tinvl���Լ�����Set_Tinvl��Ϊ0�� �����Լ�
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Contrl_Time_Task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		OSTimeDlyHMSM(0,0,1,0);//����ʱһ�룬��ֹһ�����񣬾����ϼ�һ��
		if(SYS_single == 1)//����
		{
			if(Set_Tdly.number == 0)//�ȼ��Set_Tdly�Ƿ������ 
			{
				Set_Tdly.number = ShadowTdly;
				SYS_single = 0;//������α�־
				SYS_timerunning = 0;//���running��־
				OSTaskSuspend(OS_PRIO_SELF);
			}
			else Set_Tdly.number--;
		}
		else//����
		{
			if(Set_Tdly.number == 0)//�ȼ��Set_Tdly�Ƿ������ 
			{
				if(Set_Tinvl.number == 0)//Set_Tdly==0��Set_Tinvl==0 ������Լ�
				{
						Set_Tdly.number = ShadowTdly;
						Set_Tinvl.number = ShadowTinvl;
					  SYS_timerunning = 0;
						OSTaskSuspend(OS_PRIO_SELF);
				}
				else//Set_Tdly==0 Set_Tinvl�Լ�
				Set_Tinvl.number--;
			}//Set_Tdly!=0 Set_Tdly�Լ�
			else //Set_Tdly!=0 Set_Tdly�Լ�
			Set_Tdly.number--;
		}	
	}
}
