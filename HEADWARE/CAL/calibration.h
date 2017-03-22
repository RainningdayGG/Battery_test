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
#define Power_Addr 0x005     //功率板Can地址
#define Fan_Addr   0x701     //风扇板Can地址
void adjust_end(void);//校准结束
void calibration(u8 step);
#endif

