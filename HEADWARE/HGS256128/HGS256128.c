#define OLED_GLOBALS
#include "HGS256128.h"
#include "font.h"
#include "stdlib.h"
/**************************************************************
*函数名 ：Write_CMD   Write_DAT                             
*形参   ：void                                              
*返回值 ：none                                              
*描述   ：上下半屏写命令 学数据                            
***************************************************************/

u8 Flag_Window;//切换界面 和 小窗口的开启和关闭 Flag_Window的赋值在key_exe函数中 操作完后 清除标志位
/***************************************************************
* Flag_Window = 0x03 数字键                                 
* Flag_Window = 0x05 backspace            
* Flag_Window = 0x04 关闭小窗口                                       
* Flag_Window = 0x10 切换系统界面
* Flag_Window = 0x08 切换校准界面
****************************************************************/
u8 Xpos_rev=212;
u8 Ypos_rev=7;
char *State_Output="\0";
char *State_Work = "STOP";

void Write_CMD0(u8 cmd)
{
	CS1=0;CS2=1;
	WCMD;
	SPI1_ReadWriteByte(cmd);
	CS1=1;CS2=1;
}
void Write_CMD1(u8 cmd)
{
	CS1=1;CS2=0;
	WCMD;
	SPI1_ReadWriteByte(cmd);
	CS1=1;CS2=1;	
}
void Write_DAT0(u8 dat)
{
	CS1=0;CS2=1;
	WDAT;
	SPI1_ReadWriteByte(dat);
	CS1=1;CS2=1;
	
}
void Write_DAT1(u8 dat)
{
	CS1=1;CS2=0;
	WDAT;
	SPI1_ReadWriteByte(dat);
	CS1=1;CS2=1;
}
/***************************************************************
*函数名 ：Set_Window                                        
*形参   ：u8 row0,u8 row1,u16 col0,u16 col1                 
*返回值 ：none                                             
*描述   ：开窗                                              
****************************************************************/
void Set_Window(u8 row0,u8 row1,u16 col0,u16 col1)
{
	if((row0<HD/2)&&(row1<HD/2))
	{
		Write_CMD0(0X15);
		Write_DAT0(0x1c+col0);//列地址
		Write_DAT0(0x1c+col1);
		Write_CMD0(0X75);
		Write_DAT0(row0);//行地址
		Write_DAT0(row1);
	}
	else if((row0<HD/2)&&(row1>=HD/2))
	{
		Write_CMD0(0X15);
		Write_DAT0(0x1c+col0);
		Write_DAT0(0x1c+col1);
		
		Write_CMD0(0X75);
		Write_DAT0(row0);
		Write_DAT0(HD/2-1);
		
		Write_CMD1(0X15);
		Write_DAT1(0x1c+col0);
		Write_DAT1(0x1c+col1);
		
		Write_CMD1(0X75);
		Write_DAT1(0);
		Write_DAT1(row1-HD/2);
	}
	else if((row0>=HD/2)&&(row1>=HD/2))
	{
		Write_CMD1(0X15);
		Write_DAT1(0x1c+col0);
		Write_DAT1(0x1c+col1);
		
		Write_CMD1(0X75);
		Write_DAT1(row0-HD/2);
		Write_DAT1(row1-HD/2);
	} 
}

/***************************************************************
*函数名 ：fill_screen                                       
*形参   ：data 灰度值  0-15                                 
*返回值 ：none                                              
*描述   ：填充                                              
****************************************************************/
void fill_screen(u8 data)
{
	u8 i,j,temp1,temp2;
	temp1=WD/4;
	temp2=2*temp1;
	Set_Window(0,HD-1,0,temp1-1);
	Write_CMD0(0X5C);
	Write_CMD1(0X5C);
	for(i=0;i<HD;i++)
	{
		if(i<HD/2)//上半屏
			for(j=0;j<temp2;j++)
					Write_DAT0(data);
			else //下半屏
			for(j=0;j<temp2;j++)
					Write_DAT1(data);
	}
}
/*******************************************************************************
* Function Name  : fill_jg
* Description    : 隔行显示 测试用
* Input          : dat1 第一行 dat2第二行
* Output         : none
* Return         : none
*******************************************************************************/
void fill_jg(u8 dat1,u8 dat2)
{ u8 i,j,temp1,temp2;
	temp1=WD/4;
  temp2=2*temp1; 
  Set_Window(0,HD-1,0,temp1-1); 
	Write_CMD0(0x5c);
	Write_CMD1(0x5c);
	for(i=0;i<HD;i++)
	{
		if(i<HD/2)
		{ 
			for(j=0;j<temp2;j++)
			if(i%2==0)
			Write_DAT0(dat1);
			else
			Write_DAT0(dat2);
		}
	else
	{ 
		for(j=0;j<temp2;j++)
		if(i%2==0)
		Write_DAT1(dat1);
		else
		Write_DAT1(dat2);
	}
  }
}
/*******************************************************************************
* Function Name  : DrawFourPoint
* Description    : 可书写的最小单位4个点
* Input          : x,y 位置 gray_value灰度信息 0-ff 代表两个点
* Output         : none
* Return         : none
*******************************************************************************/
void DrawFourPoint(u8 x,u8 y,u8 gray_value) //Y为奇数 最少写4个点
{
	x>>=2;
	Set_Window(y,y,x,x); 
	Write_CMD0(0X5C);
	Write_CMD1(0X5C);
	if(y>=HD/2)
	{
		Write_DAT1(gray_value&0xf0);
		Write_DAT1(gray_value&0x00);
	}
	else
	{
		Write_DAT0(gray_value&0xf0);
		Write_DAT0(gray_value&0x00);
	}	
}
/*******************************************************************************
* Function Name  : Conv_OneByte
* Description    : 转换一个字节 一个字节对应8个点
* Input          : x,y 位置 dat字节信息
* Output         : none
* Return         : none
*******************************************************************************/
void Conv_OneByte(u8 x,u8 y,u8 dat,u8 grag_value)
{
	u8 i,temp[4];
	Set_Window(y,y,x,x+1);
	Write_CMD0(0X5C);
	Write_CMD1(0X5C);
	if(grag_value==0) // 如果grag_value=0 则反显
	{
		for(i=0;i<4;i++)
		{
			switch(dat&0x03)
			{
				case 0x00:temp[i]=0xFF&Gray_valuel;break;
				case 0x01:temp[i]=0x0F&Gray_valuel;break;
				case 0x02:temp[i]=0xF0&Gray_valuel;break;
				case 0x03:temp[i]=0x00;break;
			}		
			dat>>=2;
		}
		if(y<HD/2)
		{
			for(i=0;i<4;i++)
			Write_DAT0(temp[i]);
			Write_DAT0(Gray_valuel);//写的最小单位为4个点 实际情况 不加这个 有4个点无法显示 
		}
		else
		{
			for(i=0;i<4;i++)
			Write_DAT1(temp[i]);
			Write_DAT1(Gray_valuel);
		}
 }
		else
		{
			for(i=0;i<4;i++)
			{
				switch(dat&0x03)
				{
					case 0x00:temp[i]=0x00;break;
					case 0x01:temp[i]=0xf0&grag_value;break;
					case 0x02:temp[i]=0x0f&grag_value;break;
					case 0x03:temp[i]=0xff&grag_value;break;
				}		
				dat>>=2;
			}
			if(y<HD/2)
			{
				for(i=0;i<4;i++)
				Write_DAT0(temp[i]);
				Write_DAT0(0);//写的最小单位为4个点 实际情况 不加这个 有4个点无法显示 
			}
			else
			{
				for(i=0;i<4;i++)
				Write_DAT1(temp[i]);
				Write_DAT1(0);
			}
	}
}
/*******************************************************************************
* Function Name  : showchar
* Description    : 可以输出一个字符
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void showchar(u8 x,u8 y,char cha,u8 grag_value)
{
	u8 pos,temp;
	cha=cha-' ';
	x>>=2;
	for(pos=0;pos<12;pos++)
	{
		temp=asc2_1206[cha][pos];
		Conv_OneByte(x,y,temp,grag_value);
		y++;
	}
}
/*******************************************************************************
* Function Name  : showchar
* Description    : 可以输出一个24*32的字符
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void showchar24x32(u8 x,u8 y,u8 cha,u8 grag_value)
{
	u8 pos;
	u8 tem1=0;
	u8 tem2=0;
	u8 tem3=0;
	x>>=2;
	for(pos=0;pos<96;pos+=3)
	{
		tem1=asc_3248[cha][pos];
		tem2=asc_3248[cha][pos+1];
		tem3=asc_3248[cha][pos+2];
		Conv_OneByte(x,y,tem1,grag_value);
		Conv_OneByte((x+2),y,tem2,grag_value);
		Conv_OneByte((x+4),y,tem3,grag_value);
		y++;
	}
}
/*******************************************************************************
* Function Name  : showstring
* Description    : 可以输出一段字符串
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void showstring(u8 x,u8 y,char *p,u8 grag_value)
{
	while(*p!='\0')
	{
		showchar(x,y,*p,grag_value);
		if(*p=='.')
		{
			x+=4;
			p++;
		}
		else
		{
			x+=8;//平移八个点
			p++;//字符串位置加1 最好准备显示下一个字符
		}
	}
}
/*******************************************************************************
* Function Name  : Dorp_Point(u8 x,u8 y,u8 gray_value)
* Description    : 显示一个小数点
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Dorp_Point(u8 x,u8 y,u8 gray_value)
{
		x>>=2;
		Set_Window(y,y,x,x); 
		Write_CMD0(0X5C);
		Write_CMD1(0X5C);
		if(y>=HD/2)
		{
			Write_DAT1(gray_value&0xf0);
			Write_DAT1(gray_value&0x00);
		}
		else
		{
			Write_DAT0(gray_value&0xf0);
			Write_DAT0(gray_value&0x00);
		}	
}
/*******************************************************************************
* Function Name  : Dorp_Point24x32(u8 x,u8 y,u8 gray_value)
* Description    : 显示一个大个的小数点
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void Dorp_Point24x32(u8 x,u8 y,u8 cha,u8 grag_value)
{
	u8 pos;
	u8 tem1=0;
	u8 tem2=0;
	x>>=2;
	for(pos=0;pos<96;pos+=3)
	{
		tem1=asc_3248[cha][pos];
		tem2=asc_3248[cha][pos+1];
		Conv_OneByte(x,y,tem1,grag_value);
		Conv_OneByte((x+2),y,tem2,grag_value);
		y++;
	}
}
/*******************************************************************************
* Function Name  : mypow
* Description    : m的n次方
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result = 1;	 
	while(n--) result *= m;    
	return result;
}	
/*******************************************************************************
* Function Name  : shownum
* Description    : 输出变量数值
* Input          : x,y位置 num(数字) len长度 dlen小数点后几位
* Output         : none
* Return         : none
*******************************************************************************/
void shownum(u8 x,u8 y,u16 num,u8 len,u8 dlen,u8 en_zero,u8 grag_value)//decimal length
{
	u8 t,temp;
	u8 enshow=en_zero; //消零 用
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len - t - 1))%10;//每次计算最高位
		if(enshow == 0&&t < (len-dlen-1))//在小数点前一位查询，是否有零
		{
			if(temp == 0)//如果是零，则不显示
			{
				showchar(x+(8*t),y,' ',grag_value);
				continue;
			}else enshow = 1;	//已经不是零了
		}
		if(t==(len-dlen))
		{Dorp_Point(x+(8*t),y+9,grag_value);x+=4;}//根据小数点后几位和数值的位数 确定小数点的位置
		if(((x+(8*t))==Tem_Data.Xpos)&&(y==Tem_Data.Ypos))
			showchar(x+(8*t),y,temp+'0',0);//反显
		else
		showchar(x+(8*t),y,temp+'0',grag_value); //显示数字
	}
}
/*******************************************************************************
* Function Name  : shownum
* Description    : 输出变量数值 大小为24*32
* Input          : x,y位置 num(数字) len长度 dlen小数点后几位
* Output         : none
* Return         : none
*******************************************************************************/
void shownum24x32(u8 x,u8 y,u16 num,u8 len,u8 dlen,u8 grag_value)
{
	u8 t,temp,enshow=0;
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len - t - 1))%10;//每次计算最高位
		if(enshow == 0&&t < (len-dlen-1))//在小数点前一位查询，是否有零
		{
			if(temp == 0)//如果是零，则不显示
			{
				showchar24x32(x+(24*t),y,10,grag_value);//空白
				continue;
			}else enshow = 1;	//已经不是零了
		}
		if(t==(len-dlen))
			//{showchar24x32(x+(24*t),y,11,grag_value);x+=16;}//根据小数点后几位和数值的位数 确定小数点的位置
			 {Dorp_Point24x32(x+(24*t),y,11,grag_value);x+=16;}
		showchar24x32(x+(24*t),y,temp,grag_value); //显示数字
	}
}
/*******************************************************************************
* Function Name  : DrawLine
* Description    : 划线 只能划横线和竖线 竖线的话 宽度在1和2 横线随意
* Input          : void
* Output         : none
* Return         : none
********************************************************************************/
void DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 t; 
	u8 kuang=1;//竖线宽度
	int delta_x=0,delta_y=0,temp1=0,temp2=0;  
	x1>>=2;
	x2>>=2;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	if(delta_y>delta_x)//要竖线
	{
		if(kuang==1)//竖线宽度
		{
			temp1=0xf0;
			temp2=0x00;
		}
		if(kuang==2)
		{
			temp1=0xff;
			temp2=0x00;
		}
		for(t=y1;t<=y2;t++)
		{
			Set_Window(t,t,x1,x1); 
			Write_CMD0(0X5C);
			Write_CMD1(0X5C);
			if(t>=HD/2)
			{
				Write_DAT1(temp1);
				Write_DAT1(temp2);
			}
			else
			{
				Write_DAT0(temp1);
				Write_DAT0(temp2);
			}	
		}
	}
	else//划横线
	{
		for(y1=y1;y1<=y2;y1++)
		{
			temp1=0xff;
			temp2=0xff;
			for(t=x1;t<=x2;t++)
			{
				Set_Window(y1,y1,t,t); 
				Write_CMD0(0X5C);
				Write_CMD1(0X5C);
				if(y1>=HD/2)
				{
					Write_DAT1(temp1);
					Write_DAT1(temp2);
				}
				else
				{
					Write_DAT0(temp1);
					Write_DAT0(temp2);
				}	
			}
		}
	}
}
/*******************************************************************************
* Function Name  : DrawRect
* Description    : 画实心矩形，大小、位置任意 但是 涉及到重叠的矩形(但是涉及到重叠的时候 比如扣除一块 要注意边界问题)
* Input          : u8 mode1,u8 mode2  mode是为了处理边界时 不好处理时 就用这种自定义的模式
* Output         : none
* Return         : none
********************************************************************************/
void DrawRect(u8 x1, u8 y1, u8 x2, u8 y2,u8 gray_value,u8 mode1,u8 mode2)
{
	u16 t,y=y1;
	int delta_x,temp1,temp2;  
  u8 L=x1%4;
  u8 R=x2%4; 
	x1>>=2;
	x2>>=2;
	delta_x=x2-x1; //计算坐标增量 
	if(gray_value==0)
	{
		delta_x-=1; //计算坐标增量 
	} 	

	DrowRectL(x1,y1,y2,L,gray_value,mode1);//左边界
	DrowRectR(x2,y1,y2,R,gray_value,mode2);//右边界
	for(y=y;y<=y2;y++)
	{
		temp1=gray_value;
		temp2=gray_value;
		for(t=x1+1;t<=delta_x+x1;t++)
		{
			Set_Window(y,y,t,t); 
			Write_CMD0(0X5C);
			Write_CMD1(0X5C);
			if(y>=HD/2)
			{
				Write_DAT1(temp1);
				Write_DAT1(temp2);
			}
			else
			{
				Write_DAT0(temp1);
				Write_DAT0(temp2);
			}	
		}
	}	 
}
/*******************************************************************************
* Function Name  : DrawRectL
* Description    : 涉及到边界x不是4的整数倍时 要特殊处理边界问题  
* Input          : mode是为了处理边界时 不好处理时 就用这种自定义的模式
* Output         : none
* Return         : none
********************************************************************************/
void DrowRectL(u8 x,u8 y1,u8 y2,u8 Lx,u8 gray_value,u8 mode)
{
	u8 temp1,temp2;
	if(mode==1)// 特殊地方 用户自定义模式 
	{
		if(Lx==0)
		{
			temp1=0;
			temp2=0;
		}
		else if(Lx==1)
		{
			temp1=0x0F&Gray_valuel;
			temp2=0xF0&Gray_valuel;
		}
		else if(Lx==2)
		{
			temp1=0x0F&Gray_valuel;
			temp2=0xF0&Gray_valuel;
		}
		else
		{
			temp1=0x00&Gray_valuel;
			temp2=0xF0&Gray_valuel;
		}
	}
	else
	{
		if(gray_value==0)
		{
			if(Lx==0)
			{
				temp1=0;
				temp2=0;
			}
			else if(Lx==1)
			{
				temp1=0xF0&Gray_valuel;
				temp2=0x00&Gray_valuel;
			}
			else if(Lx==2)
			{
				temp1=0xFF&Gray_valuel;
				temp2=0x00;
			}
			else
			{
				temp1=0xFF&Gray_valuel;
				temp2=0xF0&Gray_valuel;
			}
		}
		else
		{
			if(Lx==0)
			{
				temp1=gray_value;
				temp2=gray_value;
			}
			else if(Lx==1)
			{
				temp1=gray_value&0x0f;
				temp2=gray_value;
			}
			else if(Lx==2)
			{
				temp1=0;
				temp2=gray_value;
			}
			else
			{
				temp1=0;
				temp2=gray_value*0x0f;
			}
		}
	}
	for(y1=y1;y1<=y2;y1++)
	{
		Set_Window(y1,y1,x,x); 
		Write_CMD0(0X5C);
		Write_CMD1(0X5C);
		if(y1>=HD/2)
		{
			Write_DAT1(temp1);
			Write_DAT1(temp2);
		}
		else
		{
			Write_DAT0(temp1);
			Write_DAT0(temp2);
		}		
	}
}

/*******************************************************************************
* Function Name  : DrawRectL
* Description    : 涉及到边界x不是4的整数倍时 要特殊处理边界问题  
* Input          : mode是为了处理边界时 不好处理时 就用这种自定义的模式
* Output         : none
* Return         : none
********************************************************************************/
void DrowRectR(u8 x,u8 y1,u8 y2,u8 Rx,u8 gray_value,u8 mode)
{
	u8 temp1,temp2;
	if(mode==1)
	{
		if(Rx==0)
		{
			temp1=0;
			temp2=0;
		}
		else if(Rx==1)
		{
			temp1=0xF0&Gray_valuel;
			temp2=0x00;
		}
		else if(Rx==2)
		{
			temp1=0xFF&Gray_valuel;
			temp2=0x00;
		}
		else
		{
			temp1=0x00;
			temp2=0xF0&Gray_valuel;
		}
	}
	else
	{
		if(gray_value==0)
		{
			if(Rx==0)
			{
				temp1=0x0F&Gray_valuel;
				temp2=0xFF&Gray_valuel;
			}
			else if(Rx==1)
			{
				temp1=0;
				temp2=0xFF&Gray_valuel;
			}
			else if(Rx==2)
			{
				temp1=0x00;
				temp2=0x0F&Gray_valuel;
			}
			else if(Rx==3)
			{
				temp1=0;
				temp2=0;
			}
		}
		else
		{
			if(Rx==0)
			{
				temp1=0;
				temp2=0;
			}
			else if(Rx==1)
			{
				temp1=gray_value&0xF0;
				temp2=0;
			}
			else if(Rx==2)
			{
				temp1=gray_value;
				temp2=0;
			}
			else if(Rx==3)
			{
				temp1=gray_value;
				temp2=gray_value*0xF0;
			}
		}
	}
	for(y1=y1;y1<=y2;y1++)
	{
		Set_Window(y1,y1,x,x); 
		Write_CMD0(0X5C);
		Write_CMD1(0X5C);
		if(y1>=HD/2)
		{
			Write_DAT1(temp1);
			Write_DAT1(temp2);
		}
		else
		{
			Write_DAT0(temp1);
			Write_DAT0(temp2);
		}		
	}
}
/*******************************************************************************
* Function Name  : GUI_Init
* Description    : 系统界面 
* Input          : none
* Output         : none
* Return         : none
********************************************************************************/
void GUI_Init(void)
{
	fill_screen(0);
	DrawRect(176, 1, 255, 40,Gray_valuel,0,0);//右上角第一块
	DrawRect(184, 5, 253, 36,0,0,0);
	showstring(180,1,"S",0X00);
	showstring(180,13,"E",0X00);
	showstring(180,25,"T",0X00);
	showstring(244,7,"V",Gray_valuel);
	showstring(244,20,"A",Gray_valuel);

  DrawRect(176, 43, 215, 83,Gray_valuel,0,0);//右中第一块
	showstring(184,43,"P.O.D",0X00);
	DrawRect(178, 56, 213, 78,0x00,0,0); 

	DrawRect(217, 43,255, 83,Gray_valuel,0,0);//右中第二块
	showstring(222,43,"P.O.I",0X00);
	DrawRect(218, 56, 253, 78,0x00,1,0);
	showstring(204,60,"s",Gray_valuel);
	showstring(244,60,"s",Gray_valuel);

	DrawRect(176, 86, 255, 127,Gray_valuel,0,0);//右下
  DrawRect(178, 89, 253, 123,0,0,0);
	showstring(180,90,"STA SCR:",Gray_valuel); 
	showstring(180,102,"SCR1",Gray_valuel);
  showstring(180,112,"SCR2",Gray_valuel); 	
	
	DrawRect(0, 0, 170, 127,Gray_valuel,0,0);
	DrawRect(3, 3, 168, 124,0,0,0);
	showchar24x32(136,30,12,Gray_valuel);//V
	showchar24x32(136,64,13,Gray_valuel);//A
}
/*******************************************************************************
* Function Name  : Cal_GUI_Init
* Description    : 校准界面 
* Input          : none
* Output         : none
* Return         : none
********************************************************************************/
void Cal_GUI_Init(void)
{
	fill_screen(0);
	showstring(8,16,"VMin",0xff);showstring(96,16,"V",0xff);showstring(176,16,"V",0xff);
	shownum(60,16,Set_Vmin,5,3,0,0xff);
	showstring(128,16,"--.---",0xff);
	showstring(8,16*2,"VMax",0xff);showstring(96,16*2,"V",0xff);showstring(176,16*2,"V",0xff);
	shownum(60,16*2,Set_Vmax,5,3,0,0xff);
	showstring(128,16*2,"--.---",0xff);
	showstring(8,16*3,"IMin",0xff);showstring(96,16*3,"A",0xff);showstring(176,16*3,"A",0xff);
	shownum(60,16*3,Set_Imin,5,3,0,0xff);
	showstring(128,16*3,"--.---",0xff);
	showstring(8,16*4,"IMax",0xff);showstring(96,16*4,"A",0xff);showstring(176,16*4,"A",0xff);
	shownum(60,16*4,Set_Imax,5,3,0,0xff);
	showstring(128,16*4,"--.---",0xff);	
}
/*******************************************************************************
* Function Name  : show_mess
* Description    : 显示预设值和实际值
* Input          : 灰度值
* Output         : none
* Return         : none
********************************************************************************/
void show_mess(void)
{	 
	/*反显项 指示当前在哪一步*/
	if(cal_Vmin ==1) showstring(8,16,"VMin",0);
	else showstring(8,16,"VMin",0xff);
	
	if(cal_Vmax ==1) showstring(8,16*2,"VMax",0);
	else  showstring(8,16*2,"VMax",0xff);
	
	if(cal_Imin == 1)showstring(8,16*3,"IMin",0);
	else showstring(8,16*3,"IMin",0XFF);
	
	if(cal_Imax == 1) showstring(8,16*4,"IMax",0);
	else showstring(8,16*4,"IMax",0XFF);
	/*查询是否有数字按键操作 如果有 处理, 没有 跳过 */
	Hanle_NumKey(Str_KeyValue);	
	/*在哪一步 显示哪一步数据 */
	switch(cal_state)
	{
		case 0x01:
		{
			showstring(128,16,String_key,0xff);//正在输入VMin
		}break;
		case 0x02:
		{
			shownum(128,16,Adjust_Vmin,5,3,0,0xff);//VMin输入完成后，直接显示Adjust_Vmin
			showstring(200,16,"OK",0xff);//指示此步校准完成
			showstring(128,16*2,String_key,0xff);//正在输入VMax
		}break;
		case 0x03:
		{
				shownum(128,16*2,Adjust_Vmax,5,3,0,0xff);	
				showstring(200,16*2,"OK",0xff);
				showstring(128,16*3,String_key,0xff);
		}break;
		case 0x04:
		{
				shownum(128,16*3,Adjust_Imin,5,3,0,0xff);
				showstring(200,16*3,"OK",0xff);
        showstring(128,16*4,String_key,0xff);
		}break;
		case 0x05:
		{
				shownum(128,16*4,Adjust_Imax,5,3,0,0xff);
				showstring(200,16*4,"OK",0xff);
				showstring(4,16*5,"Calibrate Success.",0xff);//校准成功
				showstring(4,16*6,"Press the system key.",0xff);//请按系统按键 进入系统界面
		}break;
		default:{}break;
	}
}
/*******************************************************************************
* Function Name  : Refresh_Data
* Description    : 刷新数据 包括电压 电流 时间设定值 实时输出值 校准情况下的输入值 
* Input          : 灰度值
* Output         : none
* Return         : none
********************************************************************************/
void Refresh_Data(void)
{
	if(SYS_cal == 0)//正常功能
	{
		if(Flag_Window == 0x10)//放在前边 为了界面切换时 先刷GUI 再刷数据
		{
			GUI_Init();
			Flag_Window &= ~0x10;
		}
		shownum(204,7,Set_Volt.number,4,2,1,Gray_valuel);//电压设置值
		shownum(204,20,Set_Curr.number,4,2,1,Gray_valuel);//电流设置值
		shownum(180,60,Set_Tdly.number,3,0,1,Gray_valuel);//延迟
		shownum(220,60,Set_Tinvl.number,3,0,1,Gray_valuel);//间隔
		showstring(220,102,"OK",Gray_valuel);//SCR1状态
		showstring(220,112,"ERR",Gray_valuel);//SCR2状态
		switch(SYS_Flags.uB.receive&0x03)
		{
			case 0x03:State_Output="CV";State_Work="RUN";break;
			case 0x02:State_Output="\0";State_Work="STOP";break;
			case 0x01:State_Output="CC";State_Work="RUN";break;
			case 0X00:State_Output="\0";State_Work="STOP";break;
			default:break;
		}
		showstring(80,4,State_Output,Gray_valuel);//cc or cv
		showstring(8,4,State_Work,Gray_valuel);//run or stop
//		if(Comm_Count>=100)//通信故障警告
//		{
//			if((Comm_Count%4)<=1)
//			showstring(120,4,"A.C.M",0xff);
//			else
//				showstring(120,4,"     ",0xff);
//		}
//		else
//			showstring(120,4,"     ",0xff);
		shownum24x32(24,30,Dis_Volt,4,2,Gray_valuel);//电压实时显示值
		shownum24x32(24,64,Dis_Curr,4,2,Gray_valuel);//电流实时显示值
		if(Flag_Window&0x01) //系统模式下 数字键设置各项设定值 开启小窗口
		{
			Open_Window(Str_KeyValue);  
			Flag_Window&=~0x01;		
		}
		if(Flag_Window == 0x40) //关闭小窗口
		{
			Colse_Window(60,7,164,30);
			Flag_Window&=~0x40;
		}
	}
	else//校准模式下
	{
		if(Flag_Window == 0x08)//切换校准界面
		{	
			Cal_GUI_Init();
			Flag_Window &= ~0x08;
		}
		show_mess();//校准数据刷新
	}
}

/*******************************************************************************
* Function Name  : show_logo
* Description    : 显示一幅256*128的图片
* Input          : void
* Output         : none
* Return         : none
********************************************************************************/
//void show_logo(char *gImage)
//{
//	u16 i;
//	u8 j,temp1,dat,temp[3];
//	temp1=WD/4;
//	Set_Window(0,HD-1,0,temp1-1);
//	Write_CMD0(0X5C);
//	Write_CMD1(0X5C);
//	for(i=0;i<4096;i++)//128*64(64*4=256)
//	{
//		dat=gImage[i];
//		for(j=0;j<4;j++)//每个字节可以显示八个点
//		{
//			switch(dat&0x03)
//			{
//				case 0x00:temp[j]=0x00;break;
//				case 0x01:temp[j]=0xf0;break;
//				case 0x02:temp[j]=0x0f;break;
//				case 0x03:temp[j]=0xff;break;
//			}		
//			dat>>=2;
//		}
//		if(i<2048)//上半屏
//		{
//			for(j=0;j<4;j++)
//			Write_DAT0(temp[j]);
//		}
//		else//下半屏
//		{
//			for(j=0;j<4;j++)
//			Write_DAT1(temp[j]);
//		}
//	}
//}
/*******************************************************************************
* Function Name  : OLED_Init
* Description    : 初始化OLED
* Input          : void
* Output         : none
* Return         : none
********************************************************************************/
void OLED_Init(void)
{
	RSET=0;
	delay_us(10);
	RSET=1;
	delay_us(100);
	
	Write_CMD0(0XFD);//准备接受命令
	Write_DAT0(0X12);
	Write_CMD0(0XAE);//开休眠 显示关
	Write_CMD0(0X15);//设置列窗口
	Write_DAT0(0X1C);
	Write_DAT0(0X5B);
	Write_CMD0(0X75);//设置行窗口
	Write_DAT0(0X00);
	Write_DAT0(0X3F);	
	Write_CMD0(0XA0);//设置位址重新印象和双重COM模式
	Write_DAT0(0X04);//选择高字节在前，com反向扫描，水平模式
	//水平地址增量 禁止列地址重新印象 启动半字节重新映像 
	//com0-com[n-1] 禁止奇偶分离 启动双重模式
	Write_DAT0(0X11);//选择双屏模式
	Write_CMD0(0XA1);//设置起始行
	Write_DAT0(0X00);//0	
	Write_CMD0(0XA2);//设置显示偏移量
	Write_DAT0(0X00);//无分支	
	Write_CMD0(0XA6);//设置显示模式 正常显示
	Write_CMD0(0XA6);//不反显
	Write_CMD0(0XA9);//设置部分显示 a8进入部分显示，a9退出
	Write_CMD0(0XAB);//设置VDD来源
	Write_DAT0(0X01);//00 外部 01H内部
	Write_CMD0(0XB1);//设置相位长度
	Write_DAT0(0XE7);// P1=29 P2=7
	Write_CMD0(0XB3);//设置D和Fosc
	Write_DAT0(0X91);//F=9 D=1	
	Write_CMD0(0XB4);//设置显示质量和vsl来源
	Write_DAT0(0XA0);//A0H为外部Vsl，A2H为内部Vsl
	Write_DAT0(0xfd);//FDH 提高质量 B5H正常 
	Write_CMD0(0XB5);//设置GPIO
	Write_DAT0(0X00);
	Write_CMD0(0XB6);//设置第二预先充电周期
	Write_DAT0(0X08);
	Write_CMD0(0XB8);//设置灰度值
	Write_DAT0(0x0c);
	Write_DAT0(0x18);
	Write_DAT0(0x24);
	Write_DAT0(0x30);
	Write_DAT0(0x3c);
	Write_DAT0(0x48);
	Write_DAT0(0x54);
	Write_DAT0(0x60);
	Write_DAT0(0x6c);
	Write_DAT0(0x78);
	Write_DAT0(0x84);
	Write_DAT0(0x90);
	Write_DAT0(0x9c);
	Write_DAT0(0xa8);
	Write_DAT0(0xb4);
	Write_CMD0(0X00);//使能灰阶
	Write_CMD0(0XBB);//设置预充电电压
	Write_DAT0(0x1F);
	Write_CMD0(0XBE);//设置Vcomh电压
	Write_DAT0(0x07);
	Write_CMD0(0XC1);//设置对比度
	Write_DAT0(0x7F);	////////////////////////////////////////////////////////
	Write_CMD0(0XC7);//设置SEG电流范围
	Write_DAT0(0x0F);//满范围	
	Write_CMD0(0XCA);//设置显示起始行
	Write_DAT0(0x3F);
	Write_CMD0(0XD1);//设置显示质量
	Write_DAT0(0xA2);
	Write_DAT0(0x20);
	Write_CMD0(0XAF);//开显示
	
	Write_CMD1(0XFD);//准备接受命令
	Write_DAT1(0X12);
	Write_CMD1(0XAE);//开休眠
	Write_CMD1(0X15);//设置列窗口
	Write_DAT1(0X1C);
	Write_DAT1(0X5B);
	Write_CMD1(0X75);//设置行窗口
	Write_DAT1(0X00);
	Write_DAT1(0X3F);	
	Write_CMD1(0XA0);//设置可逆地图
	Write_DAT1(0X16);//选择高字节在前，com反向扫描，水平模式
	Write_DAT1(0X11);//选择双屏模式
	Write_CMD1(0XA1);//设置起始行
	Write_DAT1(0X00);//0	
	Write_CMD1(0XA2);//设置
	Write_DAT1(0X00);//无分支	
	Write_CMD1(0XA6);//设置显示模式
	Write_CMD1(0XA6);//不反显
	Write_CMD1(0XA9);//设置部分显示 a8进入部分显示，a9退出
	Write_CMD1(0XAB);//设置VDD来源
	Write_DAT1(0X01);//00 外部 01H内部
	Write_CMD1(0XB1);//设置P1 P2
	Write_DAT1(0XE7);// P1=29 P2=7
	Write_CMD1(0XB3);//设置D和Fosc
	Write_DAT1(0X91);//F=9 D=1	
	Write_CMD1(0XB4);//设置显示质量和vsl来源
	Write_DAT1(0XA0);//A0H为外部Vsl，A2H为内部Vsl
	Write_DAT1(0xfd);//FDH 提高质量 B5H正常 
	Write_CMD1(0XB5);
	Write_DAT1(0X00);
	Write_CMD1(0XB6);
	Write_DAT1(0X08);
	Write_CMD1(0XB8);//设置灰度值
	Write_DAT1(0x0c);
	Write_DAT1(0x18);
	Write_DAT1(0x24);
	Write_DAT1(0x30);
	Write_DAT1(0x3c);
	Write_DAT1(0x48);
	Write_DAT1(0x54);
	Write_DAT1(0x60);
	Write_DAT1(0x6c);
	Write_DAT1(0x78);
	Write_DAT1(0x84);
	Write_DAT1(0x90);
	Write_DAT1(0x9c);
	Write_DAT1(0xa8);
	Write_DAT1(0xb4);
	Write_CMD1(0X00);//使能灰阶
	Write_CMD1(0XBB);//设置VP2
	Write_DAT1(0x1F);
	Write_CMD1(0XBE);//设置Vcomh
	Write_DAT1(0x07);
	Write_CMD1(0XC1);//设置对比度
	Write_DAT1(0x7F);	
	Write_CMD1(0XC7);//设置SEG电流范围
	Write_DAT1(0x0F);//满范围	
	Write_CMD1(0XCA);//设置显示起始行
	Write_DAT1(0x3F);
	Write_CMD1(0XD1);//设置显示质量
	Write_DAT1(0xA2);
	Write_DAT1(0x20);
	Write_CMD1(0XAF);//开显示
	
	fill_screen(0X00);
} 

