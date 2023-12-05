#include "SPI.h"
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI2_Init(void)
{//FLASH_CS pb12
	//SPI_MISO pb14
	//SPI_SCK pb13
	//SPI_MOSI pb15
	
	/* SPI��IO�ں�SPI�����ʱ�� */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* SPI��IO������ */
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;  //CSƬѡ
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15 ;  //PB13-SPI1_SCK  PB15-SPI1_MOSI
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;  //fuyong�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //�������� MISO
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI_InitTypeDef  SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	//SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	
	//SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
	//SPI_NSSInternalSoftwareConfig(SPI2, SPI_NSSInternalSoft_Set);//Ϊѡ���� SPI ��������ڲ� NSS �ܽ�
	
	//SPI_DataSizeConfig (SPI2,SPI_DataSize_8b);//����ѡ���� SPI ���ݴ�С
	//SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_TXE|SPI_I2S_IT_RXNE,ENABLE);
	
//	//�ж����ȼ�
//	NVIC_InitTypeDef NVIC_InitStruct={0};
//	NVIC_InitStruct.NVIC_IRQChannel=SPI1_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
//	NVIC_Init(& NVIC_InitStruct);
// 
 
	//SPI1_ReadWriteByte(0xff);//��������	
}


uint8_t spiflag=0;
void SPI2_IRQHandler(void)//�жϺ���
{
	uint8_t data=0;
	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_RXNE)==1)//���ܻ���ǿ��жϱ�־λ
	{
		data=SPI_I2S_ReceiveData(SPI2);
	}
	
	if(SPI_I2S_GetITStatus(SPI2,SPI_I2S_IT_TXE)==1)//���ͻ�����жϱ�־λ
	{
		spiflag=1;
		SPI_I2S_ClearFlag(SPI2,SPI_I2S_IT_TXE);//������б�־λ
		SPI_I2S_SendData(SPI2,0x1234);
		
	}
}