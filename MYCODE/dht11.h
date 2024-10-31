#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f4xx.h"
#include "delay.h"
#include "stdio.h"

void Dht11_Init(void);
void Dht11_Pin_Mode(GPIOMode_TypeDef GPIO_Mode);
int	Dht11_Start(void);
uint8_t Dht11_Read_Byte();
int Dht11_Data();



#endif 

