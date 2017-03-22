#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x_spi.h"

#define LED_RCK 				PBout(1)						//595�������
#define KEY_LD					PDout(9)						//161װ�ؿ���
#define RCK_LED_KEY			PDout(8)						//ǰ���key 				  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void SPI2_Init(void);
u16 SPI2_ReadWriteByte(u16 TxData);
#endif

