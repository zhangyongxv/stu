//串口4 pc10TX pc11
#include "su03t.h"
#include "led.h"
#include "stdio.h"
#include "string.h"
#include "beep.h"
#include "dht11.h"
extern DHT11_H11 dht11;
//串口1驱动 TX_PA9复用推挽输出   RX_PA10浮空输入 APB2
void usart_confg4(void)
{
	//开gpio时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//串口开时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 9600; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //八位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不适用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//不使用硬件流控制 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //全双工
	USART_Init(UART4,&USART_InitStructure);//初始阿华串口
	//使能串口接收 中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);//空闲中断
	//中断优先级
	/*
	使用分数波特率发生器 —— 12位整数和4位小数的表示方法。
● 一个状态寄存器(USART_SR)
● 数据寄存器(USART_DR)
● 一个波特率寄存器(USART_BRR)，12位的整数和4位小数
● 一个智能卡模式下的保护时间寄存器(USART_GTPR)
	*/
		//中断优先级
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=UART4_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
	
	USART_Cmd(UART4,ENABLE);//串口使能
	

}
//发送单个字节(构造)
void usart_sendbyte4(uint8_t date)
{
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==0)//上一个发送完成
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
////接受数据（构造）
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
uint8_t rvflag4=0;//接收完成标志位
uint8_t se4[10]={0};//发送
void UART4_IRQHandler(void)//串口1中断函数
{
	uint8_t data=0;
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==1)//检查指定的 USART 中断发生与否//如果USART_CR1寄存器中的RXNEIE为1，则产生中断。对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。0：数据没有收到； 1：收到数据，可以读出。
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
		UART4->DR;//清楚空闲标志位
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
//分析接受数组函数
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
					case 1: //获取温度
					se4[0]=	0xAA;
					se4[1]=	0x55;
					se4[2]=	0x01;
					se4[3]=	dht11.tem;//30度
					se4[4]=	0x55;
					se4[5]=	0xAA;
					usart_sendstr4(se4,6);	
					break;
				  case 2:
					se4[0]=	0xAA;
					se4[1]=	0x55;
					se4[2]=	0x02;
					se4[3]=	dht11.hum;//30度
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
				printf("六六六\r\n");
				rvcnt4=0;
				memset(rv4,0,10);	
				memset(se4,0,10);	
			}
			else
			{
			rvcnt4=0;
			memset(rv4,0,10);
				printf("校验不对\r\n");
			}
		}
		else
		{
		rvcnt4=0;
			memset(rv4,0,10);
			printf("数据不等长度不是6\r\n");
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