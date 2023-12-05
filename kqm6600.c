#include "kqm6600.h"
#include "stdio.h"
#include "string.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySem_Handle;//��ֵ�ź������
//USART2_RX  PA3  APB1
void usart2_confg(void)
{
	//��gpioʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//�������� 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//���ڿ�ʱ��
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 9600; //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //��λ���ݳ���
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��������żУ��
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//��ʹ��Ӳ�������� 
	USART_InitStructure.USART_Mode = USART_Mode_Rx;  //ȫ˫��
	USART_Init(USART2,&USART_InitStructure);//��ʼ������
	//ʹ�ܴ��ڽ��� �ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//�����ж�
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
 NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
 USART_Cmd(USART2,ENABLE);  //����ʹ��
}

//�������ݣ����죩
uint16_t rv1[10]={0};
uint16_t rvcnt1=0;
uint8_t rvflag1=0;//������ɱ�־λ
uint8_t rvflagrtos=0;
void USART2_IRQHandler(void)//�����жϺ���
{
	//uint8_t data=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==1)//���ָ���� USART �жϷ������//���USART_CR1�Ĵ����е�RXNEIEΪ1��������жϡ���USART_DR�Ķ��������Խ���λ���㡣RXNEλҲ����ͨ��д��0�������ֻ���ڶ໺��ͨѶ�в��Ƽ������������0������û���յ��� 1���յ����ݣ����Զ�����
	{
		//data=USART_ReceiveData(USART1);
		//if(USART_ReceiveData(USART2)!=0Xff)
		//{
		rv1[rvcnt1++]=USART_ReceiveData(USART2);//��������
		if(rvcnt1>8)
		{
		rvcnt1=0;
		}
		//}
	}
//	if(USART_GetITStatus(USART2,USART_IT_IDLE)==1)//
//	{
//		//data=USART_ReceiveData(USART1);
//		//rv[rvcnt++]=USART_ReceiveData(USART1);
//		rvflag1=1;
//		USART2->SR;		
//		USART2->DR;//������б�־λ
//	}
	BaseType_t pxHigherPriorityTaskWoken;
	if(USART_GetITStatus(USART2,USART_IT_IDLE)==1)//
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		rvflag1=1;
		//xSemaphoreGiveFromISR( BinarySem_Handle,&pxHigherPriorityTaskWoken );// �ͷŶ�ֵ�ź���
	//	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л���ϵͳ���ж��Ƿ���Ҫ�����л�
//	if ( xReturn == pdTRUE )
//	printf("BinarySem_Handle ��ֵ�ź����ͷųɹ�!\r\n");
//	else
//	printf("BinarySem_Handle ��ֵ�ź����ͷ�ʧ��!\r\n");
//	rvflagrtos=1;
		USART2->SR;		
		USART2->DR;//������б�־λ
	}
}
//1 8 0 1��
//���� 1 ��λ\���� 1 ��λ��VOC ������ݣ�16 ���ƣ�����λ 0.1PPM
//���� 2 ��λ\���� 2 ��λ����ȩ������ݣ�16 ���ƣ�����λ 0.01mg/m3
//���� 3 ��λ\���� 3 ��λ��CO2 ������ݣ�16 ���ƣ�����λ 1PPM
void an1(void)
{ 	
	//printf("%d\r\n",rvcnt1);
	if(rvflag1==1)
	{
		//printf("%d\r\n",rvcnt1);
		if(rvcnt1==8)
		{
			if(rv1[0]==0x5f)
		{
			if((uint8_t)(rv1[0]+rv1[1]+rv1[2]+rv1[3]+rv1[4]+rv1[5]+rv1[6]==rv1[7]))//У��
			{
				printf("VOC: %.2f\r\n",((rv1[1]<<8|rv1[2]))*0.1);
				printf("��ȩ: %.2f\r\n",((rv1[3]<<8|rv1[4]))*0.01);
				printf("CO2: %d\r\n",(rv1[5]<<8|rv1[6]));
				rvcnt1=0;
				memset(rv1,0,10);
			}
			else
			{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("��������\r\n");
			}		
		}
			else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("��ַ����\r\n");
		}
	}
		else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("���ݲ��ȳ��Ȳ���8\r\n");
		}
		rvflag1=0;
	}
	else
	{
	printf("���ǿ���\r\n");
	}
}

void ankqm6600_rtos(void)
{
	if(rvcnt1==8)
		{
			if(rv1[0]==0x5f)
		{
			if((uint8_t)(rv1[0]+rv1[1]+rv1[2]+rv1[3]+rv1[4]+rv1[5]+rv1[6])==rv1[7])//У��
			{
				printf("VOC: %.2f\r\n",((rv1[1]<<8|rv1[2]))*0.1);
				printf("��ȩ: %.2f\r\n",((rv1[3]<<8|rv1[4]))*0.01);
				printf("CO2: %d\r\n",(rv1[5]<<8|rv1[6]));
				rvcnt1=0;
				memset(rv1,0,10);
			}
			else
			{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("��������\r\n");
			}		
		}
			else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("��ַ����\r\n");
		}
	}
		else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("���ݲ��ȳ��Ȳ���8\r\n");
		}
		rvflag1=0;
}

