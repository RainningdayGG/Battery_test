#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#ifdef	KEY_GLOBALS
#define	KEY_EXT
#else
#define	KEY_EXT extern
#endif
#include "includes.h"
/******位控制策略*****************************************************/
typedef union
{
	u16 halfword;
	struct
	{
		unsigned char bit0:1;
		unsigned char bit1:1;
		unsigned char bit2:1;
		unsigned char bit3:1;
		unsigned char bit4:1;	
		unsigned char bit5:1;
		unsigned char bit6:1;
		unsigned char bit7:1;
		unsigned char bit8:1;
	  unsigned char bit9:1;
	  unsigned char bit10:1;
	  unsigned char bit11:1;
	  unsigned char bit12:1;	
  	unsigned char bit13:1;
	  unsigned char bit14:1;
	  unsigned char bit15:1;
	}bits;
}BIT16_DEF;
typedef union
{
	u32 word;
	struct
	{
		u16 low;
		u16 high;
	}half;	
	struct
	{
		unsigned char bit0:1;
		unsigned char bit1:1;
		unsigned char bit2:1;
		unsigned char bit3:1;
		unsigned char bit4:1;	
		unsigned char bit5:1;
		unsigned char bit6:1;
		unsigned char bit7:1;
		unsigned char bit8:1;
	  unsigned char bit9:1;
	  unsigned char bit10:1;
	  unsigned char bit11:1;
	  unsigned char bit12:1;	
  	unsigned char bit13:1;
	  unsigned char bit14:1;
	  unsigned char bit15:1;
		unsigned char bit16:1;
		unsigned char bit17:1;
		unsigned char bit18:1;
		unsigned char bit19:1;
		unsigned char bit20:1;	
		unsigned char bit21:1;
		unsigned char bit22:1;
		unsigned char bit23:1;
		unsigned char bit24:1;
	  unsigned char bit25:1;
	  unsigned char bit26:1;
	  unsigned char bit27:1;
	  unsigned char bit28:1;	
  	unsigned char bit29:1;
	  unsigned char bit30:1;
	  unsigned char bit31:1;		
	}bits;	
	
}BIT32_DEF;
KEY_EXT BIT16_DEF led_panel;
KEY_EXT BIT32_DEF led_key;
KEY_EXT BIT32_DEF key_buff;
KEY_EXT char Key_value;
KEY_EXT char Str_KeyValue;
#define BEEP_bit        PBout(9)
#define LED_RCK 				PBout(1)						//595输出控制
#define KEY_LD					PDout(9)						//161装载控制
#define RCK_LED_KEY			PDout(8)						//前面板key
/***按键灯位定义************************************************************/
#define LED_KEY1 led_key.bits.bit0
#define LED_KEY2 led_key.bits.bit1
#define LED_KEY3 led_key.bits.bit2
#define LED_KEY4 led_key.bits.bit3
#define LED_KEY5 led_key.bits.bit4
#define LED_KEY6 led_key.bits.bit5
#define LED_KEY7 led_key.bits.bit6
#define LED_KEY8 led_key.bits.bit7
#define LED_KEY9 led_key.bits.bit8
#define LED_KEY10 led_key.bits.bit9
#define LED_KEY11 led_key.bits.bit10
#define LED_KEY12 led_key.bits.bit11
#define LED_KEY13 led_key.bits.bit12
#define LED_KEY14 led_key.bits.bit13
#define LED_KEY15 led_key.bits.bit14
#define LED_KEY16 led_key.bits.bit15
#define LED_KEY17 led_key.bits.bit16
#define LED_KEY18 led_key.bits.bit17
#define LED_KEY19 led_key.bits.bit18
#define LED_KEY20 led_key.bits.bit19
#define LED_KEY21 led_key.bits.bit20
#define LED_KEY22 led_key.bits.bit21
#define LED_KEY23 led_key.bits.bit22
#define LED_KEY24 led_key.bits.bit23
#define LED_KEY25 led_key.bits.bit24
#define LED_KEY26 led_key.bits.bit25
#define LED_KEY27 led_key.bits.bit26
#define LED_KEY28 led_key.bits.bit27
#define LED_KEY29 led_key.bits.bit28
#define LED_KEY30 led_key.bits.bit29
#define LED_KEY31 led_key.bits.bit30
#define LED_KEY32 led_key.bits.bit31
/***************************************************************/
//void beep_init(void);
u8 Led_Key(BIT16_DEF disp_panel,BIT32_DEF *dis_data,BIT32_DEF *key_data);
void KEY_Scan(u32 ke);
void key_exe(void);
void key_enter(void);
#endif  

