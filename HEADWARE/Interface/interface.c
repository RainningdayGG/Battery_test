#include "interface.h"
char *Window_Value = "Volt:";
char *Window_Curr = "Curr:";
char *Window_Dly = "Dely:";
char *Window_Invl = "Inal:";
char String_key[7] = {' ',' ',' ',' ',' ','\0','\0'};
u8 Str_Count=0;
u8 Max_StrCnt=6;
/***************************************************************
*������ ��fwd                                        
*�β�   ��void                 
*����ֵ ��none                                             
*����   ������λ����ǰ��һλ                                        
****************************************************************/
void fwd(void)
{
	if((Tem_Data.Xpos==Set_Volt.Xpos)||(Tem_Data.Xpos==(Set_Volt.Xpos+20)))
	{Tem_Data.Xpos+=8;}
	else if (Tem_Data.Xpos==(Set_Volt.Xpos+8))
	{
		Tem_Data.Xpos+=12;
	}
	else if((Tem_Data.Xpos==Set_Tdly.Xpos)||(Tem_Data.Xpos==Set_Tdly.Xpos+8)||(Tem_Data.Xpos==Set_Tinvl.Xpos)||(Tem_Data.Xpos==Set_Tinvl.Xpos+8))
	{
		Tem_Data.Xpos+=8;
	}
	else
	{
		Tem_Data.Xpos=Tem_Data.Xpos;
	}
}
/***************************************************************
*������ ��bwd                                        
*�β�   ��void                 
*����ֵ ��none                                             
*����   ������λ����ǰ��һλ                                           
****************************************************************/
void bwd(void)
{
	if((Tem_Data.Xpos==Set_Volt.Xpos+8)||(Tem_Data.Xpos==(Set_Volt.Xpos+28)))
	{Tem_Data.Xpos-=8;}
	else if (Tem_Data.Xpos==(Set_Volt.Xpos+20))
	{
		Tem_Data.Xpos-=12;
	}
	else if((Tem_Data.Xpos==Set_Tdly.Xpos+16)||(Tem_Data.Xpos==Set_Tdly.Xpos+8)||(Tem_Data.Xpos==Set_Tinvl.Xpos+16)||(Tem_Data.Xpos==Set_Tinvl.Xpos+8))
	{
		Tem_Data.Xpos-=8;
	}
	else
	{
		Tem_Data.Xpos=Tem_Data.Xpos;
	}
}
/***************************************************************
*������ ��add                                      
*�β�   ��void                 
*����ֵ ��none                                             
*����   ������λ������+1                                          
****************************************************************/
void add(void)
{
	if(Tem_Data.Ypos==(Set_Volt.Ypos))
	{
		if(Tem_Data.Xpos==Set_Volt.Xpos)
		{	
				if((Set_Volt.number+1000)<=3000)
				Set_Volt.number+=1000;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+8))
		{
			if((Set_Volt.number+100)<=3000)
				Set_Volt.number+=100;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+20))
		{		
			if((Set_Volt.number+10)<=3000)			
				Set_Volt.number+=10;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+28))
		{
			if(Set_Volt.number<3000)	
				Set_Volt.number+=1;
		}
	}
	else if(Tem_Data.Ypos==(Set_Curr.Ypos))
	{
		if(Tem_Data.Xpos==Set_Curr.Xpos)
		{	
				if((Set_Curr.number+1000)<=2000)
				Set_Curr.number+=1000;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+8))
		{
			if((Set_Curr.number+100)<=2000)
				Set_Curr.number+=100;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+20))
		{		
			if((Set_Curr.number+10)<=200)			
				Set_Curr.number+=10;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+28))
		{
			if(Set_Curr.number<2000)	
				Set_Curr.number+=1;
		}
	}
	else if(Tem_Data.Ypos==(Set_Tdly.Ypos))
	{
		if(Tem_Data.Xpos==Set_Tdly.Xpos)
		{	
				if((Set_Tdly.number+100)<=600)
				Set_Tdly.number+=100;
		}
		else if(Tem_Data.Xpos==(Set_Tdly.Xpos+8))
		{
			if((Set_Tdly.number+10)<=600)
				Set_Tdly.number+=10;
		}
		else if(Tem_Data.Xpos==(Set_Tdly.Xpos+16))
		{		
			if(Set_Tdly.number<600)			
				Set_Tdly.number+=1;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos))
		{
			if((Set_Tinvl.number+100)<=600)	
				Set_Tinvl.number+=100;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos+8))
		{
			if((Set_Tinvl.number+10)<=600)	
				Set_Tinvl.number+=10;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos+16))
		{
			if(Set_Tinvl.number<600)	
				Set_Tinvl.number+=1;
		}
	}
}
/***************************************************************
*������ ��void minus(void)                                     
*�β�   ��void                 
*����ֵ ��none                                             
*����   ������λ������-1                                          
****************************************************************/
void minus(void)
{
	if(Tem_Data.Ypos==(Set_Volt.Ypos))
	{
		if(Tem_Data.Xpos==Set_Volt.Xpos)
		{	
				if((Set_Volt.number>=1000))
				Set_Volt.number-=1000;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+8))
		{
			if(Set_Volt.number>=100)
				Set_Volt.number-=100;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+20))
		{		
			if(Set_Volt.number>=10)			
				Set_Volt.number-=10;
		}
		else if(Tem_Data.Xpos==(Set_Volt.Xpos+28))
		{
			if(Set_Volt.number>=1)	
				Set_Volt.number-=1;
		}
	}
	else if(Tem_Data.Ypos==(Set_Curr.Ypos))
	{
		if(Tem_Data.Xpos==Set_Curr.Xpos)
		{	
				if((Set_Curr.number>=1000))
				Set_Curr.number-=1000;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+8))
		{
			if(Set_Curr.number>=100)
				Set_Curr.number-=100;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+20))
		{		
			if(Set_Curr.number>=10)			
				Set_Curr.number-=10;
		}
		else if(Tem_Data.Xpos==(Set_Curr.Xpos+28))
		{
			if(Set_Curr.number>=1)	
				Set_Curr.number-=1;
		}
	}
	else if(Tem_Data.Ypos==(Set_Tdly.Ypos))
	{
		if(Tem_Data.Xpos==Set_Tdly.Xpos)
		{	
				if((Set_Tdly.number>=100))
				Set_Tdly.number-=100;
		}
		else if(Tem_Data.Xpos==(Set_Tdly.Xpos+8))
		{
			if(Set_Tdly.number>=10)
				Set_Tdly.number-=10;
		}
		else if(Tem_Data.Xpos==(Set_Tdly.Xpos+16))
		{		
			if(Set_Tdly.number>=1)			
				Set_Tdly.number-=1;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos))
		{
			if(Set_Tinvl.number>=101)	
				Set_Tinvl.number-=100;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos+8))
		{
			if(Set_Tinvl.number>=11)	
				Set_Tinvl.number-=10;
		}
		else if(Tem_Data.Xpos==(Set_Tinvl.Xpos+16))
		{
			if(Set_Tinvl.number>=2)	
				Set_Tinvl.number-=1;
		}
	}
}
/*******************************************************************************
* Function Name  : Hanle_NumKey
* Description    : �����������йصİ���
* Input          : ���ּ� �˸� �����
* Output         : none
* Return         : none
*******************************************************************************/
void clear_str(void)
{
	u8 i = 0;
	Str_Count = 0;
	for(i=0;i<(Max_StrCnt-1);i++)
	{
		String_key[i] = ' ';
	}
}
/*******************************************************************************
* Function Name  : Hanle_NumKey
* Description    : �����������йصİ���
* Input          : ���ּ� �˸� �����
* Output         : none
* Return         : none
*******************************************************************************/
void Hanle_NumKey(char numkey)
{
	if(Flag_Window==0x03)//����String_key�İ���
	{
		if(numkey == 0x08)//�˸��
		{
			if(Str_Count>0)
			{
				Str_Count--;
				String_key[Str_Count]=' ';
			}
			else return;
		}
		else 	if((Str_Count+1)==Max_StrCnt)//���������5���ַ��󣬰�ÿ���ַ�д�ɿո�
		{
			for(Str_Count=(Max_StrCnt-1);Str_Count > 0;Str_Count--)
			String_key[Str_Count-1] = ' ';
		}
		else if(numkey == 0x7f)//�����
		{
			clear_str();
		}
		else
		{
			if((Str_Count==0)&&(numkey=='.'))//��һ������'.'�Ļ���'.'->'0.'
			{
				String_key[0] = '0';
				String_key[1] = '.';
				Str_Count+=2;
			}
			else
			{
				String_key[Str_Count]=numkey;
				Str_Count++;
			}
		}
		Flag_Window&=~0x03;
	}
}

/*******************************************************************************
* Function Name  : Open_Window
* Description    :������ʾ����ֵ
* Input          : ���ּ� �˸� �����
* Output         : none
* Return         : none
*******************************************************************************/
void Open_Window(char KeyValue)
{	
	/*���߿�*/
	DrawLine(80, 7, 80, 27);
	DrawLine(164, 7, 164, 27);
	DrawLine(80, 7, 160, 7);
	DrawLine(80, 27, 160, 27);
	/*��ѯ��ǰͣ������һ����*/
	if(Tem_Data.Ypos==Set_Volt.Ypos)
	{
		showstring(84,10,Window_Value,0xff);//show "Volt:"
		Hanle_NumKey(KeyValue);	//���ְ�������  String_key[]�Ĵ���
		showstring(124,10,String_key,0xff);//show String_key[]
	}
	else if(Tem_Data.Ypos==Set_Curr.Ypos)
	{
		showstring(84,10,Window_Curr,0xff);	
		Hanle_NumKey(KeyValue);	
		showstring(124,10,String_key,0xff);
	}
	else if(Tem_Data.Xpos==Set_Tdly.Xpos)
	{
		showstring(84,10,Window_Dly,0xff);
		Hanle_NumKey(KeyValue);	
		showstring(124,10,String_key,0xff);
	}
	else if(Tem_Data.Xpos==Set_Tinvl.Xpos)
	{
		showstring(84,10,Window_Invl,0xff);
		Hanle_NumKey(KeyValue);	
		showstring(124,10,String_key,0xff);
	}
}
/*******************************************************************************
* Function Name  : Colse_Window
* Description    : �������
* Input          : �߽�
* Output         : none
* Return         : none
*******************************************************************************/
void Colse_Window(u8 x1, u8 y1, u8 x2, u8 y2)
{
	u8 i=0;
	u16 t,y=y1;
	int delta_x;  
	x1>>=2;
	x2>>=2;
	delta_x=x2-x1; //������������ 
	for(y=y;y<=y2;y++)
	{
		for(t=x1+1;t<=delta_x+x1;t++)
		{
			Set_Window(y,y,t,t); 
			Write_CMD0(0X5C);
			Write_CMD1(0X5C);
			if(y>=HD/2)
			{
				Write_DAT1(0);
				Write_DAT1(0);
			}
			else
			{
				Write_DAT0(0);
				Write_DAT0(0);
			}	
		}
	}	
	Flag_Window = 0;
	Str_Count = 0;
	for(i=0;i<(Max_StrCnt-1);i++)
		{
			String_key[i] = ' ';
		}
}
