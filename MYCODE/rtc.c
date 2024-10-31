#include "rtc.h"
#include "OLED_I2C.h"
#include "delay.h"
#include <stdio.h>

#define BKR 0x23

int mintues_temp;
int ret;
bool time_flag;
int time_setting_flag;
int year_temp;
int month_temp;
int day_temp;
int hour_temp;
int minutes_temp;
int seconds_temp;

void Rtc_Init(void) {

  RTC_InitTypeDef RTC_InitStruct;
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  PWR_BackupAccessCmd(ENABLE);

 
  
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  RCC_RTCCLKCmd(ENABLE);
  
  RCC_LSEConfig(RCC_LSE_ON);

  
  RTC_InitStruct.RTC_AsynchPrediv = 0x7F; 
  RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
  RTC_InitStruct.RTC_SynchPrediv = 0xFF; 
  RTC_Init(&RTC_InitStruct);

  
  RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours = 9;
  RTC_TimeStruct.RTC_Minutes = 45;
  RTC_TimeStruct.RTC_Seconds = 40;

  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

  
  RTC_DateStruct.RTC_Year = 24;
  RTC_DateStruct.RTC_Month = 10;
  RTC_DateStruct.RTC_Date = 16;
  RTC_DateStruct.RTC_WeekDay = 1;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

  RTC_WriteBackupRegister(RTC_BKP_DR0, BKR);

  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
  year_temp = RTC_DateStruct.RTC_Year;
  month_temp = RTC_DateStruct.RTC_Month;
  day_temp = RTC_DateStruct.RTC_Date;
  hour_temp = RTC_TimeStruct.RTC_Hours;
  minutes_temp = RTC_TimeStruct.RTC_Minutes;
  seconds_temp = RTC_TimeStruct.RTC_Seconds;
}

void Get_RTC_Time() {

  u8 Tem_high, Tem_low;
  u8 data[5] = {0};

  char tem_Info[50];
  char hum_Info[50];

  RTC_DateTypeDef RTC_DateStruct;
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

  char Year[6];
  char Month[6];
  char Day[6];

  sprintf(Year, "20%d", RTC_DateStruct.RTC_Year);
  sprintf(Month, "%d", RTC_DateStruct.RTC_Month);
  sprintf(Day, "%d", RTC_DateStruct.RTC_Date);

  OLED_ShowStr(2, 0, Year, 2, 0);
  OLED_ShowCN(36, 0, 7);
  OLED_ShowStr(54, 0, Month, 2, 0);
  OLED_ShowCN(72, 0, 8);
  OLED_ShowStr(90, 0, Day, 2, 0);
  OLED_ShowCN(108, 0, 9);

  if (mintues_temp != RTC_TimeStruct.RTC_Minutes) {
    OLED_CLS();
    mintues_temp = RTC_TimeStruct.RTC_Minutes;
  }

  char Hours[8];
  sprintf(Hours, "%d", RTC_TimeStruct.RTC_Hours);
  char Minutes[8];
  sprintf(Minutes, "%d", RTC_TimeStruct.RTC_Minutes);
  char Seconds[8];
  sprintf(Seconds, "%d", RTC_TimeStruct.RTC_Seconds);
  char Colon[6] = " : ";

  OLED_ShowStr(16, 3, Hours, 2, 0);
  OLED_ShowStr(32, 3, Colon, 2, 0);
  OLED_ShowStr(56, 3, Minutes, 2, 0);
  OLED_ShowStr(72, 3, Colon, 2, 0);
  OLED_ShowStr(96, 3, Seconds, 2, 0);

  ret = Dht11_Start();
  if (ret == 0) {
    ret = Dht11_Data(data);
    if (ret == 0) {
      sprintf(hum_Info, "%d.%d%%", data[0], data[1]);
      sprintf(tem_Info, "%d.%d", data[2], data[3]);
    }
  }

  OLED_ShowStr(88, 6, hum_Info, 2, 0);
  OLED_ShowCN(40, 6, 10);
  OLED_ShowStr(0, 6, tem_Info, 2, 0);
  int light_level = Get_Adc_Light_Value();
  if (light_level >= 1000)
  {
    Sleep_mode();
  }
  
}

void Rtc_Setting_Show(void) {

  time_flag = true;

  OLED_CLS();

  char Year[6];
  char Month[6];
  char Day[6];
  char Hours[8];
  char Minutes[8];
  char Seconds[8];
  char Colon[6] = " : ";

  RTC_DateTypeDef RTC_DateStruct;
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);
  year_temp = RTC_DateStruct.RTC_Year;
  month_temp = RTC_DateStruct.RTC_Month;
  day_temp = RTC_DateStruct.RTC_Date;
  hour_temp = RTC_TimeStruct.RTC_Hours;
  minutes_temp = RTC_TimeStruct.RTC_Minutes;
  seconds_temp = RTC_TimeStruct.RTC_Seconds;
  
    while (time_flag)
    {
      switch (time_setting_flag) {
      case 0: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 1);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 0);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 0);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 0);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 0);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 0);
      } break;

      case 1: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 0);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 1);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 0);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 0);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 0);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 0);
      } break;

      case 2: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 0);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 0);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 1);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 0);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 0);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 0);
      } break;

      case 3: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 0);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 0);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 0);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 1);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 0);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 0);
      } break;

      case 4: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 0);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 0);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 0);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 0);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 1);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 0);
      } break;

      case 5: {
        OLED_ShowCN(48, 0, 18);
        OLED_ShowCN(64, 0, 19);
        
        sprintf(Year, "20%d", year_temp);
        sprintf(Month, "%d", month_temp);
        sprintf(Day, "%d", day_temp);
        sprintf(Hours, "%d", hour_temp);
        sprintf(Minutes, "%d", mintues_temp);
        sprintf(Seconds, "%d", seconds_temp);

        OLED_ShowStr(2, 3, Year, 2, 0);
        OLED_ShowCN(36, 3, 7);
        OLED_ShowStr(54, 3, Month, 2, 0);
        OLED_ShowCN(72, 3, 8);
        OLED_ShowStr(90, 3, Day, 2, 0);
        OLED_ShowCN(108, 3, 9);
        OLED_ShowStr(16, 6, Hours, 2, 0);
        OLED_ShowStr(32, 6, Colon, 2, 0);
        OLED_ShowStr(56, 6, Minutes, 2, 0);
        OLED_ShowStr(72, 6, Colon, 2, 0);
        OLED_ShowStr(96, 6, Seconds, 2, 1);
      } break;
      }
      delay_ms(500);
    }
    
}

void Time_Select(void)
{
  if (time_setting_flag >= 5) {
    time_setting_flag = 0;
  } else {
    time_setting_flag++;
  }
  printf("time_setting_flag = %d\n", time_setting_flag);
}
void Rtc_Time_Up(void)
{
  switch (time_setting_flag)
  {
  case 0:
  {
    if (year_temp >= 99)
    {
      OLED_CLS();
      year_temp = 0;
    }
    else
    {
      year_temp++;
    }
    
  }
    break;
    
  case 1:
  {
    if (month_temp >= 12)
    {
      OLED_CLS();
      month_temp = 1;
    }
    else
    {
      month_temp++;
    }
  }
    break;

  case 2:
  {
    if (day_temp >= 31)
    {
      OLED_CLS();
      day_temp = 1;
    }
    else
    {
      day_temp++;
    }
  }
    break;

  case 3:
  {
    if (hour_temp >= 23)
    {
      OLED_CLS();
      hour_temp = 0;
    }
    else
    {
      hour_temp++;
    }
  }
    break;

  case 4:
  {
    if (mintues_temp >= 59)
    {
      OLED_CLS();
      mintues_temp = 0;
    }
    else
    {
      mintues_temp++;
    }
  }
    break;

  case 5:
  {
    if (seconds_temp >= 59)
    {
      OLED_CLS();
      seconds_temp = 0;
    }
    else
    {
      seconds_temp++;
    }
  }
    break;
  }
}

void Rtc_Time_Down(void) {
  switch (time_setting_flag)
  {
  case 0:
  {
    if (year_temp <= 0)
    {
      year_temp = 99;
    }
    else
    {
      year_temp--;
    }
  }
    break;

  case 1: {
    if (month_temp <= 1)
    {
      month_temp = 12;
    }
    else
    {
      month_temp--;
    }
  }
    break;
    
  case 2: {
    if (day_temp <= 1)
    {
      day_temp = 31;
    }
    else
    {
      day_temp--;
    }
  }
    break;
    
  case 3: {
    if (hour_temp <= 0)
    {
      hour_temp = 23;
    }
    else
    {
      hour_temp--;
    }
  }
    break;
    
  case 4: {
    if (mintues_temp <= 0)
    {
      mintues_temp = 59;
    }
    else
    {
      mintues_temp--;
    }
  } 
    break;
    
  case 5: {
    if (seconds_temp <= 0)
    {
      seconds_temp = 59;
    }
    else
    {
      seconds_temp--;
    }
  } 
    break;
  }
}

void Time_Set_In(void)
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
  RTC_TimeStruct.RTC_Hours = hour_temp;
  RTC_TimeStruct.RTC_Minutes = mintues_temp;
  RTC_TimeStruct.RTC_Seconds = seconds_temp;

  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

  RTC_DateStruct.RTC_Year = year_temp;
  RTC_DateStruct.RTC_Month = month_temp;
  RTC_DateStruct.RTC_Date = day_temp;
  RTC_DateStruct.RTC_WeekDay = 1;
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);

  RTC_WriteBackupRegister(RTC_BKP_DR0, BKR);
}

void Sleep_mode() 
{
  OLED_CLS();
  __WFI();
}