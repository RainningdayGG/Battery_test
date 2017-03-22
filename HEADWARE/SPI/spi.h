#ifndef __SPI_H
#define __SPI_H
#include "stm32f10x_spi.h"

#define LED_RCK 				PBout(1)						//595输出控制
#define KEY_LD					PDout(9)						//161装载控制
#define RCK_LED_KEY			PDout(8)						//前面板key 				  	    													  
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void SPI2_Init(void);
u16 SPI2_ReadWriteByte(u16 TxData);
#endif

