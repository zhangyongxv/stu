#include "delay.h"
#include "dht11.h"
//构造微秒延时函数
void delay_us(uint32_t t)
{
	while(t--)
	{
		delay_1us();
	}
}
void Delay_us(uint32_t t)
{
	while(t--)
	{
		delay_1us();
	}
}

uint16_t led1time[2]={0,500};
uint16_t led2time[2]={0,500};
uint16_t led3time[2]={0,500};
uint16_t led4time[2]={0,500};

uint16_t keytime[2]={0,20};
uint16_t keytime2[2]={0,20};
uint16_t dhtime[2]={0,20};
uint16_t light[2]={0,20};
uint16_t va[2]={0,20};
uint16_t va1[2]={0,20};
//void SysTick_Handler(void)//系统定时器中断函数
//{
//	led4time[0]++;
//	led1time[0]++;
//	led2time[0]++;
//	led3time[0]++;
//	keytime[0]++;
//	keytime2[0]++;
//	dhtime[0]++;
//	light[0]++;
//	va[0]++;
//	va1[0]++;
//}

//构造毫秒秒延时函数
void delay_ms(uint32_t t)
{
	t*=1000;
	while(t--)
	{
		delay_1us();
	}
}
void Delay_ms(uint32_t t)
{
	t*=1000;
	while(t--)
	{
		Delay_1us();
	}
}

