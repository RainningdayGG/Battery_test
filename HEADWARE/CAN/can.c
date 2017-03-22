#define CAN_GLOBALS
#include "can.h"
u16 CAN_ID;//can�˲���ID ����λЯ����Ϣ��.����λ�ǵ�ַ��Ϣ
u8 can_buf[8];//can ���ܻ���
u8 Comm_Count;//ͨ�ż�����
/*******************************************************************************
* Function Name  : CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
* Description    : CAN��ʼ��
* Input          : tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:1~3; CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
											tbs2:ʱ���2��ʱ�䵥Ԫ.��Χ:1~8;
											tbs1:ʱ���1��ʱ�䵥Ԫ.��Χ:1~16;	  CAN_BS1_1tq ~CAN_BS1_16tq
											brp :�����ʷ�Ƶ��.��Χ:1~1024;(ʵ��Ҫ��1,Ҳ����1~1024) tq=(brp)*tpclk1
											ע�����ϲ����κ�һ����������Ϊ0,�������.
											������=Fpclk1/((tsjw+tbs1+tbs2)*brp);//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Normal_Init(1,8,7,5,1);
											mode:0,��ͨģʽ;1,�ػ�ģʽ;//������Ϊ:36M/((1+8+7)*5)=450Kbps
* Output         : none
* Return         : ����ֵ:0,��ʼ��OK;����,��ʼ��ʧ��;
*******************************************************************************/
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
	 CAN_InitTypeDef        CAN_InitStructure;
 	 CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
            											
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
 	//CAN��Ԫ����
 	  CAN_InitStructure.CAN_TTCM=DISABLE;						 //��ʱ�䴥��ͨ��ģʽ  //
 	  CAN_InitStructure.CAN_ABOM=DISABLE;						 //����Զ����߹���	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 //��ֹ�����Զ����� //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //���Ĳ�����,�µĸ��Ǿɵ� // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //���ȼ��ɱ��ı�ʶ������ //
  	CAN_InitStructure.CAN_Mode= mode;	             //ģʽ���ã� mode:0,��ͨģʽ;1,�ػ�ģʽ; //
  	//���ò�����
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1��ʱ�䵥λCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1��ʱ�䵥λCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            //��Ƶϵ��(Fdiv)Ϊbrp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // ��ʼ��CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
 	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ������λģ
// 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;//��ʶ���б�ģʽ  	
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x000<<5; //32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xE000;//32λMASK 	  
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;	//         		   
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
 	  CAN_FilterInitStructure.CAN_FilterNumber=1;	  //������1
 	  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;//��ʶ������λģ
// 	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdList;//��ʶ���б�ģʽ  	
	  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;//32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x100<<5; //32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0xE000;//32λMASK 	  
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;	//         		   
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������1
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��		
#if CAN_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
} 
/*******************************************************************************
* Function Name  : Can_Send_Msg(u8* msg,u8 len,u32 add);
* Description    : can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
* Input          : len:���ݳ���(���Ϊ8)			msg:����ָ��,���Ϊ8���ֽ�.
* Output         : none
* Return         : ����ֵ:0,�ɹ�;����,ʧ��;
*******************************************************************************/ 
u8 Can_Send_Msg(u8* msg,u8 len,u32 add)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=add;		// ��׼��ʶ��Ϊ0  /*This parameter can be a value between 0 to 0x7FF. */
  TxMessage.IDE=0;			 		 // ʹ�ñ�׼��ʶ��
  TxMessage.RTR=0;		 			 // ��Ϣ����Ϊ����֡��һ֡8λ 
  TxMessage.DLC=len;				 // ֡����
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				           
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}
/*******************************************************************************
* Function Name  : Can_Receive_Msg(u8 *buf)  
* Description    : can�ڽ������ݲ�ѯ
* Input          : buf:���ݻ�����;	 
* Output         : none
* Return         : ����ֵ:0,�����ݱ��յ�;����,���յ����ݳ���;
*******************************************************************************/
u8 Can_Receive_Msg(u8 *buf,u8 fifo)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,fifo)==0)return 0;		//û�н��յ�����,ֱ���˳� 
	CAN_Receive(CAN1, fifo, &RxMessage);//��ȡ����	
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
* Description    : can�����жϴ�����
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/	
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	int i=0;
	if(CAN_GetITStatus(CAN1,CAN_IT_ERR)!=RESET)//����Ƿ��жϴ���
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_ERR);
	}
	else
	{
		CAN_Receive(CAN1,0,&RxMessage);
		CAN_ID=RxMessage.StdId;
		for(i=0;i<RxMessage.DLC;i++)
		{
			CAN_RX[i]=RxMessage.Data[i];//����ģ��巢����DAC_V  DAC_C 
		}
	}
}
#endif
/******************************************************************************
* Function Name   : Send_SetValue
* Description     : ��ǰ����趨��״̬����ģ���(��ѹ����������ѹ����ֵ����Դ�Ŀ���)
* Input           : none
* Return          : none
*******************************************************************************/
void Send_SetValue(void)//�����趨ֵ
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
	send_buf[0]=SYS_Flags.uB.send;	//�޸İ���ʱ���޸�
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : Decipher()
* Description     : ����CAN����������
* Input           : none
* Return          : none
*******************************************************************************/
void Recevie_Candata(void)
{
	u16 addr;
	addr = CAN_ID;
	switch(addr)
	{
		case 0x101://ģ��巢���ĵ�ѹ������״̬
		{
			Dis_Volt = can_buf[0]*256+can_buf[1];
			Dis_Curr = can_buf[2]*256+can_buf[3];
			SYS_Flags.uB.receive = can_buf[6];
			Comm_Count=0;
		}break;
		case 0x103://У׼���
		{
			if(can_buf[0] == 1)
			{
				SYS_cal = 0;
				cal_state = 0;
				SYS_output = 0;//�ر����
				Comm_Count=0;
			}break;
		}
		default:
		{}break;
	}
}
/******************************************************************************
* Function Name   : Comm_Can
* Description     : canͨ�ź���
* Input           : none
* Return          : none
*******************************************************************************/
void Comm_Can(void)
{
	if(Can_Receive_Msg(can_buf,CAN_FIFO0))//�����������ӷ���ǰ�������
	{
			Recevie_Candata();
	}
  if(SYS_cal == 0)//��У׼״̬
	{
		Send_SetValue();
	}
	else//У׼״̬
	{
		calibration(cal_state);
	}
	Comm_Count++;
	if(Comm_Count>=140)
	{
		Comm_Count = 100;
	}
}
