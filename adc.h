#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x.h"
	uint16_t get_light();
	void adc_config();
	void amoke_config();
	void get_smoke();
	void wd_config();
	void GetTemperature(void);
	void get_sl(void);
	
	void svalue(void);
	
#endif