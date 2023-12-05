#include "beep.h"

void beep_config(void)
{
	RCC->APB2ENR |=(1<<4);//����ʱ��ʹ��
	GPIOC->CRL&=~(0xf<<0);//�˿����õͼĴ�����Ӧλ����
	GPIOC->CRL|=(0x3<<0);//8��11λ 0x3;
	GPIOC->ODR&=~(0x1<<0);//pc0����ߵ�ƽ
}

void beep_configlib(void)
{
	//����
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//1.��ʱ��
	
	//��
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOC, &GPIO_InitStructure);

//����
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOE, GPIO_Pin_0);//3.��ʼ���Ĵ���
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
