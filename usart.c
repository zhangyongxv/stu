#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "string.h"

//����1���� TX_PA9�����������   RX_PA10�������� APB2
void usart_confg(void)
{
	//��gpioʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//����������� 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�������� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		//���ڿ�ʱ��
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 115200; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //��λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��������żУ��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//��ʹ��Ӳ�������� 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //ȫ˫��
	USART_Init(USART1,&USART_InitStructure);//��ʼ��������
	USART_Cmd(USART1,ENABLE);//����ʹ��
	//ʹ�ܴ��ڽ��� �ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//�����ж�
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
 NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);

}
//���͵����ֽ�(����)
void usart_sendbyte(uint8_t date)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0)//��һ���������
 {}
	 USART_SendData(USART1,date);
}

void usart_sendstr(uint8_t str[],uint8_t len)
{
	for(uint8_t i=0;i<len;i++)
	{
	 usart_sendbyte(str[i]);
	}
}

//�������ݣ����죩
uint8_t recive_data(void)
{
	uint8_t data=0;
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==0)
	{}
		data=USART_ReceiveData(USART1);
		return data;
}

uint8_t rv[10]={0};
uint8_t rvcnt=0;
uint8_t rvflag=0;//������ɱ�־λ
void USART1_IRQHandler(void)//����1�жϺ���
{
	uint8_t data=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==1)//�������ݼĴ����ǿձ�־λ//���ָ���� USART �жϷ������//���USART_CR1�Ĵ����е�RXNEIEΪ1��������жϡ���USART_DR�Ķ��������Խ���λ���㡣RXNEλҲ����ͨ��д��0�������ֻ���ڶ໺��ͨѶ�в��Ƽ������������0������û���յ��� 1���յ����ݣ����Զ�����
	{
		data=USART_ReceiveData(USART1);
		USART_SendData(USART3,data);
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
	}
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=0)//�������߱�־λ
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		rvflag=1;
		USART1->SR;		
		USART1->DR;//������б�־λ
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
uint8_t an(void)
{
	if(rvflag==1)
	{
		if(rvcnt==5)
		{
			if((rv[0]==0xAA)&&(rv[1]==0x55)&&(rv[3]==0x55)&&(rv[4]==0xAA))
			{
				switch(rv[2])
				{
					case 0: ledr_turn(ledrport,ledrpin);break;
				  case 1:;break;
					case 2:;break;
					case 3:;break;
				}
				printf("������\r\n");
						rvcnt=0;
			memset(rv,0,10);	
			}
			else
			{
			rvcnt=0;
			memset(rv,0,10);
				printf("������Э��\r\n");
			}
		}
		else
		{
		rvcnt=0;
			memset(rv,0,10);
			printf("���ݲ��ȳ��Ȳ���5\r\n");
		}
	}
	rvflag=0;
}

int fputc(int ch, FILE *f)
{
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)//������ɱ�־λ
  {}
	USART_SendData(USART1, (uint8_t) ch);
  return ch;
}