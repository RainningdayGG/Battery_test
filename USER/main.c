#include "includes.h"

/////////////////////////UCOSII任务设置///////////////////////////////////
//START 任务
#define START_TASK_PRIO      			10//开始任务的优先级设置为最低
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
	OS_ENTER_CRITICAL();//进入临界区(无法被中断打断)
	OSTaskCreate(Communication_Can_task,(void *)0,&Comm_canTaskstk[Comm_canTasksize-1],Comm_canTaskPrio);	
	OSTaskCreate(RefDat_task,(void *)0,&RefDat_Task_stk[RefDat_STK_SIZE-1],RefDat_Task_PRIO);		
  OSTaskCreate(Key_task,(void *)0,&KEY_Task_stk[KEY_STK_SIZE-1],KEY_Task_PRIO);	
	OSTaskCreate(Contrl_Time_Task,(void *)0,&Time_Task_stk[Time_STK_SIZE-1],Time_Task_PRIO);	
	OSTaskSuspend(Time_Task_PRIO);
	OSTaskSuspend(START_TASK_PRIO);	//挂起起始任务.
	OS_EXIT_CRITICAL();	//退出临界区(可以被中断打断)
}

void RefDat_task(void *p_arg)//数据刷新任务
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
	{	           //                   |------校准状态
		Comm_Can();//包含了接收和发送---
		delay_ms(20);//                 |------非校准状态
	}
}
/*******************************************************************************
* Function Name  : Contrl_Time_Task
* Description    : Set_Tdly和Set_Tinvl的自减。Set_Tdly先自减，当减为0时，Set_Tinvl再自减，当Set_Tinvl减为0后 挂起自己
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Contrl_Time_Task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		OSTimeDlyHMSM(0,0,1,0);//先延时一秒，防止一进任务，就马上减一秒
		if(SYS_single == 1)//单次
		{
			if(Set_Tdly.number == 0)//先检测Set_Tdly是否等于零 
			{
				Set_Tdly.number = ShadowTdly;
				SYS_single = 0;//清除单次标志
				SYS_timerunning = 0;//清除running标志
				OSTaskSuspend(OS_PRIO_SELF);
			}
			else Set_Tdly.number--;
		}
		else//连续
		{
			if(Set_Tdly.number == 0)//先检测Set_Tdly是否等于零 
			{
				if(Set_Tinvl.number == 0)//Set_Tdly==0且Set_Tinvl==0 则挂起自己
				{
						Set_Tdly.number = ShadowTdly;
						Set_Tinvl.number = ShadowTinvl;
					  SYS_timerunning = 0;
						OSTaskSuspend(OS_PRIO_SELF);
				}
				else//Set_Tdly==0 Set_Tinvl自减
				Set_Tinvl.number--;
			}//Set_Tdly!=0 Set_Tdly自减
			else //Set_Tdly!=0 Set_Tdly自减
			Set_Tdly.number--;
		}	
	}
}
