#include "rgb.h"
void rgb_config(void)
{
//PA6---B--TIM3-CH1  PA7----G--TIM3-CH2  PA8-----R------TIM1-CH1
	
	//pb8 pb9 TIM4 34;apb1//电机
//kaishizxhong
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4,ENABLE);//开启GPIO和定时器时钟
//设置gpio口模式
		/* Configure all the GPIOA in Input Floating mode */
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;//定时器4通道4-3
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//设置定时器模式
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0; //时钟分割
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;//计数模式
	//TIM_TimeBaseInitStruct.TIM_Period=7199;//重装载值
	TIM_TimeBaseInitStruct.TIM_Period=1000-1;
	//TIM_TimeBaseInitStruct.TIM_Prescaler=1999;//预分频
	TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;//预分频
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
//配置pwm
	TIM_OCInitTypeDef TIM_OCInitStructure={0};
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//设置pwm模式
	//TIM_OCInitStructure. = TIM_Channel_1;//通道
	TIM_OCInitStructure.TIM_Pulse = 0;//比较直
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//极性“有效电平位低电平”
	TIM_OCInitStructure.TIM_OutputState= TIM_OutputState_Enable;//
	
	TIM_OC1Init(TIM3, & TIM_OCInitStructure);//使能定时器3第一通道
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//使能定时器3第二通道
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//使能定时器1第二通道
	TIM_OC3Init(TIM4, & TIM_OCInitStructure);//使能定时器3第一通道
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);//使能定时器3第二通道
//	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//使能高级定时器1主输出
	TIM_Cmd(TIM1,ENABLE);//使能定时器1
	TIM_Cmd(TIM3,ENABLE);
//	
	//TIM_CtrlPWMOutputs(TIM4,ENABLE);//使能定时器1主输出
	TIM_Cmd(TIM4,ENABLE);
}

void setlight(uint16_t r,uint16_t g,uint16_t b)
{
TIM_SetCompare1(TIM3,b);//蓝
TIM_SetCompare2(TIM3,g);//绿
TIM_SetCompare1(TIM1,r);//红
}