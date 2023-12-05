#include "dht11.h"
#include "delay.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
DHT11_H11 dht11;
//PG11  浮空输入//推完输出/
//mode 1 输出模式 0 输入模式
void dht11_config(uint8_t mode)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	if(mode==1)
	{
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出 
		GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	}
	else if(mode==0)
	{
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_IN_FLOATING ;//浮空输入 
	}
	GPIO_Init(GPIOG,& GPIO_InitStructure);	
}
//起始信号
void start(void)
{
	dht11_config(1);//输出模式
	DHT11_L;//输出低电平
	delay_ms(20);//演示二四毫秒
}
//响应信号  返回0真确 其他错误
uint8_t check(void)
{	
	uint8_t cnt=0;
	dht11_config(0);//
	while(DHT11_STATE==1)			//等待低电平的到来
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 1;
		}
	}
	cnt=0;//程序执行到这里说明总线属于低电平
	while(DHT11_STATE==0)			//等待高电平的到来
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 2;
		}
	}
		return 0;
}
//读取一位
uint8_t readonrbit(void)
{
	uint8_t cnt=0;
	while(DHT11_STATE==1)			//等待低电平的到来
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 3;
		}
	}
	cnt=0;
	while(DHT11_STATE==0)			//等待高电平的到来
	{
		delay_1us();
		cnt++;
		if(cnt>100)
		{
		return 4;
		}
	}
	delay_us(30);//延时30us判断电平状态
	if(DHT11_STATE==0)
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}
//接受一个字节
uint8_t readonebyte()
{
	uint8_t data=0;
	for(uint8_t i=0;i<8;i++)
	{
		data<<=1;//左移一位 
		data|=(readonrbit());
	}
	return data;
}
//获取温湿度
//extern uint16_t dhtime[2];
uint8_t value[5];
void get_tem_hum(void)
{
	start();
	if(check()==0)
	{
		for(uint8_t i=0;i<5;i++)
		{
		 value[i]=readonebyte();
		}
		if(value[0]+value[1]+value[2]+value[3]==value[4])
		{
			dht11.hum=value[0];
			dht11.tem=value[2]+value[3]/10;
		  //printf("湿度：%.2f\r\n",dht11.hum);
			//printf("温度：%.2f\r\n",dht11.tem);
			if(dht11.tem>30)
			{
				//beep_on(beepport, beeppin);
			}
			else
			{
				//beep_off(beepport, beeppin);
			}
			if(dht11.hum>60)
			{
				//ledb_on();
			}
			else
			{
				//ledb_off();
			}	
		}
	}
	else
	{
	// printf("dht11响应失败\r\n");
	}
	//dhtime[0]=0;
}