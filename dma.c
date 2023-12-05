#include "dma.h"
uint16_t adc_value[2]={0};
void dma_config()
{
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);//开时钟
	//DMA AHB总线 DMA1—ch1;()()
	//12个独立的可配置的通道(请求)：DMA1有7个通道，DMA2有5个通道
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure={0};
	DMA_InitStructure.DMA_BufferSize = 2;//定 DMA 通道的 DMA 缓存的大小，单位为数据单位。根据传输方向，数据单
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据传输的来源
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// 不使能 DMA 通道的/内存到内存/传输
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_value;//定义 DMA 内存基地址
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord;//设定了外设数据宽度//半字	
//位等于结构中参数 DMA_PeripheralDataSize 或者参数 DMA_MemoryDataSize 的值。
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//用来设定内存地址寄存器递增与否//是
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// CAN 的工作模式//工作在   循环缓存模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// DMA 通道 x 的软件优先级//中优先
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//用来设定外设地址寄存器递增与否//否
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord;//设定了外设数据宽度//16位
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//定义 DMA 外设基地址
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能或者失能指定的通道 x//148
}