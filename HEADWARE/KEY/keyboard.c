#define KEY_GLOBALS
#include "keyboard.h"	 		
#include "stdlib.h"
u8 key;    //ǰ��尴��ֵ
char Str_KeyValue=0;
BIT16_DEF led_panel = {0};
BIT32_DEF led_key   = {0};
BIT32_DEF key_buff  = {0};

/*******************************************************************************
* Function Name  : Faceplate
* Description    : ǰ���LED�Ͱ���ֵˢ��.	��ʱ us
* Input          : *data: ָ������ .u8 *2
* Output         : ��
* Return         : ��
*******************************************************************************/
u8 Led_Key(BIT16_DEF disp_panel,BIT32_DEF *dis_data,BIT32_DEF *key_data)
{	  
  KEY_LD  = 1; //�����ź� ��� 0  װ��166	
	LED_RCK = 0; //���0   
  SPI2_ReadWriteByte(disp_panel.halfword);  //�������ݵ�595	
	LED_RCK = 1;//���1 
	KEY_LD = 0; //�����ź� ��� 1    װ��166���
	
	RCK_LED_KEY = 1;//������0 ���Ƽ��̵�
	key_data->half.high = ~SPI2_ReadWriteByte(dis_data->half.high); //�������ݵ�595��ȡ166����
	key_data->half.low = ~SPI2_ReadWriteByte(dis_data->half.low); //�������ݵ�595��ȡ166����	
	RCK_LED_KEY = 0;//������1 ���Ƽ��̵�
  
	return 1;  //����SPI����
}
/*******************************************************************************
* Function Name  : KEY_Scan
* Description    : �жϰ���ֵ���˺������ü���������ke���µ�ʱ����10ms��Ϊ����
* Input          : ke
* Output         : none
* Return         : 0-8 ;0��ʾ�ް������£�1-8��ʾ�˸�����
*******************************************************************************/
void KEY_Scan(u32 ke)
{	  
  static u8 table = 1;
  static u32 temp  = 0;
  static u8 key_modle = 1;      //=1��ֹ����������Ӧ =0����

  if(table==1)         //��һ��ʱ��Ƭ
  {
    if(key_modle==1)
     {
       temp  = ke;
       table = 2;          //��һ������ڶ���ʱ��Ƭ
     }
     else if(key_modle==0)
     {
        table = 2;          //��һ������ڶ���ʱ��Ƭ
     }
     else 
     {
        key_modle=1;
        temp  = 0;
        table = 1;          //��һ��ʱ��Ƭ
     }   
  }
  else if(table==2)   //�ڶ���ʱ��Ƭ
  { 
    table = 1;
    if((temp==ke)&&(key_modle))
    {       
      switch (ke)
      {
        case 0x00000000://������
        {  
          key_modle = 1;
          key = 0;
        }break;
        case 0x00000001://����1
        {  
          key_modle = 0;
          key = 1;
        }break;				
        case 0x00000002://����2
        { 
          key_modle = 0; 
          key = 2;
        }break;
        case 0x00000004://����3
        {  
          key_modle = 0; 
          key = 3;
        }break;
        case 0x00000008://����4
        {  
          key_modle = 0;
          key = 4;
        }break;
        case 0x00000010://����5
        {  
          key_modle = 0;
          key = 5;
        }break;
        case 0x00000020://����6
        { 
          key_modle = 0;
          key = 6;
        }break; 
        case 0x00000040://����7
        {  
          key_modle = 0;
          key = 7;
        }break;
        case 0x00000080://����8
        { 
          key_modle = 0;
          key = 8;
        }break;
        case 0x00000100://����9
        {  
          key_modle = 0;
          key = 9;
        }break;
        case 0x00000200://����10
        { 
          key_modle = 0; 
          key = 10;
        }break;
        case 0x00000400://����11
        {  
          key_modle = 0; 
          key = 11;
        }break;
        case 0x00000800://����12
        {  
          key_modle = 0;
          key = 12;
        }break;
        case 0x00001000://����13
        {  
          key_modle = 0;
          key = 13;
        }break;
        case 0x00002000:
        { 
          key_modle = 0;
          key = 14;
        }break; 
        case 0x00004000:
        {  
          key_modle = 0;
          key = 15;
        }break;
        case 0x00008000:
        { 
          key_modle = 0;
          key = 16; 
				}break;	
        case 0x00010000:
        {  
          key_modle = 0;
          key = 17;
        }break;
        case 0x00020000:
        { 
          key_modle = 0; 
          key = 18;
        }break;
        case 0x00040000:
        {  
          key_modle = 0; 
          key = 19;
        }break;
        case 0x00080000:
        {  
          key_modle = 0;
          key = 20;
        }break;
        case 0x00100000:
        {  
          key_modle = 0;
          key = 21;
        }break;
        case 0x00200000:
        { 
          key_modle = 0;
          key = 22;
        }break; 
        case 0x00400000:
        {  
          key_modle = 0;
          key = 23;
        }break;
        case 0x00800000:
        { 
          key_modle = 0;
          key = 24;
        }break;
        case 0x01000000:
        {  
          key_modle = 0;
          key = 25;
        }break;
        case 0x02000000:
        { 
          key_modle = 0; 
          key = 26;
        }break;
        case 0x04000000:
        {  
          key_modle = 0; 
          key = 27;
        }break;
        case 0x08000000:
        {  
          key_modle = 0;
          key = 28;
        }break;
        case 0x10000000:
        {  
          key_modle = 0;
          key = 29;
        }break;
        case 0x20000000:
        { 
          key_modle = 0;
          key = 30;
        }break; 
        case 0x40000000:
        {  
          key_modle = 0;
          key = 31;
        }break;
        case 0x80000000:
        { 
          key_modle = 0;
          key = 32; 
				}break;	
				
        default:
        {  
          key_modle = 0;
        }break;
          
      }    
    }
    else if(temp!=ke)
    {
      if(ke==0x0)
			{
				key_modle = 1;
			}			
    }
  }
  else  
  {   key_modle = 1;
      table = 1;//���¿�ʼ��һƬ
  } 
}
/*******************************************************************************
* Function Name  : key_exe();
* Description    : ���ݰ���ֵ������Ӧ�Ĳ���.20msִ��һ�����
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void key_exe(void)
{
  switch (key)
  {
    case 1:// ��
    {   
			key = 0;
			add();
			LED_KEY1 = ~LED_KEY1;
    }break;
    case 2://��
    {
      key = 0;
			minus();
			LED_KEY2 = ~LED_KEY2;
    }break;
    case 3://У׼
    {
      key = 0;	
			SYS_cal=1;//��λcal��־λ
			Max_StrCnt = 7;//������6���ַ� example:12.345
			clear_str();
			cal_state = 0;
			Flag_Window = 0x08;
			LED_KEY3 = ~LED_KEY3;
     }break;
    case 4://ϵͳ
    {
      key = 0;
			SYS_cal=0;//���cal��־λ
			Max_StrCnt = 6;//������5���ַ� example:12.34
			clear_str();
			String_key[5] ='\0';
			Flag_Window = 0x10;
			LED_KEY4 = ~LED_KEY4;
    }break;
    case 5://��
    {
      key = 0;
			bwd();
			LED_KEY5 = ~LED_KEY5;
    }break;
    case 6://��
    {
      key = 0;
			fwd();
			LED_KEY6 = ~LED_KEY6;
    }break;
    case 7://shift
    {
      key = 0;
			LED_KEY7 = ~LED_KEY7;
    }break;
    case 8://�˳�
    {
      key = 0;
			LED_KEY8 = ~LED_KEY8;
    }break;		
    case 9://volt
    {   
			key = 0;
			if((Flag_Window==0)&&!SYS_cal)//�ڿ���ʱ ������ֵ�� �����л���ѹ ���� 
			StructCopy(Tem_Data,Set_Volt);
			LED_KEY9 = ~LED_KEY9;
    }break;
    case 10://curr
    {
      key = 0;
			if((Flag_Window==0)&&!SYS_cal)
			StructCopy(Tem_Data,Set_Curr);
			LED_KEY10 = ~LED_KEY10;
    }break;
    case 11://delay
    {
      key = 0;
			if((Flag_Window==0)&&!SYS_cal)	
			StructCopy(Tem_Data,Set_Tdly);
			LED_KEY11 = ~LED_KEY11;			
     }break;
    case 12://���
    {
      key = 0;
			if((Flag_Window==0)&&!SYS_cal)
			StructCopy(Tem_Data,Set_Tinvl);
			LED_KEY12 = ~LED_KEY12;
    }break;
    case 13://7
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue='7';
			LED_KEY13 = ~LED_KEY13;
    }break;
    case 14://4
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue='4';
			LED_KEY14 = ~LED_KEY14;
    }break;
    case 15://1
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue='1';
			LED_KEY15 = ~LED_KEY15;
    }break;
    case 16://0
    {
      key = 0;			
			Flag_Window=3;
			Key_value=0;
			Str_KeyValue='0';
			LED_KEY16 = ~LED_KEY16;
    }break;			
    case 17://8
    {
      key = 0;			
			Flag_Window=3;
			Str_KeyValue='8';
			LED_KEY17 = ~LED_KEY17;
    }break;
    case 18://5
    {
      key = 0;			
			Flag_Window=3;
			Str_KeyValue='5';
			LED_KEY18 = ~LED_KEY18;
    }break;		
    case 19://2
    {   
			key = 0;			
			Flag_Window=3;
			Str_KeyValue='2';
			LED_KEY19 = ~LED_KEY19;
    }break;
    case 20://.
    {
      key = 0;		
			Flag_Window=3;
			LED_KEY20 = ~LED_KEY20;
			Str_KeyValue='.';
    }break;
    case 21://9
    {
      key = 0;		
			Flag_Window=3;
			Str_KeyValue='9';
			LED_KEY21 = ~LED_KEY21;			
     }break;
    case 22://6
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue='6';
			LED_KEY22 = ~LED_KEY22;	
    }break;
    case 23://3
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue='3';
			LED_KEY23 = ~LED_KEY23;
    }break;
    case 24://backspace
    {
      key = 0;
			Flag_Window=5;
			LED_KEY24 = ~LED_KEY24;
    }break;
    case 25://����
    {
      key = 0;
			if(SYS_timerunning == 0)//���SYS_timerunning �������run �Ͳ�������
			{
				ShadowTdly = Set_Tdly.number;
				ShadowTinvl = Set_Tinvl.number;
				SYS_single = 0;
				SYS_timerunning = 1;//��λSYS_timerunning
				OSTaskResume(6);//�ָ�Contrl_Time_Task����
				LED_KEY25 = ~LED_KEY25;
			}
    }break;
    case 26://�ر�
    {
      key = 0;
			SYS_output = 0; 
			LED_KEY26 = ~LED_KEY26;
    }break;			
		case 27://���
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue=0x7f;
			LED_KEY27 = ~LED_KEY27;
    }break;
    case 28://�˸�
    {
      key = 0;
			Flag_Window=3;
			Str_KeyValue=0x08;
			LED_KEY28 = ~LED_KEY28;
    }break;		
    case 29://����
    {   
			key = 0;
			if(SYS_timerunning == 0)//���SYS_timerunning �������run �Ͳ�������
			{
				ShadowTdly = Set_Tdly.number;
				SYS_single = 1;
				SYS_timerunning = 1;
				OSTaskResume(6);//�ָ�Contrl_Time_Task����
				LED_KEY29 = ~LED_KEY29;
			}
    }break;
    case 30://����
    {
      key = 0;
			SYS_output = 1;
			LED_KEY30 = ~LED_KEY30;
    }break;
    case 31://ȡ��
    {
      key = 0;
			Flag_Window = 0x40;
			LED_KEY31 = ~LED_KEY31;			
     }break;
    case 32://ȷ��
    {
      key = 0;
			key_enter();
			LED_KEY32 = ~LED_KEY32;
    }break;
    default:
    {
      //  key = 0;
    } break;      
  }
}

//��У׼ģʽ�� ����Ҫ���õĵ�ѹ ���� ��ʱ ���
void NormalSetNum(float Para)
{
	if(Tem_Data.Ypos==Set_Volt.Ypos)//���õ�ѹ
	{
		if(Para>=0&&Para<=36)
		{
			Para=Para*100;
			Set_Volt.number=(u16)Para;
		}
		else 
			{
				showstring(124,10,"error",0xff);
				delay_ms(800);
				showstring(124,10,"     ",0xff);
			}
	}
	else if(Tem_Data.Ypos==Set_Curr.Ypos)//���õ���
	{
		if(Para>=0&&Para<=16)
		{
			Para=Para*100;
			Set_Curr.number=(int)Para;
		}
		else
		{
			showstring(124,10,"error",0xff);
			delay_ms(800);
			showstring(124,10,"     ",0xff);
		}
	}
	else if(Tem_Data.Xpos==Set_Tdly.Xpos)//����delay
	{		
		if(Para>=0&&Para<=600)
		{
			Set_Tdly.number=(int)Para;
		}
		else
		{
			showstring(124,10,"error",0xff);
			delay_ms(800);
			showstring(124,10,"     ",0xff);
		}
	}
	else if(Tem_Data.Xpos==Set_Tinvl.Xpos)//���ü��
	{
		if(Para>=0&&Para<=600)
		{
			Set_Tinvl.number=(int)Para;
		}
		else
		{
			showstring(124,10,"error",0xff);
			delay_ms(800);
			showstring(124,10,"     ",0xff);
		}
	}
}
//У׼ģʽ�� ����Ҫʵ�ʵĵ�ѹ ���� P1 P2
void CalSetNum(float Para)
{
 if(cal_Vmin == 1)//���õ�ѹ�͵�
	{
		if(Para>=0&&Para<=10)
		{
			Para=Para*1000;
			Adjust_Vmin=(int)Para;
			cal_state++;
		}
		else
		{
			showstring(128,16*1,"error",0xff);
			delay_ms(800);
			showstring(128,16*1,"     ",0xff);
		}
	}
	else if(cal_Vmax == 1)//���õ�ѹ�ߵ�
	{
		if(Para>=0&&Para<=36)
		{
			Para=Para*1000;
			Adjust_Vmax =(u16)Para;
			cal_state++;
		}
		else 
			{
				showstring(128,16*2,"error",0xff);
				delay_ms(800);
				showstring(128,16*2,"     ",0xff);
			}
	}
	else if(cal_Imin == 1)//���õ����͵�
	{
		if(Para>=0&&Para<=10)
		{
			Para=Para*1000;
			Adjust_Imin=(int)Para;
			cal_state++;
		}
		else
		{
			showstring(128,16*3,"error",0xff);
			delay_ms(800);
			showstring(128,16*3,"     ",0xff);
		}
	}
	else if(cal_Imax == 1)//���õ����ߵ�
	{		
		if(Para>=0&&Para<=10)
		{
			Para=Para*1000;
			Adjust_Imax=(int)Para;
			cal_state++;//5
		}
		else
		{
			showstring(128,16*4,"error",0xff);
			delay_ms(800);
			showstring(128,16*4,"     ",0xff);
		}
	}
}
void key_enter(void)
{
	float ftemp=0.0;
	if(Str_Count == 0)
		return;
	else
	{
		ftemp=atof(String_key);//���ֻ���װ��Ϊ������
		clear_str();
		if(SYS_cal==0)//��У׼
		{
			NormalSetNum(ftemp);
			Flag_Window = 0x40;
		}
		else//У׼
		{
			CalSetNum(ftemp);
			showstring(128,16*(cal_state-1),"      ",0xff);	
		}
	}
}

