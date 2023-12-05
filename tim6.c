#include "tim6.h"
#include "led.h"
#include "key.h"

void  time6_config(void)
{
//���ö�ʱ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//1.��ʱ��
	
//2.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	TIM_TimeBaseInitStruct.TIM_Period=7199;//��װ��ְ
	//TIM_TimeBaseInitStruct.TIM_Period=Period;
	TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//Ԥ��Ƶ
	//TIM_TimeBaseInitStruct.TIM_Prescaler=Prescale;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
//3.
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ�������ж�
	//4.�����ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
 
 //5.��ʱ��ʹ��
 TIM_Cmd(TIM6,ENABLE);
 }
void  time6_configc(uint16_t Period,uint16_t Prescale)
{
//���ö�ʱ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//1.��ʱ��	
//2.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0; //ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	//TIM_TimeBaseInitStruct.TIM_Period=7199;//��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Period=Period;
	//TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_Prescaler=Prescale;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStruct);
//3.
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ�������ж�//�����ж�����//��ʱ�����ʱ�����жϡ�������ڵĳ����ɶ�ʱ����ص�Ԥ��Ƶ������װ�ؼĴ��������þ�����
	//4.�����ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=TIM6_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=3;
 NVIC_Init(& NVIC_InitStruct);
 
 //5.��ʱ��ʹ��
 TIM_Cmd(TIM6,ENABLE);
 }
 //6.��д�жϷ�����
  void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
	{
		
//		led_turn(led1port,led1pin);
		TIM_ClearITPendingBit (TIM6,TIM_IT_Update);
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)		//��������
			{
			ledr_turn(ledrport,ledrpin);
			}
	}
}


void  time7_configc(uint16_t Period,uint16_t Prescale)
{
//���ö�ʱ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);//1.��ʱ��
	
//2.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	//TIM_TimeBaseInitStruct.TIM_Period=7199;//��װ��ְ
	TIM_TimeBaseInitStruct.TIM_Period=Period;
	//TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_Prescaler=Prescale;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStruct);
//3.
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ�������ж�
	//4.�����ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
 NVIC_Init(& NVIC_InitStruct);
 
 //5.��ʱ��ʹ��
 TIM_Cmd(TIM7,ENABLE);
 }
 void TIM7_IRQHandler(void)
 {
 if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)
	{
		
		led_turn(led2port,led2pin);
		TIM_ClearITPendingBit (TIM7,TIM_IT_Update);	
	}
 }
	 
	 
	 
	 
	 
	 
void  time2_configc(uint16_t Period,uint16_t Prescale)
{
//���ö�ʱ����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//1.��ʱ��
	
//2.
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	//TIM_TimeBaseInitStruct.TIM_Period=7199;//��װ��ְ
	TIM_TimeBaseInitStruct.TIM_Period=Period;
	//TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_Prescaler=Prescale;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
//3.
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//ʹ�ܶ�ʱ�������ж�
	//4.�����ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=TIM2_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
 NVIC_Init(&NVIC_InitStruct);
 
 //5.��ʱ��ʹ��
 TIM_Cmd(TIM2,ENABLE);
 }

 void TIM2_IRQHandler (void)
 {
	 if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		
		led_turn(led3port,led3pin);
		TIM_ClearITPendingBit (TIM2,TIM_IT_Update);
		
	}
 }
