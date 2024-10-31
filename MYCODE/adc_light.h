#ifndef ADC_LIGHT_H
#define ADC_LIGHT_H

#include "stm32f4xx.h"

void Adc3_Light_Init(void);

u16 Get_Adc_Light_Value(void);

uint16_t Reality_ADLight(uint16_t ADCnum);

#endif