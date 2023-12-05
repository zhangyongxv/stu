#include "kqm6600.h"
#include "stdio.h"
#include "string.h"
#include "freertos.h"
#include "semphr.h"
extern SemaphoreHandle_t BinarySem_Handle;//二值信号量句柄
//USART2_RX  PA3  APB1
void usart2_confg(void)
{
	//开gpio时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//串口开时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2,ENABLE);
	USART_InitTypeDef USART_InitStructure={0}; 
	USART_InitStructure.USART_BaudRate = 9600; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //八位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不适用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//不使用硬件流控制 
	USART_InitStructure.USART_Mode = USART_Mode_Rx;  //全双工
	USART_Init(USART2,&USART_InitStructure);//初始化串口
	//使能串口接收 中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//更新中断
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//空闲中断
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
 NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
 USART_Cmd(USART2,ENABLE);  //串口使能
}

//接受数据（构造）
uint16_t rv1[10]={0};
uint16_t rvcnt1=0;
uint8_t rvflag1=0;//接收完成标志位
uint8_t rvflagrtos=0;
void USART2_IRQHandler(void)//串口中断函数
{
	//uint8_t data=0;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==1)//检查指定的 USART 中断发生与否//如果USART_CR1寄存器中的RXNEIE为1，则产生中断。对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。0：数据没有收到； 1：收到数据，可以读出。
	{
		//data=USART_ReceiveData(USART1);
		//if(USART_ReceiveData(USART2)!=0Xff)
		//{
		rv1[rvcnt1++]=USART_ReceiveData(USART2);//接受数据
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
//		USART2->DR;//清楚空闲标志位
//	}
	BaseType_t pxHigherPriorityTaskWoken;
	if(USART_GetITStatus(USART2,USART_IT_IDLE)==1)//
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		rvflag1=1;
		//xSemaphoreGiveFromISR( BinarySem_Handle,&pxHigherPriorityTaskWoken );// 释放二值信号量
	//	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);//如果需要的话进行一次任务切换，系统会判断是否需要进行切换
//	if ( xReturn == pdTRUE )
//	printf("BinarySem_Handle 二值信号量释放成功!\r\n");
//	else
//	printf("BinarySem_Handle 二值信号量释放失败!\r\n");
//	rvflagrtos=1;
		USART2->SR;		
		USART2->DR;//清楚空闲标志位
	}
}
//1 8 0 1；
//数据 1 高位\数据 1 低位：VOC 输出数据（16 进制），单位 0.1PPM
//数据 2 高位\数据 2 低位：甲醛输出数据（16 进制），单位 0.01mg/m3
//数据 3 高位\数据 3 低位：CO2 输出数据（16 进制），单位 1PPM
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
			if((uint8_t)(rv1[0]+rv1[1]+rv1[2]+rv1[3]+rv1[4]+rv1[5]+rv1[6]==rv1[7]))//校验
			{
				printf("VOC: %.2f\r\n",((rv1[1]<<8|rv1[2]))*0.1);
				printf("甲醛: %.2f\r\n",((rv1[3]<<8|rv1[4]))*0.01);
				printf("CO2: %d\r\n",(rv1[5]<<8|rv1[6]));
				rvcnt1=0;
				memset(rv1,0,10);
			}
			else
			{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("数据有误\r\n");
			}		
		}
			else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("地址有误\r\n");
		}
	}
		else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("数据不等长度不是8\r\n");
		}
		rvflag1=0;
	}
	else
	{
	printf("不是空闲\r\n");
	}
}

void ankqm6600_rtos(void)
{
	if(rvcnt1==8)
		{
			if(rv1[0]==0x5f)
		{
			if((uint8_t)(rv1[0]+rv1[1]+rv1[2]+rv1[3]+rv1[4]+rv1[5]+rv1[6])==rv1[7])//校验
			{
				printf("VOC: %.2f\r\n",((rv1[1]<<8|rv1[2]))*0.1);
				printf("甲醛: %.2f\r\n",((rv1[3]<<8|rv1[4]))*0.01);
				printf("CO2: %d\r\n",(rv1[5]<<8|rv1[6]));
				rvcnt1=0;
				memset(rv1,0,10);
			}
			else
			{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("数据有误\r\n");
			}		
		}
			else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("地址有误\r\n");
		}
	}
		else
		{
			rvcnt1=0;
			memset(rv1,0,10);
			printf("数据不等长度不是8\r\n");
		}
		rvflag1=0;
}

