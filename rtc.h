#ifndef _RTC_H
#define _RTC_H
#include "stm32f10x.h"
void rtc_config(void);

void NVIC_Configuration(void);
void RTC_Configuration(void);
void Time_Adjust(void);

#endif