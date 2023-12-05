#include "led.h"
#include "delay.h"

void led_config(void)
{
	RCC->APB2ENR |=(1<<6);//外设时钟使能
	GPIOE->CRL&=~(0xffff<<8);//端口配置低寄存器对应位清零
	GPIOE->CRL|=(0x3333<<8);//8到11位 0x3;
	//GPIOE->ODR&=~(0xf<<2);//pe2,3,4,5输出低电平
	//delay_ms(1);
	GPIOE->ODR|=(0xf<<2);//pe2,3,4,5输出高电平
}

void led_configlib(void)
{
	//例：
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//1.开时钟
	
	//例
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOE, &GPIO_InitStructure);//3.初始化寄存器

//例：
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOE, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}

//亮灯
void led_on(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR &=~(ledpin);
	
}
//灭灯
void led_off(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR |= (ledpin);
	
}
//反转
void led_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR ^= (ledpin);
	
}

void ledrgb_configlib(void)//rgb配置
{
	//例：
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//1.开时钟
	
	//例
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOA, &GPIO_InitStructure);

//例：
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOA, GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);//3.初始化寄存器
}

//红亮灯
void ledr_on(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
}
//灭灯
void ledr_off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
//红反转
void ledr_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledrport->ODR ^= (ledrpin);
}

//绿亮灯
void ledg_on(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}
void ledg_off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
}
void ledg_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledgport->ODR ^= (ledgpin);
}

//蓝
void ledb_on(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}
void ledb_off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}
void ledb_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledbport->ODR ^= (ledbpin);
}
