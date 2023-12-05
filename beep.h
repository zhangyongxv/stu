#ifndef _BEEP_H
#define _BEEP_H
#include "stm32f10x.h"

#define beepport GPIOC
#define beeppin  1<<0

void beep_config(void);
void beep_configlib(void);
void beep_on(GPIO_TypeDef *beeport,uint16_t beepin);
void beep_off(GPIO_TypeDef *beeport,uint16_t beepin);
void beep_turn(GPIO_TypeDef *beeport,uint16_t beepin);

#endif
