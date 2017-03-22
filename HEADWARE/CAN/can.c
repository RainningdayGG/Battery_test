#define CAN_GLOBALS
#include "can.h"
u16 CAN_ID;//can滤波器ID 低四位携带信息了.第五位是地址信息
u8 can_buf[8];//can 接受缓存
u8 Comm_Count;//通信计数器
/*******************************************************************************
* Function Name  : CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
* Description    : CAN初始化
* Input          : tsjw:重新同步跳跃时间单元.范围:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
											tbs2:时间段2的时间单元.范围:1~8;
											tbs1:时间段1的时间单元.范围:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
											brp :波特率分频器.范围:1~1024;(实际要加1,也就是1~1024) tq=(brp)*tpclk1
											注意以上参数任何一个都不能设为0,否则会乱.
											波特率=Fpclk1/((tsjw+tbs1+tbs2)*brp);//Fpclk1的时钟在初始化的时候设置为36M,如果设置CAN_Normal_Init(1,8,7,5,1);
											mode:0,普通模式;1,回环模式;//则波特率为:36M/((1+8+7)*5)=450Kbps
* Output         : none
* Return         : 返回值:0,初始化OK;其他,初始化失败;
*******************************************************************************/
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	 CAN_InitTypeDef        CAN_InitStructure;
 	 CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
            											
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
 	//CAN单元设置
 	  CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	  CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 //禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= mode;	             //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // 初始化CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
 	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//标识符屏蔽位模
// 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;//标识符列表模式  	
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x000<<5; //32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xE000;//32位MASK 	  
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;	//         		   
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
 	  CAN_FilterInitStructure.CAN_FilterNumber=1;	  //过滤器1
 	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//标识符屏蔽位模
// 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;//标识符列表模式  	
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x100<<5; //32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xE000;//32位MASK 	  
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;	//         		   
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器1
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化		
#if CAN_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
} 
/*******************************************************************************
* Function Name  : Can_Send_Msg(u8* msg,u8 len,u32 add);
* Description    : can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
* Input          : len:数据长度(最大为8)			msg:数据指针,最大为8个字节.
* Output         : none
* Return         : 返回值:0,成功;其他,失败;
*******************************************************************************/ 
u8 Can_Send_Msg(u8* msg,u8 len,u32 add)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=add;		// 标准标识符为0  /*This parameter can be a value between 0 to 0x7FF. */
  TxMessage.IDE=0;			 		 // 使用标准标识符
  TxMessage.RTR=0;		 			 // 消息类型为数据帧，一帧8位 
  TxMessage.DLC=len;				 // 帧长度
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				           
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;		

}
/*******************************************************************************
* Function Name  : Can_Receive_Msg(u8 *buf)  
* Description    : can口接收数据查询
* Input          : buf:数据缓存区;	 
* Output         : none
* Return         : 返回值:0,无数据被收到;其他,接收的数据长度;
*******************************************************************************/
u8 Can_Receive_Msg(u8 *buf,u8 fifo)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,fifo)==0)return 0;		//没有接收到数据,直接退出 
	CAN_Receive(CAN1, fifo, &RxMessage);//读取数据	
	for(i=0;i<8;i++)
	{
		buf[i]=RxMessage.Data[i];  
	}
	CAN_ID=RxMessage.StdId;
	return RxMessage.DLC;	
}
#if CAN_RX0_INT_ENABLE
/******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : can接受中断处理函数
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/	
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	int i=0;
	if(CAN_GetITStatus(CAN1,CAN_IT_ERR)!=RESET)//检查是否中断错误
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_ERR);
	}
	else
	{
		CAN_Receive(CAN1,0,&RxMessage);
		CAN_ID=RxMessage.StdId;
		for(i=0;i<RxMessage.DLC;i++)
		{
			CAN_RX[i]=RxMessage.Data[i];//接受模拟板发来的DAC_V  DAC_C 
		}
	}
}
#endif
/******************************************************************************
* Function Name   : Send_SetValue
* Description     : 把前面变设定的状态传给模拟板(电压，电流，过压保护值，电源的开关)
* Input           : none
* Return          : none
*******************************************************************************/
void Send_SetValue(void)//发送设定值
{
	u16 add = Power_Addr;
	u8 send_buf[8] = {0};
	send_buf[0]=Set_Volt.number/256;
	send_buf[1]=Set_Volt.number%256;
	send_buf[2]=Set_Curr.number/256;
	send_buf[3]=Set_Curr.number%256;
	send_buf[4]=Set_Tdly.number/256;
	send_buf[5]=Set_Tdly.number%256;
	send_buf[6]=Set_Tinvl.number/256;
	send_buf[7]=Set_Tinvl.number%256;
	Can_Send_Msg(send_buf,8,add);
}
void Send_Set(void)
{
	u16 add = Power_Addr;
	u8 send_buf[1] = {0};
	send_buf[0]=SYS_Flags.uB.send;	//修改按键时需修改
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : Decipher()
* Description     : 解析CAN发来的数据
* Input           : none
* Return          : none
*******************************************************************************/
void Recevie_Candata(void)
{
	u16 addr;
	addr = CAN_ID;
	switch(addr)
	{
		case 0x101://模拟板发来的电压电流和状态
		{
			Dis_Volt = can_buf[0]*256+can_buf[1];
			Dis_Curr = can_buf[2]*256+can_buf[3];
			SYS_Flags.uB.receive = can_buf[6];
			Comm_Count=0;
		}break;
		case 0x103://校准完毕
		{
			if(can_buf[0] == 1)
			{
				SYS_cal = 0;
				cal_state = 0;
				SYS_output = 0;//关闭输出
				Comm_Count=0;
			}break;
		}
		default:
		{}break;
	}
}
/******************************************************************************
* Function Name   : Comm_Can
* Description     : can通信函数
* Input           : none
* Return          : none
*******************************************************************************/
void Comm_Can(void)
{
	if(Can_Receive_Msg(can_buf,CAN_FIFO0))//解析其他板子发给前面板数据
	{
			Recevie_Candata();
	}
  if(SYS_cal == 0)//非校准状态
	{
		Send_SetValue();
	}
	else//校准状态
	{
		calibration(cal_state);
	}
	Comm_Count++;
	if(Comm_Count>=140)
	{
		Comm_Count = 100;
	}
}
