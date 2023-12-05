#include "dma.h"
uint16_t adc_value[2]={0};
void dma_config()
{
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);//��ʱ��
	//DMA AHB���� DMA1��ch1;()()
	//12�������Ŀ����õ�ͨ��(����)��DMA1��7��ͨ����DMA2��5��ͨ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructure={0};
	DMA_InitStructure.DMA_BufferSize = 2;//�� DMA ͨ���� DMA ����Ĵ�С����λΪ���ݵ�λ�����ݴ��䷽�����ݵ�
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ʹ�� DMA ͨ����/�ڴ浽�ڴ�/����
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc_value;//���� DMA �ڴ����ַ
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_HalfWord;//�趨���������ݿ��//����	
//λ���ڽṹ�в��� DMA_PeripheralDataSize ���߲��� DMA_MemoryDataSize ��ֵ��
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����趨�ڴ��ַ�Ĵ����������//��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// CAN �Ĺ���ģʽ//������   ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// DMA ͨ�� x ��������ȼ�//������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����趨�����ַ�Ĵ����������//��
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord;//�趨���������ݿ��//16λ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;//���� DMA �������ַ
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ�ܻ���ʧ��ָ����ͨ�� x//148
}