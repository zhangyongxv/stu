#include "beep.h"

void beep_config(void)
{
	RCC->APB2ENR |=(1<<4);//外设时钟使能
	GPIOC->CRL&=~(0xf<<0);//端口配置低寄存器对应位清零
	GPIOC->CRL|=(0x3<<0);//8到11位 0x3;
	GPIOC->ODR&=~(0x1<<0);//pc0输出高电平
}

void beep_configlib(void)
{
	//例：
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//1.开时钟
	
	//例
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOC, &GPIO_InitStructure);

//例：
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOE, GPIO_Pin_0);//3.初始化寄存器
}

void beep_on(GPIO_TypeDef *beeport,uint16_t beepin)
{
	beeport->ODR|=(beepin);
}

void beep_off(GPIO_TypeDef *beeport,uint16_t beepin)
{
	beeport->ODR&=~(beepin);
}

void beep_turn(GPIO_TypeDef *beeport,uint16_t beepin)
{
	beeport->ODR^=(beepin);
}
