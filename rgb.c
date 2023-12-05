#include "rgb.h"
void rgb_config(void)
{
//PA6---B--TIM3-CH1  PA7----G--TIM3-CH2  PA8-----R------TIM1-CH1
	
	//pb8 pb9 TIM4 34;apb1//���
//kaishizxhong
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);//����GPIO�Ͷ�ʱ��ʱ��
//����gpio��ģʽ
		/* Configure all the GPIOA in Input Floating mode */
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;//��ʱ��4ͨ��4-3
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//���ö�ʱ��ģʽ
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0; //ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//����ģʽ
	//TIM_TimeBaseInitStruct.TIM_Period=7199;//��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;
	//TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//Ԥ��Ƶ
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;//Ԥ��Ƶ
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
//����pwm
	TIM_OCInitTypeDef TIM_OCInitStructure={0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//����pwmģʽ
	//TIM_OCInitStructure. = TIM_Channel_1;//ͨ��
	TIM_OCInitStructure.TIM_Pulse = 0;//�Ƚ�ֱ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//���ԡ���Ч��ƽλ�͵�ƽ��
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;//
	
	TIM_OC1Init(TIM3, & TIM_OCInitStructure);//ʹ�ܶ�ʱ��3��һͨ��
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//ʹ�ܶ�ʱ��3�ڶ�ͨ��
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//ʹ�ܶ�ʱ��1�ڶ�ͨ��
	TIM_OC3Init(TIM4, & TIM_OCInitStructure);//ʹ�ܶ�ʱ��3��һͨ��
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);//ʹ�ܶ�ʱ��3�ڶ�ͨ��
//	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//ʹ�ܸ߼���ʱ��1�����
	TIM_Cmd(TIM1,ENABLE);//ʹ�ܶ�ʱ��1
	TIM_Cmd(TIM3,ENABLE);
//	
	//TIM_CtrlPWMOutputs(TIM4,ENABLE);//ʹ�ܶ�ʱ��1�����
	TIM_Cmd(TIM4,ENABLE);
}

void setlight(uint16_t r,uint16_t g,uint16_t b)
{
TIM_SetCompare1(TIM3,b);//��
TIM_SetCompare2(TIM3,g);//��
TIM_SetCompare1(TIM1,r);//��
}