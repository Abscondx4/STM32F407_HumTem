#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx.h"
#include "stdbool.h"

extern int time_setting_flag;
extern bool time_flag;
extern float Tem_History[32];
extern int hum_flag;

void Rtc_Init(void);
void Get_RTC_Time(void);
void Rtc_Setting_Show(void);
void Rtc_Time_Up(void);
void Rtc_Time_Down(void);
void Time_Select(void);
void Time_Set_In(void);
void Sleep_mode();

#endif
