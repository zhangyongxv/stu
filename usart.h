#ifndef _USART_H
#define _USART_H
#include "stm32f10x.h"
void usart_confg(void);
void usart_sendbyte(uint8_t date);
void usart_sendstr(uint8_t str[],uint8_t len);
uint8_t recive_data(void);
uint8_t an(void);
#endif