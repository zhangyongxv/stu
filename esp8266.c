#include "esp8266.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"
#include "mqtt.h"
#include "su03t.h"
#include "dht11.h"
//TX——pb10 
//Rx-pb11
esp esp8266;
extern DHT11_H11 dht11;
void esp_confg(void)
{
	//开gpio时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通tuiwan输出
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推完输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//串口开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitTypeDef USART_InitStructure={0};
	USART_InitStructure.USART_BaudRate = 115200; //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //八位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//不适用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;//不使用硬件流控制 
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;  //全双工
	USART_Init(USART3,&USART_InitStructure);//初始化串口
	
	//使能串口接收 中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//更新中断
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//空闲中断
	
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(&NVIC_InitStruct);
 USART_Cmd(USART3,ENABLE);//串口使能
}

uint16_t esrv1[10]={0};
uint16_t esrvcnt1=0;
uint8_t esrvflag1=0;//接收完成标志位

void USART3_IRQHandler(void)//串口中断函数
{
	uint8_t data=0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==1)//检查指定的 USART 中断发生与否//如果USART_CR1寄存器中的RXNEIE为1，则产生中断。对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除，只有在多缓存通讯中才推荐这种清除程序。0：数据没有收到； 1：收到数据，可以读出。
	{
		data =USART_ReceiveData(USART3);
		USART_SendData(USART1,data);
		if(data=='\0')
		{
			data ='b';
		}
		esp8266.resrv1[esp8266.esrvcnt1++]=data;//USART_ReceiveData(USART3);
	}
	if(USART_GetITStatus(USART3,USART_IT_IDLE)==1)//
	{
		//data=USART_ReceiveData(USART1);
		//rv[rvcnt++]=USART_ReceiveData(USART1);
		esp8266.esrvflag1=1;
		USART3->SR;		
		USART3->DR;//清楚空闲标志位
	}
}

//发送单个字节(构造)
void usart3_sendbyte(uint8_t date)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==0)//上一个发送完成
 {}
	 USART_SendData(USART3,date);
}

void usart3_sendstr1(uint8_t str[],uint16_t num)
{
 for(int i=0;i<num;i++)
	{
	usart3_sendbyte(str[i]);
	}
}

void usart3_sendstr(uint8_t str[])
{
	uint8_t i=0;
	while(str[i]!='\0')
	{
	usart3_sendbyte(str[i++]);
	}
}

//1.发送相应的at指令，2.判断wifi模块是否正确响应（正常返回0，一场返回一）
uint8_t sendcmdcheck(char *cmd,char *str,uint16_t ms)
{
	
	//uint8_t data=2;
	memset(esp8266.resrv1,0,1024);//清空缓冲区
	esp8266.esrvcnt1=0;//清空计数器
	usart3_sendstr((uint8_t*)cmd);
	while(ms--)
	{
		if(esp8266.esrvflag1==1)
		{
			esp8266.esrvflag1=0;
			if((strstr((char *)esp8266.resrv1,str))!=NULL)
			{
				return 0;
		  //data=0;//返回数据正常
			}
		}
	delay_ms(1);
	}
return 1;
}

uint8_t sendcmdcheck1(char *str,uint16_t ms)
{
	//uint8_t data=2;
	//char str[]={"GET http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp"};
	while(esp8266.esrvflag1==0){}
	memset(esp8266.resrv1,0,1024);//清空缓冲区
	esp8266.esrvcnt1=0;//清空计数器
	usart3_sendstr((uint8_t*)"GET http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp");
	//printf("%s\r\n","GET http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp");
	while(ms--)
	{
		if(esp8266.esrvflag1==1)
		{
			//strcpy ((char *)esp8266.resrv1,"123445");
			//printf("%s\r\n",(char *)esp8266.resrv1);
			//esp8266.esrvflag1=0;
			if((strstr((char *)esp8266.resrv1,"SUCCESS"))!=NULL)
			{
				printf("%s\r\n",esp8266.resrv1);
				esp8266.esrvflag1=0;
				return 0;
		  //data=0;//返回数据正常
			}
		}
	delay_ms(1);
	}
	esp8266.esrvflag1=0;
return 1;
}

char msgbuff[1024];

void connect(void)
{
	//get_tem_hum();
	//dht11.hum;//float
	//dht11.tem;

	//sprintf(msgbuff,"{\"id\":1689737038818,\"params\":{\"temperature\":%f,\
								\"Humidity\":%f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}",dht11.hum,dht11.tem);
	if(sendcmdcheck("AT\r\n","OK", 2000)==0)
	{
		if(sendcmdcheck("AT+CWMODE=1\r\n","OK", 2000)==0)
		{
			if(sendcmdcheck("AT+CWJAP=\"realme GT neo 2\",\"qaz20001020Y\"\r\n","OK",20000)==0)
			{
				if(sendcmdcheck("AT+CIPSTART=\"TCP\",\"iot-06z00gms7nxts13.mqtt.iothub.aliyuncs.com\",1883\r\n","OK",2000)==0)
				{
					if(sendcmdcheck("AT+CIPMODE=1\r\n","OK", 2000)==0)
					{
						if(sendcmdcheck("AT+CIPSEND\r\n","OK", 2000)==0)
						{
							if(mqtt_connect()==0)
							{
							printf("MQTT连接成功\r\n");
						//	mqtt_publish("{\"id\":1689737038818,\"params\":{\"temperature\":55,\
								\"Humidity\":99},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}");
								//mqtt_publish(msgbuff);
							mqtt_subscribe();//订阅
							}
							
//							else
//							{
//							
//							}
							
						}
						
					}
					
				}
				
			}
			
		}
		
	}
	
}

//
void pu(void)//发布数据
{
	get_tem_hum();
	sprintf(msgbuff,"{\"id\":1689737038818,\"params\":{\"temperature\":%f,\
								\"Humidity\":%f},\"version\":\"1.0\",\"method\":\"thing.event.property.post\"}",dht11.tem,dht11.hum);
	mqtt_publish(msgbuff);//发布
}



//void connect1(void)
//{
//	
//	if(sendcmdcheck("AT\r\n","OK", 2000)==0)
//	{
//		if(sendcmdcheck("AT+CWMODE=1\r\n","OK", 2000)==0)
//		{
//			if(sendcmdcheck("AT+CWJAP=\"realme GT neo 2\",\"qaz20001020Y\"\r\n","OK",20000)==0)
//			{
//				if(sendcmdcheck("AT+CIPSTART=\"TCP\",\"api.m.taobao.com\",80\r\n","CONNECT",5000)==0)
//				{
//					if(sendcmdcheck("AT+CIPMODE=1\r\n","OK", 2000)==0)
//					{
//						if(sendcmdcheck("AT+CIPSEND\r\n","OK", 2000)==0)
//						{
//							if(sendcmdcheck1("GET http://api.m.taobao.com/rest/api3.do?api=mtop.common.getTimestamp\r\n","SUCCESS",5000)==0)
//							{
//							printf("tb连接成功\r\n");
//							printf("%s\r\n",(char  *)esp8266.resrv1);
//								
//							}					
//							else
//							{
//							printf("tb连接失败\r\n");
//								printf("%s\r\n",(char  *)esp8266.resrv1);
//							}
//							
//						}
//						
//					}
//					
//				}
//				
//			}
//			
//		}
//		
//	}
//}
