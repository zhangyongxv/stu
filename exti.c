#include "exti.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
void exti0_config(void)
{
	RCC->APB2ENR|=(1<<0);//开启复用时钟
	
	AFIO->EXTICR[0]&=~(0xF<<0);//配置PA0为外部中断
	//AFIO->EXTICR[1]&=~(0xfff<<0);//清零配置Pc456为外部中断
	//AFIO->EXTICR[1]|=~(0x222<<0);//置一
	//EXTI->IMR|=(7<<4);//开放中断线0请求//中断屏蔽寄存器
	//EXTI->RTSR &=~(7<<4);//关闭上升沿出发//允许输入线x上的上升沿触发(中断和事件)
	//EXTI ->FTSR |=(7<<4);//设置下降沿触发//禁止输入线x上的下降沿触发(中断和事件)
	//EXTI->PR |=(7<<4);
	//设置中断优先级
	//NVIC_SetPriority(EXTI4_IRQn,4);//抢占0，次级0
	//NVIC_EnableIRQ(EXTI4_IRQn);//使能中断
	//NVIC_SetPriority(EXTI9_5_IRQn,5);//抢占0，次级0
	//NVIC_EnableIRQ(EXTI9_5_IRQn);//使能中断
	
	EXTI->IMR|=(1<<0);//开放中断线0请求//中断屏蔽寄存器
	EXTI->RTSR |=(1<<0);//设置上升沿出发//允许输入线x上的上升沿触发(中断和事件)
	EXTI ->FTSR &=~(1<<0);//关闭下降沿触发//禁止输入线x上的下降沿触发(中断和事件)
	
	//要产生中断，必须先配置好并使能中断线。根据需要的边沿检测设置2个触发寄存器，同时在中
//断屏蔽寄存器的相应位写’1’允许中断请求。当外部中断线上发生了期待的边沿时，将产生一个
//中断请求，对应的挂起位也随之被置’1’。在挂起寄存器的对应位写’1’，将清除该中断请求
	
	EXTI->PR |=(1<<0);//挂起寄存器,写1清除挂起起位//当在外部中断线上发生了选择的边沿事件，该位被置’1’。在该位中写入’1’可以清除它，也可以
//通过改变边沿检测的极性清除。
	
	//设置中断优先级
	NVIC_SetPriority(EXTI0_IRQn,0);//抢占0，次级0
	NVIC_EnableIRQ(EXTI0_IRQn);//使能中断
	
}


void exti0_configlib(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* Selects PB.08 as EXTI Line 8 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//Pa0 中断线//选择 GPIO 管脚用作外部中断线路
//外部中断控制
 EXTI_InitTypeDef EXTI_InitStruct={0};//定义结构体变量
 EXTI_InitStruct.EXTI_Line= EXTI_Line0;
 EXTI_InitStruct.EXTI_LineCmd= ENABLE;
 EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
 
 EXTI_Init(&EXTI_InitStruct);
 
 //设置中断优先级
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 
 NVIC_Init(& NVIC_InitStruct);
 //
}

void exti1_configlib(void)  //pC4
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* Selects PB.08 as EXTI Line 8 */
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);//Pc4中断线//选择 GPIO 管脚用作外部中断线路
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);//Pc5 中断线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);//Pc6 中断线
	
//外部中断控制
 EXTI_InitTypeDef EXTI_InitStruct={0};//定义结构体变量
 EXTI_InitStruct.EXTI_Line= /*EXTI_Line4|EXTI_Line5|*/EXTI_Line6;
 EXTI_InitStruct.EXTI_LineCmd= ENABLE;
 EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
 
 EXTI_Init(&EXTI_InitStruct);
 
 //设置中断优先级
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 NVIC_Init(& NVIC_InitStruct);
 
 NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//号
 NVIC_Init(& NVIC_InitStruct);
 //	
}

//void exti2_configlib(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	/* Selects PB.08 as EXTI Line 8 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//Pa0 中断线
////外部中断控制
// EXTI_InitTypeDef EXTI_InitStruct={0};//定义结构体变量
// EXTI_InitStruct.EXTI_Line= EXTI_Line0;
// EXTI_InitStruct.EXTI_LineCmd= ENABLE;
// EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
// EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
// 
// EXTI_Init(&EXTI_InitStruct);
// 
// //设置中断优先级
// NVIC_InitTypeDef NVIC_InitStruct={0};
// NVIC_InitStruct.NVIC_IRQChannel=EXTI0_IRQn;
// NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
// NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
// NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
// 
// NVIC_Init(& NVIC_InitStruct);
// 
// //
//	
//}


 void EXTI0_IRQHandler()
 {
//	if((EXTI->PR &(1<<0))!=0)//发生中断请求
//	{
//		EXTI->PR |=(1<<0);//挂起寄存器,清除挂起起位
//		led_on(led1port,led1pin);
//	}
	 if(EXTI_GetITStatus(EXTI_Line0)!=0)
	 {
		  // static int a=0;
		  EXTI_ClearITPendingBit(EXTI_Line0);
		  led_turn(led2port,led2pin); 
		  //beep_turn(beepport,beeppin);
	 }
 }
 
 void EXTI4_IRQHandler()
 {
  if(EXTI_GetITStatus(EXTI_Line4)!=0)
	 {
		  EXTI_ClearITPendingBit(EXTI_Line4);
		  //led_on(led2port,led2pin);
		 ledr_turn(ledrport,ledrpin);
	 }
 }
 
 void EXTI9_5_IRQHandler () 
{
	if(EXTI_GetITStatus(EXTI_Line5)!=0)
	 {
		  EXTI_ClearITPendingBit(EXTI_Line5);
		  ledg_turn(ledgport,ledgpin);
	 }
	 if(EXTI_GetITStatus(EXTI_Line6)!=0)
	 {
		  EXTI_ClearITPendingBit(EXTI_Line6);
		  ledb_turn(ledbport,ledbpin);
	 }
}	

 
