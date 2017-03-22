#ifndef __CAN_H
#define __CAN_H	 
#include "includes.h"	 
#include "stm32f10x_can.h"
#ifdef	CAN_GLOBALS
#define	CAN_EXT
#else
#define	CAN_EXT extern
#endif

CAN_EXT u16 CAN_ID;//can�˲���ID ����λЯ����Ϣ��.����λ�ǵ�ַ��Ϣ
CAN_EXT u8 CAN_RX[8];//CAN���ܻ���
CAN_EXT u8 CAN_TX[8];//CAN���ͻ���
CAN_EXT u8 Comm_Count;//ͨ�ż�����
//////////////////////////////////////////////////////////////////////////////////
#define CH1_ADDR_NORM 0xE00
//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
u8 Can_Send_Msg(u8* msg,u8 len,u32 add);						//��������
u8 Can_Receive_Msg(u8 *buf,u8 fifo);							//��������
void Comm_Can(void);
#endif

