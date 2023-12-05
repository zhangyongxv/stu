#ifndef _SU03T_H
#define _SU03T_H
#include "stm32f10x.h"
void usart_confg4(void);
void usart_sendbyte4(uint8_t date);
void usart_sendstr4(uint8_t str[],uint8_t len);
uint8_t recive_data4(void);
uint8_t an4(void);
#endif