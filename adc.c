#include "adc.h"
#include "stdio.h"
#include "dma.h"
#include "beep.h"
extern uint16_t adc_value[2];
extern uint16_t va[2];
void adc_config()
{
	//pc1 adc123 11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);//开时钟
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//光照强度
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//烟雾浓度
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置 ADC 时钟（ADCCLK）  12MHZ
	//配置adc
	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 和 ADC2 工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; //ADC扫描模式(不扫描通道， 扫描通道)
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //ADC连续转换模式(只转换一次或者连续转换)
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//转换注入由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;//顺序进行规则转换的 ADC 通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);//初始化
	ADC_Cmd(ADC1,ENABLE);//使能ADC1;
	dma_config();
	ADC_DMACmd (ADC1,ENABLE);//开启ADC1的dma传输请求
	//复位和校准
	ADC_ResetCalibration(ADC1);//重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//等待重置完成
	ADC_StartCalibration (ADC1);//开始指定 ADC 的校准程序
	while(ADC_GetCalibrationStatus(ADC1)==SET);//等待重置完成
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_1Cycles5);// 设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_1Cycles5);// 设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
	ADC_SoftwareStartConvCmd (ADC1,ENABLE);//使能或者失能指定的 ADC 的软件转换启动功能
	}


//	ADC_GetCalibrationStatus ();//获取指定 ADC 的校准状态
//	ADC_SoftwareStartConvCmd ();//使能或者失能指定的 ADC 的软件转换启动功能
//	ADC_GetSoftwareStartConvStatus();// 获取 ADC 软件转换启动状态	
//	ADC_RegularChannelConfig();// 设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间	
//	ADC_GetConversionValue ();//返回最近一次 ADCx 规则组的转换结果
//	
	//获取光照值
	uint16_t get_light()
	{
//		uint16_t light=0;
//		ADC_SoftwareStartConvCmd (ADC1,ENABLE);//使能或者失能指定的 ADC 的软件转换启动功能
//		while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==0)//等待软件转换结束
//		{}
//		light=ADC_GetConversionValue(ADC1);//返回最近一次 ADCx 规则组的转换结果	
//		printf("关照强度：%d\r\n",light);	
//		printf("光照电压值：%.2f\r\n",light*3.3/4096);
//		return light;
	}

	void get_sl(void)
	{
		uint16_t sm=0;
		uint16_t li=0;
		li=adc_value[0];
		sm=adc_value[1];
			//printf("光照：%d\r\n",li);
			//printf("烟雾：%d\r\n",sm);
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
		printf("光照：%d\r\n",l/6);
		printf("烟雾：%d\r\n",s/6);
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC2,ENABLE);//开时钟
	
	GPIO_InitTypeDef GPIO_InitStructure={0};//2.定义结构体变量
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ;//模拟输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	//GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置 ADC 时钟（ADCCLK）  12MHZ
	//配置adc
	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 和 ADC2 工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //模数转换工作在扫描模式（多通道）还是单次（单通道）模式(不扫描单次)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在连续还是单次模式//单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);//初始化
	ADC_Cmd(ADC2,ENABLE);//使能ADC1;
	
	//复位和校准
	ADC_ResetCalibration(ADC2);//重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1)==SET)//等待重置完成
	{}	
	ADC_StartCalibration (ADC2);//开始指定 ADC 的校准程序
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//等待重置完成
	ADC_RegularChannelConfig(ADC2,ADC_Channel_11,1,ADC_SampleTime_1Cycles5);// 设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
}

	void get_smoke()
	{
		uint16_t light=0;
		ADC_SoftwareStartConvCmd (ADC2,ENABLE);//使能或者失能指定的 ADC 的软件转换启动功能
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0)//等待软件转换结束
		{}
		light=ADC_GetConversionValue(ADC2);//返回最近一次 ADCx 规则组的转换结果	
		printf("烟雾浓度：%d\r\n",light);	
		//printf("光照电压值：%.2f\r\n",light*3.3/4096);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	//面内温度
	void wd_config()
{
	//pc1 adc123 11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);//开时钟
	//配置adc
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //设置 ADC 时钟（ADCCLK）  12MHZ
	//配置adc

	ADC_InitTypeDef ADC_InitStructure={0};
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1 和 ADC2 工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; //模数转换工作在扫描模式（多通道）还是单次（单通道）模式(不扫描单次)
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在连续还是单次模式//单次转换
	ADC_InitStructure.ADC_ExternalTrigConv = 	ADC_ExternalTrigConv_None;//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);//初始化
	
	ADC_Cmd(ADC2,ENABLE);//使能ADC1;
	
	//复位和校准
		ADC_TempSensorVrefintCmd(ENABLE);//传感器打开，
	ADC_ResetCalibration(ADC2);//重置指定的 ADC 的校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC2)==SET);//等待重置完成
	ADC_StartCalibration (ADC2);//开始指定 ADC 的校准程序
	while(ADC_GetResetCalibrationStatus(ADC2)==SET);//等待重置完成
	ADC_RegularChannelConfig(ADC2,ADC_Channel_16,1,ADC_SampleTime_239Cycles5);// 设置指定 ADC 的规则组通道，设置它们的转化顺序和采样时间
}
	void GetTemperature(void)
{
	
	uint16_t light=0;
	uint16_t VSense=0;
		ADC_SoftwareStartConvCmd (ADC2,ENABLE);//使能或者失能指定的 ADC 的软件转换启动功能
		while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0);//等待软件转换结束
		light=ADC_GetConversionValue(ADC2);//返回最近一次 ADCx 规则组的转换结果	
		VSense = light*3.3/4095.0;
		printf("%d\r\n",light);
		printf("采样:%d; 芯片温度:%.2f\r\n",VSense,((1.43 - VSense)/0.0043+25));//温度(°C) = {(V25 - VSENSE) / Avg_Slope} + 25
}
		

	