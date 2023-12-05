#include "key.h"
#include "delay.h"
extern  uint16_t keytime2[2];
void key_config(void)
{
	RCC->APB2ENR |=(1<<2);
	RCC->APB2ENR |=(1<<4);
	GPIOA->CRL &=~(0xf<<0);
	GPIOA->CRL |=(0X4<<0);
	
	
	GPIOC->CRL &=~(0xfff<<16);
	GPIOC->CRL |=(0x444<<16);
}

void key_configlib(void)
{
	//����
/* Enable GPIOA, GPIOB and SPI1 clocks */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//1.��ʱ��
	
	//��
	/* Configure all the GPIOA in Input Floating mode */
GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOC, &GPIO_InitStructure);

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_Init(GPIOA, &GPIO_InitStructure);

//����
/* Set the GPIOA port pin 10 and pin 15 */
//GPIO_SetBits(GPIOE, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);//3.��ʼ���Ĵ���
}

uint8_t get_valuelib(void)
{ 
	uint8_t value=0;
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)		//��������
	{
		delay_ms(10);	
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)	
		{
			//keytime2[0]=0;
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)  //�ȴ�����̧��
			{		
			}			
//			if(keytime2[0]>3000)
//		{
//		  value=5;
//		}
//		else
//		{
			value=1;
//		}	
		}
	}
	
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==0)		//��������
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==0)
		{
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==0)  //�ȴ�����̧��
			{
	
			}
			value=2;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)		//��������
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)
		{
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)  //�ȴ�����̧��
			{
	
			}
			value=3;
		}
	}
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0)		//��������
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0)
		{
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0)  //�ȴ�����̧��
			{
	
			}
			value=4;
		}
	}
		return value;

}
