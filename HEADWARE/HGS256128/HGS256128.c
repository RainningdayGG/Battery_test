#define OLED_GLOBALS
#include "HGS256128.h"
#include "font.h"
#include "stdlib.h"
/**************************************************************
*������ ��Write_CMD   Write_DAT                             
*�β�   ��void                                              
*����ֵ ��none                                              
*����   �����°���д���� ѧ����                            
***************************************************************/

u8 Flag_Window;//�л����� �� С���ڵĿ����͹ر� Flag_Window�ĸ�ֵ��key_exe������ ������� �����־λ
/***************************************************************
* Flag_Window = 0x03 ���ּ�                                 
* Flag_Window = 0x05 backspace            
* Flag_Window = 0x04 �ر�С����                                       
* Flag_Window = 0x10 �л�ϵͳ����
* Flag_Window = 0x08 �л�У׼����
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
*������ ��Set_Window                                        
*�β�   ��u8 row0,u8 row1,u16 col0,u16 col1                 
*����ֵ ��none                                             
*����   ������                                              
****************************************************************/
void Set_Window(u8 row0,u8 row1,u16 col0,u16 col1)
{
	if((row0<HD/2)&&(row1<HD/2))
	{
		Write_CMD0(0X15);
		Write_DAT0(0x1c+col0);//�е�ַ
		Write_DAT0(0x1c+col1);
		Write_CMD0(0X75);
		Write_DAT0(row0);//�е�ַ
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
*������ ��fill_screen                                       
*�β�   ��data �Ҷ�ֵ  0-15                                 
*����ֵ ��none                                              
*����   �����                                              
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
		if(i<HD/2)//�ϰ���
			for(j=0;j<temp2;j++)
					Write_DAT0(data);
			else //�°���
			for(j=0;j<temp2;j++)
					Write_DAT1(data);
	}
}
/*******************************************************************************
* Function Name  : fill_jg
* Description    : ������ʾ ������
* Input          : dat1 ��һ�� dat2�ڶ���
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
* Description    : ����д����С��λ4����
* Input          : x,y λ�� gray_value�Ҷ���Ϣ 0-ff ����������
* Output         : none
* Return         : none
*******************************************************************************/
void DrawFourPoint(u8 x,u8 y,u8 gray_value) //YΪ���� ����д4����
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
* Description    : ת��һ���ֽ� һ���ֽڶ�Ӧ8����
* Input          : x,y λ�� dat�ֽ���Ϣ
* Output         : none
* Return         : none
*******************************************************************************/
void Conv_OneByte(u8 x,u8 y,u8 dat,u8 grag_value)
{
	u8 i,temp[4];
	Set_Window(y,y,x,x+1);
	Write_CMD0(0X5C);
	Write_CMD1(0X5C);
	if(grag_value==0) // ���grag_value=0 ����
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
			Write_DAT0(Gray_valuel);//д����С��λΪ4���� ʵ����� ������� ��4�����޷���ʾ 
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
				Write_DAT0(0);//д����С��λΪ4���� ʵ����� ������� ��4�����޷���ʾ 
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
* Description    : �������һ���ַ�
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
* Description    : �������һ��24*32���ַ�
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
* Description    : �������һ���ַ���
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
			x+=8;//ƽ�ư˸���
			p++;//�ַ���λ�ü�1 ���׼����ʾ��һ���ַ�
		}
	}
}
/*******************************************************************************
* Function Name  : Dorp_Point(u8 x,u8 y,u8 gray_value)
* Description    : ��ʾһ��С����
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
* Description    : ��ʾһ�������С����
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
* Description    : m��n�η�
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
* Description    : ���������ֵ
* Input          : x,yλ�� num(����) len���� dlenС�����λ
* Output         : none
* Return         : none
*******************************************************************************/
void shownum(u8 x,u8 y,u16 num,u8 len,u8 dlen,u8 en_zero,u8 grag_value)//decimal length
{
	u8 t,temp;
	u8 enshow=en_zero; //���� ��
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len - t - 1))%10;//ÿ�μ������λ
		if(enshow == 0&&t < (len-dlen-1))//��С����ǰһλ��ѯ���Ƿ�����
		{
			if(temp == 0)//������㣬����ʾ
			{
				showchar(x+(8*t),y,' ',grag_value);
				continue;
			}else enshow = 1;	//�Ѿ���������
		}
		if(t==(len-dlen))
		{Dorp_Point(x+(8*t),y+9,grag_value);x+=4;}//����С�����λ����ֵ��λ�� ȷ��С�����λ��
		if(((x+(8*t))==Tem_Data.Xpos)&&(y==Tem_Data.Ypos))
			showchar(x+(8*t),y,temp+'0',0);//����
		else
		showchar(x+(8*t),y,temp+'0',grag_value); //��ʾ����
	}
}
/*******************************************************************************
* Function Name  : shownum
* Description    : ���������ֵ ��СΪ24*32
* Input          : x,yλ�� num(����) len���� dlenС�����λ
* Output         : none
* Return         : none
*******************************************************************************/
void shownum24x32(u8 x,u8 y,u16 num,u8 len,u8 dlen,u8 grag_value)
{
	u8 t,temp,enshow=0;
	for(t=0;t<len;t++)
	{
		temp = (num/mypow(10,len - t - 1))%10;//ÿ�μ������λ
		if(enshow == 0&&t < (len-dlen-1))//��С����ǰһλ��ѯ���Ƿ�����
		{
			if(temp == 0)//������㣬����ʾ
			{
				showchar24x32(x+(24*t),y,10,grag_value);//�հ�
				continue;
			}else enshow = 1;	//�Ѿ���������
		}
		if(t==(len-dlen))
			//{showchar24x32(x+(24*t),y,11,grag_value);x+=16;}//����С�����λ����ֵ��λ�� ȷ��С�����λ��
			 {Dorp_Point24x32(x+(24*t),y,11,grag_value);x+=16;}
		showchar24x32(x+(24*t),y,temp,grag_value); //��ʾ����
	}
}
/*******************************************************************************
* Function Name  : DrawLine
* Description    : ���� ֻ�ܻ����ߺ����� ���ߵĻ� �����1��2 ��������
* Input          : void
* Output         : none
* Return         : none
********************************************************************************/
void DrawLine(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u16 t; 
	u8 kuang=1;//���߿��
	int delta_x=0,delta_y=0,temp1=0,temp2=0;  
	x1>>=2;
	x2>>=2;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	if(delta_y>delta_x)//Ҫ����
	{
		if(kuang==1)//���߿��
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
	else//������
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
* Description    : ��ʵ�ľ��Σ���С��λ������ ���� �漰���ص��ľ���(�����漰���ص���ʱ�� ����۳�һ�� Ҫע��߽�����)
* Input          : u8 mode1,u8 mode2  mode��Ϊ�˴���߽�ʱ ���ô���ʱ ���������Զ����ģʽ
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
	delta_x=x2-x1; //������������ 
	if(gray_value==0)
	{
		delta_x-=1; //������������ 
	} 	

	DrowRectL(x1,y1,y2,L,gray_value,mode1);//��߽�
	DrowRectR(x2,y1,y2,R,gray_value,mode2);//�ұ߽�
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
* Description    : �漰���߽�x����4��������ʱ Ҫ���⴦��߽�����  
* Input          : mode��Ϊ�˴���߽�ʱ ���ô���ʱ ���������Զ����ģʽ
* Output         : none
* Return         : none
********************************************************************************/
void DrowRectL(u8 x,u8 y1,u8 y2,u8 Lx,u8 gray_value,u8 mode)
{
	u8 temp1,temp2;
	if(mode==1)// ����ط� �û��Զ���ģʽ 
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
* Description    : �漰���߽�x����4��������ʱ Ҫ���⴦��߽�����  
* Input          : mode��Ϊ�˴���߽�ʱ ���ô���ʱ ���������Զ����ģʽ
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
* Description    : ϵͳ���� 
* Input          : none
* Output         : none
* Return         : none
********************************************************************************/
void GUI_Init(void)
{
	fill_screen(0);
	DrawRect(176, 1, 255, 40,Gray_valuel,0,0);//���Ͻǵ�һ��
	DrawRect(184, 5, 253, 36,0,0,0);
	showstring(180,1,"S",0X00);
	showstring(180,13,"E",0X00);
	showstring(180,25,"T",0X00);
	showstring(244,7,"V",Gray_valuel);
	showstring(244,20,"A",Gray_valuel);

  DrawRect(176, 43, 215, 83,Gray_valuel,0,0);//���е�һ��
	showstring(184,43,"P.O.D",0X00);
	DrawRect(178, 56, 213, 78,0x00,0,0); 

	DrawRect(217, 43,255, 83,Gray_valuel,0,0);//���еڶ���
	showstring(222,43,"P.O.I",0X00);
	DrawRect(218, 56, 253, 78,0x00,1,0);
	showstring(204,60,"s",Gray_valuel);
	showstring(244,60,"s",Gray_valuel);

	DrawRect(176, 86, 255, 127,Gray_valuel,0,0);//����
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
* Description    : У׼���� 
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
* Description    : ��ʾԤ��ֵ��ʵ��ֵ
* Input          : �Ҷ�ֵ
* Output         : none
* Return         : none
********************************************************************************/
void show_mess(void)
{	 
	/*������ ָʾ��ǰ����һ��*/
	if(cal_Vmin ==1) showstring(8,16,"VMin",0);
	else showstring(8,16,"VMin",0xff);
	
	if(cal_Vmax ==1) showstring(8,16*2,"VMax",0);
	else  showstring(8,16*2,"VMax",0xff);
	
	if(cal_Imin == 1)showstring(8,16*3,"IMin",0);
	else showstring(8,16*3,"IMin",0XFF);
	
	if(cal_Imax == 1) showstring(8,16*4,"IMax",0);
	else showstring(8,16*4,"IMax",0XFF);
	/*��ѯ�Ƿ������ְ������� ����� ����, û�� ���� */
	Hanle_NumKey(Str_KeyValue);	
	/*����һ�� ��ʾ��һ������ */
	switch(cal_state)
	{
		case 0x01:
		{
			showstring(128,16,String_key,0xff);//��������VMin
		}break;
		case 0x02:
		{
			shownum(128,16,Adjust_Vmin,5,3,0,0xff);//VMin������ɺ�ֱ����ʾAdjust_Vmin
			showstring(200,16,"OK",0xff);//ָʾ�˲�У׼���
			showstring(128,16*2,String_key,0xff);//��������VMax
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
				showstring(4,16*5,"Calibrate Success.",0xff);//У׼�ɹ�
				showstring(4,16*6,"Press the system key.",0xff);//�밴ϵͳ���� ����ϵͳ����
		}break;
		default:{}break;
	}
}
/*******************************************************************************
* Function Name  : Refresh_Data
* Description    : ˢ������ ������ѹ ���� ʱ���趨ֵ ʵʱ���ֵ У׼����µ�����ֵ 
* Input          : �Ҷ�ֵ
* Output         : none
* Return         : none
********************************************************************************/
void Refresh_Data(void)
{
	if(SYS_cal == 0)//��������
	{
		if(Flag_Window == 0x10)//����ǰ�� Ϊ�˽����л�ʱ ��ˢGUI ��ˢ����
		{
			GUI_Init();
			Flag_Window &= ~0x10;
		}
		shownum(204,7,Set_Volt.number,4,2,1,Gray_valuel);//��ѹ����ֵ
		shownum(204,20,Set_Curr.number,4,2,1,Gray_valuel);//��������ֵ
		shownum(180,60,Set_Tdly.number,3,0,1,Gray_valuel);//�ӳ�
		shownum(220,60,Set_Tinvl.number,3,0,1,Gray_valuel);//���
		showstring(220,102,"OK",Gray_valuel);//SCR1״̬
		showstring(220,112,"ERR",Gray_valuel);//SCR2״̬
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
//		if(Comm_Count>=100)//ͨ�Ź��Ͼ���
//		{
//			if((Comm_Count%4)<=1)
//			showstring(120,4,"A.C.M",0xff);
//			else
//				showstring(120,4,"     ",0xff);
//		}
//		else
//			showstring(120,4,"     ",0xff);
		shownum24x32(24,30,Dis_Volt,4,2,Gray_valuel);//��ѹʵʱ��ʾֵ
		shownum24x32(24,64,Dis_Curr,4,2,Gray_valuel);//����ʵʱ��ʾֵ
		if(Flag_Window&0x01) //ϵͳģʽ�� ���ּ����ø����趨ֵ ����С����
		{
			Open_Window(Str_KeyValue);  
			Flag_Window&=~0x01;		
		}
		if(Flag_Window == 0x40) //�ر�С����
		{
			Colse_Window(60,7,164,30);
			Flag_Window&=~0x40;
		}
	}
	else//У׼ģʽ��
	{
		if(Flag_Window == 0x08)//�л�У׼����
		{	
			Cal_GUI_Init();
			Flag_Window &= ~0x08;
		}
		show_mess();//У׼����ˢ��
	}
}

/*******************************************************************************
* Function Name  : show_logo
* Description    : ��ʾһ��256*128��ͼƬ
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
//		for(j=0;j<4;j++)//ÿ���ֽڿ�����ʾ�˸���
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
//		if(i<2048)//�ϰ���
//		{
//			for(j=0;j<4;j++)
//			Write_DAT0(temp[j]);
//		}
//		else//�°���
//		{
//			for(j=0;j<4;j++)
//			Write_DAT1(temp[j]);
//		}
//	}
//}
/*******************************************************************************
* Function Name  : OLED_Init
* Description    : ��ʼ��OLED
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
	
	Write_CMD0(0XFD);//׼����������
	Write_DAT0(0X12);
	Write_CMD0(0XAE);//������ ��ʾ��
	Write_CMD0(0X15);//�����д���
	Write_DAT0(0X1C);
	Write_DAT0(0X5B);
	Write_CMD0(0X75);//�����д���
	Write_DAT0(0X00);
	Write_DAT0(0X3F);	
	Write_CMD0(0XA0);//����λַ����ӡ���˫��COMģʽ
	Write_DAT0(0X04);//ѡ����ֽ���ǰ��com����ɨ�裬ˮƽģʽ
	//ˮƽ��ַ���� ��ֹ�е�ַ����ӡ�� �������ֽ�����ӳ�� 
	//com0-com[n-1] ��ֹ��ż���� ����˫��ģʽ
	Write_DAT0(0X11);//ѡ��˫��ģʽ
	Write_CMD0(0XA1);//������ʼ��
	Write_DAT0(0X00);//0	
	Write_CMD0(0XA2);//������ʾƫ����
	Write_DAT0(0X00);//�޷�֧	
	Write_CMD0(0XA6);//������ʾģʽ ������ʾ
	Write_CMD0(0XA6);//������
	Write_CMD0(0XA9);//���ò�����ʾ a8���벿����ʾ��a9�˳�
	Write_CMD0(0XAB);//����VDD��Դ
	Write_DAT0(0X01);//00 �ⲿ 01H�ڲ�
	Write_CMD0(0XB1);//������λ����
	Write_DAT0(0XE7);// P1=29 P2=7
	Write_CMD0(0XB3);//����D��Fosc
	Write_DAT0(0X91);//F=9 D=1	
	Write_CMD0(0XB4);//������ʾ������vsl��Դ
	Write_DAT0(0XA0);//A0HΪ�ⲿVsl��A2HΪ�ڲ�Vsl
	Write_DAT0(0xfd);//FDH ������� B5H���� 
	Write_CMD0(0XB5);//����GPIO
	Write_DAT0(0X00);
	Write_CMD0(0XB6);//���õڶ�Ԥ�ȳ������
	Write_DAT0(0X08);
	Write_CMD0(0XB8);//���ûҶ�ֵ
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
	Write_CMD0(0X00);//ʹ�ܻҽ�
	Write_CMD0(0XBB);//����Ԥ����ѹ
	Write_DAT0(0x1F);
	Write_CMD0(0XBE);//����Vcomh��ѹ
	Write_DAT0(0x07);
	Write_CMD0(0XC1);//���öԱȶ�
	Write_DAT0(0x7F);	////////////////////////////////////////////////////////
	Write_CMD0(0XC7);//����SEG������Χ
	Write_DAT0(0x0F);//����Χ	
	Write_CMD0(0XCA);//������ʾ��ʼ��
	Write_DAT0(0x3F);
	Write_CMD0(0XD1);//������ʾ����
	Write_DAT0(0xA2);
	Write_DAT0(0x20);
	Write_CMD0(0XAF);//����ʾ
	
	Write_CMD1(0XFD);//׼����������
	Write_DAT1(0X12);
	Write_CMD1(0XAE);//������
	Write_CMD1(0X15);//�����д���
	Write_DAT1(0X1C);
	Write_DAT1(0X5B);
	Write_CMD1(0X75);//�����д���
	Write_DAT1(0X00);
	Write_DAT1(0X3F);	
	Write_CMD1(0XA0);//���ÿ����ͼ
	Write_DAT1(0X16);//ѡ����ֽ���ǰ��com����ɨ�裬ˮƽģʽ
	Write_DAT1(0X11);//ѡ��˫��ģʽ
	Write_CMD1(0XA1);//������ʼ��
	Write_DAT1(0X00);//0	
	Write_CMD1(0XA2);//����
	Write_DAT1(0X00);//�޷�֧	
	Write_CMD1(0XA6);//������ʾģʽ
	Write_CMD1(0XA6);//������
	Write_CMD1(0XA9);//���ò�����ʾ a8���벿����ʾ��a9�˳�
	Write_CMD1(0XAB);//����VDD��Դ
	Write_DAT1(0X01);//00 �ⲿ 01H�ڲ�
	Write_CMD1(0XB1);//����P1 P2
	Write_DAT1(0XE7);// P1=29 P2=7
	Write_CMD1(0XB3);//����D��Fosc
	Write_DAT1(0X91);//F=9 D=1	
	Write_CMD1(0XB4);//������ʾ������vsl��Դ
	Write_DAT1(0XA0);//A0HΪ�ⲿVsl��A2HΪ�ڲ�Vsl
	Write_DAT1(0xfd);//FDH ������� B5H���� 
	Write_CMD1(0XB5);
	Write_DAT1(0X00);
	Write_CMD1(0XB6);
	Write_DAT1(0X08);
	Write_CMD1(0XB8);//���ûҶ�ֵ
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
	Write_CMD1(0X00);//ʹ�ܻҽ�
	Write_CMD1(0XBB);//����VP2
	Write_DAT1(0x1F);
	Write_CMD1(0XBE);//����Vcomh
	Write_DAT1(0x07);
	Write_CMD1(0XC1);//���öԱȶ�
	Write_DAT1(0x7F);	
	Write_CMD1(0XC7);//����SEG������Χ
	Write_DAT1(0x0F);//����Χ	
	Write_CMD1(0XCA);//������ʾ��ʼ��
	Write_DAT1(0x3F);
	Write_CMD1(0XD1);//������ʾ����
	Write_DAT1(0xA2);
	Write_DAT1(0x20);
	Write_CMD1(0XAF);//����ʾ
	
	fill_screen(0X00);
} 

