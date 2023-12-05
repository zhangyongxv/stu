#include "led.h"
#include "delay.h"

void led_config(void)
{
	RCC->APB2ENR |=(1<<6);//����ʱ��ʹ��
	GPIOE->CRL&=~(0xffff<<8);//�˿����õͼĴ�����Ӧλ����
	GPIOE->CRL|=(0x3333<<8);//8��11λ 0x3;
	//GPIOE->ODR&=~(0xf<<2);//pe2,3,4,5����͵�ƽ
	//delay_ms(1);
	GPIOE->ODR|=(0xf<<2);//pe2,3,4,5����ߵ�ƽ
}

void led_configlib(void)
{
	//����
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE , ENABLE);//1.��ʱ��
	
	//��
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOE, &GPIO_InitStructure);//3.��ʼ���Ĵ���

//����
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOE, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}

//����
void led_on(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR &=~(ledpin);
	
}
//���
void led_off(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR |= (ledpin);
	
}
//��ת
void led_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledport->ODR ^= (ledpin);
	
}

void ledrgb_configlib(void)//rgb����
{
	//����
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//1.��ʱ��
	
	//��
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

GPIO_Init(GPIOA, &GPIO_InitStructure);

//����
/* Set the GPIOA port pin 10 and pin 15 */
GPIO_SetBits(GPIOA, GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6);//3.��ʼ���Ĵ���
}

//������
void ledr_on(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	
}
//���
void ledr_off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
}
//�췴ת
void ledr_turn(GPIO_TypeDef *ledport,uint16_t ledpin)
{
	ledrport->ODR ^= (ledrpin);
}

//������
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

//��
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
