#ifndef _TIM6_H
#define _TIM6_H
#include "stm32f10x.h"


void  time6_config(void);
void  time6_configc(uint16_t Period,uint16_t Prescale);
void  time7_configc(uint16_t Period,uint16_t Prescale);
void  time2_configc(uint16_t Period,uint16_t Prescale);
#endif
