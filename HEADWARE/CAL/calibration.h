#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#ifdef CAL_GLOBALS
#define CAL_EXT
#else
#define CAL_EXT extern
#endif

#include "includes.h"
CAL_EXT u16 Adjust_Vmax,Adjust_Imax,Adjust_Vmin,Adjust_Imin;
CAL_EXT u8 cal_state;
#define Power_Addr 0x005     //���ʰ�Can��ַ
#define Fan_Addr   0x701     //���Ȱ�Can��ַ
void adjust_end(void);//У׼����
void calibration(u8 step);
#endif

