#include "usart.h"
#include "led.h"
#include "stdio.h"
#include "string.h"

//串口1驱动 TX_PA9复用推挽输出   RX_PA10浮空输入 APB2
void usart_confg(void)
{
	//开gpio时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出 
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		//串口开时钟
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 115200; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //八位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不适用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//不使用硬件流控制 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //全双工
	USART_Init(USART1,&USART_InitStructure);//初始阿华串口
	USART_Cmd(USART1,ENABLE);//串口使能
	//使能串口接收 中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//空闲中断
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
 NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);

}
//发送单个字节(构造)
void usart_sendbyte(uint8_t date)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==0)//上一个发送完成
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

//接受数据（构造）
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
uint8_t rvflag=0;//接收完成标志位
void USART1_IRQHandler(void)//串口1中断函数
{
	uint8_t data=0;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==1)//接收数据寄存器非空标志位//检查指定的 USART 中断发生与否//如果USART_CR1寄存器中的RXNEIE为1，则产生中断。对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。0：数据没有收到； 1：收到数据，可以读出。
	{
		data=USART_ReceiveData(USART1);
		USART_SendData(USART3,data);
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
	}
	
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=0)//空闲总线标志位
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		rvflag=1;
		USART1->SR;		
		USART1->DR;//清楚空闲标志位
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
				printf("六六六\r\n");
						rvcnt=0;
			memset(rv,0,10);	
			}
			else
			{
			rvcnt=0;
			memset(rv,0,10);
				printf("不符合协议\r\n");
			}
		}
		else
		{
		rvcnt=0;
			memset(rv,0,10);
			printf("数据不等长度不是5\r\n");
		}
	}
	rvflag=0;
}

int fputc(int ch, FILE *f)
{
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)//发送完成标志位
  {}
	USART_SendData(USART1, (uint8_t) ch);
  return ch;
}