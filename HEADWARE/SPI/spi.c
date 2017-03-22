#include "spi.h"

void SPI1_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫�� SPI_Direction_2Lines_FullDuplex
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		 
}   
  
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;		
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			 	
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	while((SPI1->SR&0X80)==SET);//�ȴ�����ֽڷ������
	return 1;
}

void SPI2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;      //����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;//���岨����Ԥ��Ƶ��ֵ:36M/2
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����λ������LSBλ����λ����ʼ:���ݴ����MSB�ߣ���λ����ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);         //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
}
/*******************************************************************************
* Function Name  : SPI2_ReadWriteByte
* Description    : SPI д��һ���ֽڣ����ض�ȡ��һ���ֽ�.
* Input          : TxData: Ҫд����ֽ� .
* Output         : ��
* Return         : ��ȡ����һ���ֽ�
*******************************************************************************/
u16 SPI2_ReadWriteByte(u16 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) 
		{					//���ָ����SPI��־λ�������:���ͻ���ձ�־λ		
			retry++;
			if(retry>200)
				return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) 
		{				   //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
			retry++;
			if(retry>200)
				return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}
/*******************************************************************************
* Function Name  : Faceplate
* Description    : ǰ���LED�Ͱ���ֵˢ��.	��ʱ us
* Input          : *data: ָ������ .u8 *2
* Output         : ��
* Return         : ��
*******************************************************************************/
//u8 Led_Key(BIT16_DEF disp_panel,BIT32_DEF *dis_data,BIT32_DEF *key_data)
//{	  
//  KEY_LD  = 1; //�����ź� ��� 0  װ��166
//	LED_RCK = 0; //���0   
//  SPI2_ReadWriteByte(disp_panel.halfword);  //�������ݵ�595	
//	LED_RCK = 1;//���1 
//	KEY_LD = 0; //�����ź� ��� 1    װ��166���
//	RCK_LED_KEY = 1;//������0 ���Ƽ��̵�
//	key_data->half.high = ~SPI2_ReadWriteByte(dis_data->half.high); //�������ݵ�595��ȡ166����
//	key_data->half.low = ~SPI2_ReadWriteByte(dis_data->half.low); //�������ݵ�595��ȡ166����	
//	RCK_LED_KEY = 0;//������1 ���Ƽ��̵�
//  return 1;  //����SPI����
//}



































