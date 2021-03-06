#ifndef __SYS_H
#define __SYS_H	

#ifdef SYS_GLOBALS
#define SYS_EXT 
#else
#define SYS_EXT extern
#endif

#include "stm32f10x.h"
//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		1		//定义系统文件夹是否支持UCOS
																	    
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入

typedef struct
{
	u8 Xpos;
	u8 Ypos;
	u16 number;
}Data_dev;

typedef union
{
	u16 Flags;
	struct
	{
		u8 receive;
		u8 send;
	}uB;
	struct
	{
		unsigned char SW : 1;//state of work (cc or cv)
		unsigned char SO : 1;//state of output(on or off)
		unsigned char SS : 1;//state of system(cal or sys)
		unsigned char Cal_Zero:1;
		unsigned char Cal_Vmin:1;
		unsigned char Cal_Vmax:1;
		unsigned char Cal_Imin:1;
		unsigned char Cal_Imax:1;
		
		unsigned char SYS_output: 1;
		unsigned char Singletimes : 1;
		unsigned char Timerunning : 1;
		unsigned char bit2 : 1;
		unsigned char bit3 : 1;
		unsigned char bit4 : 1;
		unsigned char bit5 : 1;
		unsigned char bit6 : 1;
	}Fb;		//Flag bits
}uDef_state;//unsigned Byte
SYS_EXT uDef_state SYS_Flags;
#define state_output  SYS_Flags.Fb.SO
#define state_work    SYS_Flags.Fb.SW
#define SYS_cal       SYS_Flags.Fb.SS
#define cal_Vmax      SYS_Flags.Fb.Cal_Vmax
#define cal_Vmin      SYS_Flags.Fb.Cal_Vmin
#define cal_Imax      SYS_Flags.Fb.Cal_Imax
#define cal_Imin      SYS_Flags.Fb.Cal_Imin
#define cal_Zero      SYS_Flags.Fb.Cal_Zero

#define SYS_output    SYS_Flags.Fb.SYS_output
#define SYS_single    SYS_Flags.Fb.Singletimes
#define SYS_timerunning    SYS_Flags.Fb.Timerunning
SYS_EXT Data_dev Set_Volt;
SYS_EXT Data_dev Set_Curr;
SYS_EXT Data_dev Set_Tdly;
SYS_EXT Data_dev Set_Tinvl;
SYS_EXT Data_dev Tem_Data;
SYS_EXT u16 ShadowTdly;//当启动后，Set_Tdly，Set_Tinvl自减，自减完后要恢复，此变量做保存用
SYS_EXT u16 ShadowTinvl;
SYS_EXT u16 Dis_Volt;
SYS_EXT u16 Dis_Curr;
SYS_EXT u16 Set_Vmax;
SYS_EXT u16 Set_Vmin;
SYS_EXT u16 Set_Imax;
SYS_EXT u16 Set_Imin;
#define StructCopy(StuA,StuB) {StuA.Xpos=StuB.Xpos;StuA.Ypos=StuB.Ypos;StuA.number=StuB.number;}
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址
void Sys_GPIOInit(void);
void Data_Init(void);
#endif
