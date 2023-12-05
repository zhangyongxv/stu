#ifndef _ESP_H
#define _ESP_H
#include "stm32f10x.h"
void connect1(void);
void esp_confg(void);
void usart3_sendbyte(uint8_t date);
//void usart3_sendstr(uint8_t str[],uint8_t len);
void usart3_sendstr(uint8_t str[]);
typedef struct {
	uint8_t resrv1[1024];
	uint16_t esrvcnt1;
	uint8_t esrvflag1;//接收完成标志位
	uint8_t sen1[1024];
	uint16_t sencnt1;
} esp;
void usart3_sendstr1(uint8_t str[],uint16_t num);
void connect(void);
void pu(void);
#endif
