#ifndef _LED_H
#define _LED_H
#include "stm32f10x.h"

#define led1port GPIOE
#define led2port GPIOE
#define led3port GPIOE
#define led4port GPIOE

#define led1pin  1<<2
#define led2pin  1<<3
#define led3pin  1<<4
#define led4pin  1<<5


#define ledrport GPIOA
#define ledgport GPIOA
#define ledbport GPIOA


#define ledrpin  1<<8
#define ledgpin  1<<7 
#define ledbpin  1<<6
void led_configlib(void);
void led_config(void);
void led_on(GPIO_TypeDef *ledport,uint16_t ledpin);
void led_off(GPIO_TypeDef *ledport,uint16_t ledpin);
void led_turn(GPIO_TypeDef *ledport,uint16_t ledpin);

void ledrgb_configlib(void);

void ledr_on(void);
void ledr_off(void);
void ledr_turn(GPIO_TypeDef *ledport,uint16_t ledpin);

void ledg_on(void);
void ledg_off(void);
void ledg_turn(GPIO_TypeDef *ledport,uint16_t ledpin);

void ledb_on(void);
void ledb_off(void);
void ledb_turn(GPIO_TypeDef *ledport,uint16_t ledpin);



#endif

