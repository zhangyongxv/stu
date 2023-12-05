#include "adc.h"
#include "stdio.h"
#include "dma.h"
#include "beep.h"
extern uint16_t adc_value[2];
extern uint16_t va[2];
void adc_config()
{
	//pc1 adc123 11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);//��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//����ǿ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//����Ũ��
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ʱ�ӣ�ADCCLK��  12MHZ
	//����adc
	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 �� ADC2 �����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ADCɨ��ģʽ(��ɨ��ͨ���� ɨ��ͨ��)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC����ת��ģʽ(ֻת��һ�λ�������ת��)
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//ת��ע��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;//˳����й���ת���� ADC ͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);//��ʼ��
	ADC_Cmd(ADC1,ENABLE);//ʹ��ADC1;
	dma_config();
	ADC_DMACmd (ADC1,ENABLE);//����ADC1��dma��������
	//��λ��У׼
	ADC_ResetCalibration(ADC1);//����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//�ȴ��������
	ADC_StartCalibration (ADC1);//��ʼָ�� ADC ��У׼����
	while(ADC_GetCalibrationStatus(ADC1)==SET);//�ȴ��������
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_1Cycles5);// ����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_1Cycles5);// ����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
	ADC_SoftwareStartConvCmd (ADC1,ENABLE);//ʹ�ܻ���ʧ��ָ���� ADC �����ת����������
	}


//	ADC_GetCalibrationStatus ();//��ȡָ�� ADC ��У׼״̬
//	ADC_SoftwareStartConvCmd ();//ʹ�ܻ���ʧ��ָ���� ADC �����ת����������
//	ADC_GetSoftwareStartConvStatus();// ��ȡ ADC ���ת������״̬	
//	ADC_RegularChannelConfig();// ����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��	
//	ADC_GetConversionValue ();//�������һ�� ADCx �������ת�����
//	
	//��ȡ����ֵ
	uint16_t get_light()
	{
//		uint16_t light=0;
//		ADC_SoftwareStartConvCmd (ADC1,ENABLE);//ʹ�ܻ���ʧ��ָ���� ADC �����ת����������
//		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0)//�ȴ����ת������
//		{}
//		light=ADC_GetConversionValue(ADC1);//�������һ�� ADCx �������ת�����	
//		printf("����ǿ�ȣ�%d\r\n",light);	
//		printf("���յ�ѹֵ��%.2f\r\n",light*3.3/4096);
//		return light;
	}

	void get_sl(void)
	{
		uint16_t sm=0;
		uint16_t li=0;
		li=adc_value[0];
		sm=adc_value[1];
			//printf("���գ�%d\r\n",li);
			//printf("����%d\r\n",sm);
	}
	
		void svalue(void)
{	
	uint16_t s=0;
	uint16_t l=0;
	if(va[0]>20)
	{	
		for(int i=0;i<6;i++)
		{
		get_sl();
		l+=adc_value[0];
		s+=	adc_value[1];
		}
	}	
		printf("���գ�%d\r\n",l/6);
		printf("����%d\r\n",s/6);
	if(s/6>1000)
	{
	//beep_on(beepport,beeppin);
	}
	else
	{
	//beep_off(beepport,beeppin);
	}
	 va[0]=0;
}
	
	







	void amoke_config()
{
	//pc1 adc123 11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC2,ENABLE);//��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.����ṹ�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;//ģ������
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ʱ�ӣ�ADCCLK��  12MHZ
	//����adc
	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 �� ADC2 �����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ(��ɨ�赥��)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת���������������ǵ���ģʽ//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);//��ʼ��
	ADC_Cmd(ADC2,ENABLE);//ʹ��ADC1;
	
	//��λ��У׼
	ADC_ResetCalibration(ADC2);//����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1)==SET)//�ȴ��������
	{}	
	ADC_StartCalibration (ADC2);//��ʼָ�� ADC ��У׼����
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//�ȴ��������
	ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_1Cycles5);// ����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
}

	void get_smoke()
	{
		uint16_t light=0;
		ADC_SoftwareStartConvCmd (ADC2,ENABLE);//ʹ�ܻ���ʧ��ָ���� ADC �����ת����������
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0)//�ȴ����ת������
		{}
		light=ADC_GetConversionValue(ADC2);//�������һ�� ADCx �������ת�����	
		printf("����Ũ�ȣ�%d\r\n",light);	
		//printf("���յ�ѹֵ��%.2f\r\n",light*3.3/4096);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	//�����¶�
	void wd_config()
{
	//pc1 adc123 11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//��ʱ��
	//����adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //���� ADC ʱ�ӣ�ADCCLK��  12MHZ
	//����adc

	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 �� ADC2 �����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //ģ��ת��������ɨ��ģʽ����ͨ�������ǵ��Σ���ͨ����ģʽ(��ɨ�赥��)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת���������������ǵ���ģʽ//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);//��ʼ��
	
	ADC_Cmd(ADC2,ENABLE);//ʹ��ADC1;
	
	//��λ��У׼
		ADC_TempSensorVrefintCmd(ENABLE);//�������򿪣�
	ADC_ResetCalibration(ADC2);//����ָ���� ADC ��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC2)==SET);//�ȴ��������
	ADC_StartCalibration (ADC2);//��ʼָ�� ADC ��У׼����
	while(ADC_GetResetCalibrationStatus(ADC2)==SET);//�ȴ��������
	ADC_RegularChannelConfig(ADC2,ADC_Channel_16,1,ADC_SampleTime_239Cycles5);// ����ָ�� ADC �Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
}
	void GetTemperature(void)
{
	
	uint16_t light=0;
	uint16_t VSense=0;
		ADC_SoftwareStartConvCmd (ADC2,ENABLE);//ʹ�ܻ���ʧ��ָ���� ADC �����ת����������
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0);//�ȴ����ת������
		light=ADC_GetConversionValue(ADC2);//�������һ�� ADCx �������ת�����	
		VSense = light*3.3/4095.0;
		printf("%d\r\n",light);
		printf("����:%d; оƬ�¶�:%.2f\r\n",VSense,((1.43 - VSense)/0.0043+25));//�¶�(��C) = {(V25 - VSENSE) / Avg_Slope} + 25
}
		

	