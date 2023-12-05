//����4 pc10TX pc11
#include "su03t.h"
#include "led.h"
#include "stdio.h"
#include "string.h"
#include "beep.h"
#include "dht11.h"
extern DHT11_H11 dht11;
//����1���� TX_PA9�����������   RX_PA10�������� APB2
void usart_confg4(void)
{
	//��gpioʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//����������� 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�������� 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//���ڿ�ʱ��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 9600; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //��λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��������żУ��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//��ʹ��Ӳ�������� 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //ȫ˫��
	USART_Init(UART4,&USART_InitStructure);//��ʼ��������
	//ʹ�ܴ��ڽ��� �ж�
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);//�����ж�
	//�ж����ȼ�
	/*
	ʹ�÷��������ʷ����� ���� 12λ������4λС���ı�ʾ������
�� һ��״̬�Ĵ���(USART_SR)
�� ���ݼĴ���(USART_DR)
�� һ�������ʼĴ���(USART_BRR)��12λ��������4λС��
�� һ�����ܿ�ģʽ�µı���ʱ��Ĵ���(USART_GTPR)
	*/
		//�ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=UART4_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
	
	USART_Cmd(UART4,ENABLE);//����ʹ��
	

}
//���͵����ֽ�(����)
void usart_sendbyte4(uint8_t date)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0)//��һ���������
 {}
	 USART_SendData(UART4,date);
}


void usart_sendstr4(uint8_t str[],uint8_t len)
{
	for(uint8_t i=0;i<len;i++)
	{
	 usart_sendbyte4(str[i]);
	}
}
////�������ݣ����죩
//uint8_t recive_data(void)
//{
//	uint8_t data=0;
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==0)
//	{}
//		data=USART_ReceiveData(USART1);
//		return data;
//}

uint8_t rv4[10]={0};
uint8_t rvcnt4=0;
uint8_t rvflag4=0;//������ɱ�־λ
uint8_t se4[10]={0};//����
void UART4_IRQHandler(void)//����1�жϺ���
{
	uint8_t data=0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==1)//���ָ���� USART �жϷ������//���USART_CR1�Ĵ����е�RXNEIEΪ1��������жϡ���USART_DR�Ķ��������Խ���λ���㡣RXNEλҲ����ͨ��д��0�������ֻ���ڶ໺��ͨѶ�в��Ƽ������������0������û���յ��� 1���յ����ݣ����Զ�����
	{
		//data=USART_ReceiveData(USART1);
		rv4[rvcnt4++]=USART_ReceiveData(UART4);
	}
	if(USART_GetITStatus(UART4,USART_IT_IDLE)!=0)//
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		rvflag4=1;
		UART4->SR;		
		UART4->DR;//������б�־λ
	}
//	if(data=='0')
//		{
//		ledr_turn(ledrport,ledrpin);
//		}
//		if(data=='1')
//		{
//		ledb_turn(ledbport,ledbpin);
//		}
}
//�����������麯��
extern uint16_t rv1[10];
float ch2o=0;
uint8_t an4(void)
{
	get_tem_hum();
	if(rvflag4==1)
	{
		if(rvcnt4==6)
		{
			if((rv4[0]==0xAA)&&(rv4[1]==0x55)&&(rv4[2]==0x00)&&(rv4[4]==0x55)&&(rv4[5]==0xAA))
			{
				switch(rv4[3])
				{
					case 1: //��ȡ�¶�
					se4[0]=	0xAA;
					se4[1]=	0x55;
					se4[2]=	0x01;
					se4[3]=	dht11.tem;//30��
					se4[4]=	0x55;
					se4[5]=	0xAA;
					usart_sendstr4(se4,6);	
					break;
				  case 2:
					se4[0]=	0xAA;
					se4[1]=	0x55;
					se4[2]=	0x02;
					se4[3]=	dht11.hum;//30��
					se4[4]=	0x55;
					se4[5]=	0xAA;
					usart_sendstr4(se4,6);	
					break;	
					case 3:
					se4[0]=	0xAA;
					se4[1]=	0x55;
//					if(ch2o>0.1)
//					{
//					se4[2]=	0x03;
//					}
//					else if((0.1>=ch2o)&&(ch2o>=0.01))
//					{
//					se4[2]=	0x04;
//					}
//					else if(ch2o<0.01)
//					{
//					se4[2]=	0x05;
//					}
					se4[2]=	0x03;
					se4[3]=((rv1[3]<<8|rv1[4]))*0.01;
					se4[4]=	0x55;
					se4[5]=	0xAA;
					usart_sendstr4(se4,6);	
					break;
					case 4:
						led_on(led2port,led2pin);
					break;
					case 5:
						led_off(led2port,led2pin);
					break;
					case 7:
						beep_on(beepport,beeppin);
					break;
					case 8:
						beep_off(beepport,beeppin);
					break;
				}
				printf("������\r\n");
				rvcnt4=0;
				memset(rv4,0,10);	
				memset(se4,0,10);	
			}
			else
			{
			rvcnt4=0;
			memset(rv4,0,10);
				printf("У�鲻��\r\n");
			}
		}
		else
		{
		rvcnt4=0;
			memset(rv4,0,10);
			printf("���ݲ��ȳ��Ȳ���6\r\n");
		}
	}
	rvflag4=0;
}

//int fputc(int ch, FILE *f)
//{

//  /* Loop until the end of transmission */
//  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//  {}
//USART_SendData(USART1, (uint8_t) ch);
//  return ch;
//}