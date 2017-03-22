#ifndef _256128_H
#define _256128_H
#include "includes.h"
#ifdef	OLED_GLOBALS
#define	OLED_EXT
#else
#define	OLED_EXT extern
#endif

OLED_EXT u8 Flag_Window;

#define WD 256
#define HD 128
#define CS1 PCout(4)
#define CS2 PCout(5)
#define RSET PBout(0)

#define WDAT GPIOA->BSRR = GPIO_Pin_4
#define WCMD GPIOA->BRR = GPIO_Pin_4
#define Gray_valuel 0x77  //ª“∂»÷µ
void Write_CMD0(u8 cmd);
void Write_CMD1(u8 cmd);
void Write_DAT0(u8 dat);
void Write_DAT1(u8 dat);
void Set_Window(u8 row0,u8 row1,u16 col0,u16 col1);
void fill_screen(u8 data);
void fill_jg(u8 dat1,u8 dat2);
void DrawFourPoint(u8 x,u8 y,u8 gray_value);
void Conv_OneByte(u8 x,u8 y,u8 dat,u8 grag_value);
u32 mypow(u8 m,u8 n);
void showchar(u8 x,u8 y,char cha,u8 grag_value);
void showstring(u8 x,u8 y,char *p,u8 grag_value);
void shownum(u8 x,u8 y,u16 num,u8 n,u8 m,u8 en_zero,u8 grag_value);
void showchar24x32(u8 x,u8 y,u8 cha,u8 grag_value);
void shownum24x32(u8 x,u8 y,u16 num,u8 len,u8 dlen,u8 grag_value);
void OLED_Init(void);

void DrawLine(u8 x1, u8 y1, u8 x2, u8 y2);
void DrowRectL(u8 x,u8 y1,u8 y2,u8 Lx,u8 gray_value,u8 mode);
void DrowRectR(u8 x,u8 y1,u8 y2,u8 Rx,u8 gray_value,u8 mode);
void DrawRect(u8 x1, u8 y1, u8 x2, u8 y2,u8 gray_value,u8 mode1,u8 mode2);

//void show_logo(char *gImage);
void GUI_Init(void);
void Cal_GUI_Init(void);
void show_mess(void);
void Refresh_Data(void);

#endif
