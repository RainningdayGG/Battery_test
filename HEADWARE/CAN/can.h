#ifndef __CAN_H
#define __CAN_H	 
#include "includes.h"	 
#include "stm32f10x_can.h"
#ifdef	CAN_GLOBALS
#define	CAN_EXT
#else
#define	CAN_EXT extern
#endif

CAN_EXT u16 CAN_ID;//can滤波器ID 低四位携带信息了.第五位是地址信息
CAN_EXT u8 CAN_RX[8];//CAN接受缓存
CAN_EXT u8 CAN_TX[8];//CAN发送缓存
CAN_EXT u8 Comm_Count;//通信计数器
//////////////////////////////////////////////////////////////////////////////////
#define CH1_ADDR_NORM 0xE00
//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 Can_Send_Msg(u8* msg,u8 len,u32 add);						//发送数据
u8 Can_Receive_Msg(u8 *buf,u8 fifo);							//接收数据
void Comm_Can(void);
#endif

