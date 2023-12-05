#include "exti.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
void exti0_config(void)
{
	RCC->APB2ENR|=(1<<0);//��������ʱ��
	
	AFIO->EXTICR[0]&=~(0xF<<0);//����PA0Ϊ�ⲿ�ж�
	//AFIO->EXTICR[1]&=~(0xfff<<0);//��������Pc456Ϊ�ⲿ�ж�
	//AFIO->EXTICR[1]|=~(0x222<<0);//��һ
	//EXTI->IMR|=(7<<4);//�����ж���0����//�ж����μĴ���
	//EXTI->RTSR &=~(7<<4);//�ر������س���//����������x�ϵ������ش���(�жϺ��¼�)
	//EXTI ->FTSR |=(7<<4);//�����½��ش���//��ֹ������x�ϵ��½��ش���(�жϺ��¼�)
	//EXTI->PR |=(7<<4);
	//�����ж����ȼ�
	//NVIC_SetPriority(EXTI4_IRQn,4);//��ռ0���μ�0
	//NVIC_EnableIRQ(EXTI4_IRQn);//ʹ���ж�
	//NVIC_SetPriority(EXTI9_5_IRQn,5);//��ռ0���μ�0
	//NVIC_EnableIRQ(EXTI9_5_IRQn);//ʹ���ж�
	
	EXTI->IMR|=(1<<0);//�����ж���0����//�ж����μĴ���
	EXTI->RTSR |=(1<<0);//���������س���//����������x�ϵ������ش���(�жϺ��¼�)
	EXTI ->FTSR &=~(1<<0);//�ر��½��ش���//��ֹ������x�ϵ��½��ش���(�жϺ��¼�)
	
	//Ҫ�����жϣ����������úò�ʹ���ж��ߡ�������Ҫ�ı��ؼ������2�������Ĵ�����ͬʱ����
//�����μĴ�������Ӧλд��1�������ж����󡣵��ⲿ�ж����Ϸ������ڴ��ı���ʱ��������һ��
//�ж����󣬶�Ӧ�Ĺ���λҲ��֮���á�1�����ڹ���Ĵ����Ķ�Ӧλд��1������������ж�����
	
	EXTI->PR |=(1<<0);//����Ĵ���,д1���������λ//�����ⲿ�ж����Ϸ�����ѡ��ı����¼�����λ���á�1�����ڸ�λ��д�롯1�������������Ҳ����
//ͨ���ı���ؼ��ļ��������
	
	//�����ж����ȼ�
	NVIC_SetPriority(EXTI0_IRQn,0);//��ռ0���μ�0
	NVIC_EnableIRQ(EXTI0_IRQn);//ʹ���ж�
	
}


void exti0_configlib(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* Selects PB.08 as EXTI Line 8 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//Pa0 �ж���//ѡ�� GPIO �ܽ������ⲿ�ж���·
//�ⲿ�жϿ���
 EXTI_InitTypeDef EXTI_InitStruct={0};//����ṹ�����
 EXTI_InitStruct.EXTI_Line= EXTI_Line0;
 EXTI_InitStruct.EXTI_LineCmd= ENABLE;
 EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
 
 EXTI_Init(&EXTI_InitStruct);
 
 //�����ж����ȼ�
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
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);//Pc4�ж���//ѡ�� GPIO �ܽ������ⲿ�ж���·
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource5);//Pc5 �ж���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);//Pc6 �ж���
	
//�ⲿ�жϿ���
 EXTI_InitTypeDef EXTI_InitStruct={0};//����ṹ�����
 EXTI_InitStruct.EXTI_Line= /*EXTI_Line4|EXTI_Line5|*/EXTI_Line6;
 EXTI_InitStruct.EXTI_LineCmd= ENABLE;
 EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
 EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Falling;
 
 EXTI_Init(&EXTI_InitStruct);
 
 //�����ж����ȼ�
 NVIC_InitTypeDef NVIC_InitStruct={0};
 NVIC_InitStruct.NVIC_IRQChannel=EXTI4_IRQn;//
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
 NVIC_Init(& NVIC_InitStruct);
 
 NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//��
 NVIC_Init(& NVIC_InitStruct);
 //	
}

//void exti2_configlib(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	/* Selects PB.08 as EXTI Line 8 */
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//Pa0 �ж���
////�ⲿ�жϿ���
// EXTI_InitTypeDef EXTI_InitStruct={0};//����ṹ�����
// EXTI_InitStruct.EXTI_Line= EXTI_Line0;
// EXTI_InitStruct.EXTI_LineCmd= ENABLE;
// EXTI_InitStruct.EXTI_Mode= EXTI_Mode_Interrupt;
// EXTI_InitStruct.EXTI_Trigger= EXTI_Trigger_Rising;
// 
// EXTI_Init(&EXTI_InitStruct);
// 
// //�����ж����ȼ�
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
//	if((EXTI->PR &(1<<0))!=0)//�����ж�����
//	{
//		EXTI->PR |=(1<<0);//����Ĵ���,���������λ
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

 
