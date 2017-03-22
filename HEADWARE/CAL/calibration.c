#define CAL_GLOBALS
#include "calibration.h" 
#include "stdlib.h"
u8 send_buf[8]={0};
u8 cal_state = 0;//校准步骤
u16 Adjust_Vmax,Adjust_Imax,Adjust_Vmin,Adjust_Imin;
/******************************************************************************
* Function Name   : adjust_begin
* Description     : 发送校准开始命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_begin(void)//校准开始
{
	u8 send_buf[1] = {0};//校准步骤
	u16 add = Power_Addr;
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : adjust_volt_L
* Description     : 发送校准电压低点命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_volt_L(void)
{
	u16 add = Power_Addr;
	u8 send_buf[1] = {1};//发送校准步骤
	SYS_Flags.uB.receive&=0x0f;
	SYS_Flags.uB.receive |=0x10;//CAL_Vmin = 1;
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : adjust_volt_H
* Description     : 发送校准电压高点命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_volt_H(void)
{
	u16 add = Power_Addr;
	u8 send_buf[1] = {2};//发送校准步骤
	SYS_Flags.uB.receive&=0x0f;
	SYS_Flags.uB.receive |=0x20;//CAL_Vmax = 1;
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : adjust_curr_L
* Description     : 发送校准电流低点命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_curr_L(void)
{
	u16 add = Power_Addr;
	u8 send_buf[1] = {3};//发送校准步骤
	SYS_Flags.uB.receive&=0x0f;
	SYS_Flags.uB.receive |= 0x40;//CAL_Imin = 1;
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : adjust_curr_H
* Description     : 发送校准电流高点命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_curr_H(void)
{
	u16 add = Power_Addr;
	u8 send_buf[1] = {4};//发送校准步骤
	SYS_Flags.uB.receive&=0x0f;
	SYS_Flags.uB.receive |=0x80;//CAL_Imax = 1;
	Can_Send_Msg(send_buf,1,add);
}
/******************************************************************************
* Function Name   : adjust_end
* Description     : 发送校准结束命令
* Input           : none
* Return          : none
*******************************************************************************/
void adjust_end(void)//校准结束
{
	u8 send_buf0[6];
	u8 send_buf1[6];
	u16 add = Power_Addr;
	send_buf0[0] = 0x05;//校准步骤
	send_buf0[1] = 0;//为了区分发送的电压还是电流
	send_buf0[2] = Adjust_Vmin/256;
	send_buf0[3] = Adjust_Vmin%256;
	send_buf0[4] = Adjust_Vmax/256;
	send_buf0[5] = Adjust_Vmax%256;
	send_buf1[0] = 0x05;//校准步骤
	send_buf1[1] = 1;//为了区分发送的电压还是电流
	send_buf1[2] = Adjust_Imin/256;
	send_buf1[3] = Adjust_Imin%256;
	send_buf1[4] = Adjust_Imax/256;
	send_buf1[5] = Adjust_Imax%256;
	Can_Send_Msg(send_buf0,6,add);
	Can_Send_Msg(send_buf1,6,add);
}
/******************************************************************************
* Function Name   : calibration
* Description     : 校准控制程序，分步发送命令，并在最后发送校准的数据。
* Input           : none
* Return          : none
*******************************************************************************/
void calibration(u8 step)
{
	switch(step)
	{
		case 0://校准开始，先向上发送旧的校准数据
		{	
			adjust_begin();
			cal_state++;//发送begin后自动进入校准电压低点步骤
		}break;
		case 1:
		{
			adjust_volt_L();
		}break;
		case 2:
		{
			adjust_volt_H();
		}break;
		case 3:
		{
			adjust_curr_L();
		}break;
		case 4:
		{
			adjust_curr_H();
		}break;
		case 5:
		{
			adjust_end();
		}break;
		default:
		{

		}			break;
	}
}
