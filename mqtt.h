#ifndef _MQTT_H
#define _MQTT_H
#include "stm32f10x.h"
uint8_t mqtt_connect();
uint8_t mqtt_publish(char *payload);
uint8_t mqtt_subscribe();
uint8_t mqtt_analysis();
#endif
