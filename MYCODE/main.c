#include "FreeRTOS.h"
#include "OLED_I2C.h"
#include "adc_light.h"
#include "adc_poten.h"
#include "beep.h"
#include "bluetooth.h"
#include "delay.h"
#include "dht11.h"
#include "exti.h"
#include "flash.h"
#include "iic.h"
#include "iwdg.h"
#include "key.h"
#include "led.h"
#include "lu90614.h"
#include "pwm_led.h"
#include "pwm_motor.h"
#include "pwm_steering.h"
#include "rtc.h"
#include "sr04.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_pwr.h"
#include "string.h"
#include "task.h"
#include "tim2.h"
#include "tim6.h"
#include "usart.h"


u8 rbuffer[100] = {0};

int r_count = 0;
u8 r_flag = false;

bool tem_flag = false;
bool setting_flag = false;

void menu1();
void Show_History();
static void Listen_Distence(void *pvParameters);
static void Menu_Show(void *pvParameters);

float Tem_History[32];
int hum_flag = 0;
int History_Flag;
int History_Page;
int main() {

  int distance;

  time_setting_flag = 0;
  ret = Dht11_Start();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  Beep_Init();
  Delay_Init();
  Led_Init();
  Rtc_Init();
  Key_Init();
  Exti_PA0_Init();
  Exti_PE234_Init();

  // Pwm_PB10_TIM2_CH3_Init();
  // Tim6_Init();
  // Tim2_Init();
  // Pwm_PF9_Init();

  // Pwm_PA2_TIM2_CH3_Init();
  // Pwm_PA3_TIM2_CH4_Init();

  Usart1_Init(9600);
  LV90614_Init(9600);
  Dht11_Init();
  Adc3_Light_Init();
  // BlueTooth_Init(9600);
  Sr04_Init();
  // Dht11_Init();
  // Rtc_Init();

  unsigned char i;

  Tem_History[0] = 36.5;
  Tem_History[1] = 36.6;
  Tem_History[2] = 36.7;
  Tem_History[3] = 36.8;
  History_Flag = 4;

  I2C_Configuration();
  OLED_Init();

  OLED_Fill(0x00);
  delay_s(2);

  u16 Command_HT[3];
  Command_HT[0] = 0XFA;
  Command_HT[1] = 0XC5;
  Command_HT[2] = 0XBF;

  USART3_SendData(Command_HT[0]);
  delay_us(10);
  USART3_SendData(Command_HT[1]);
  delay_us(10);
  USART3_SendData(Command_HT[2]);

  // // xTaskCreate((TaskFunction_t)Listen_Distence,"Listen_Distence", 512,
  // NULL, 2, NULL); xTaskCreate((TaskFunction_t)Menu_Show, "Menu_Show", 512,
  // NULL, 3, NULL);

  // vTaskStartScheduler();

  //	PWR_EnterSTANDBYMode();

  while (1) {
    if (menu_flag == 0) {
      Get_RTC_Time();
    }

    if (menu_flag == 1) {
      Rtc_Setting_Show();
    }

    if (menu_flag == 2) {
      Show_History();
    }
    if (menu_flag == 3) {
      menu1();
    }
    distance = Get_Sr04_Value();
    printf("distance:%d\n", distance);
    if (distance <= 5) {
      delay_ms(10);
      OLED_CLS();
      menu_flag = 3;
    }
    delay_ms(500);
  }

  // return 0;
}

// static void Listen_Distence(void *pvParameters)
// {
//   while (1)
//   {
//     int distance = Get_Sr04_Value();
//     if (distance <= 10)
//     {
//       menu_flag = 0;
//     }
//     vTaskDelay(1);
//   }

// }

// static void Menu_Show(void *pvParameters)
// {
//   while (1) {
//     // if (menu_flag == 0) {
//       menu1();
//     // }

//     // if (menu_flag == 1) {
//     //   Get_RTC_Time();
//     //   OLED_ShowCN(48, 0, 5);
//     //   OLED_ShowCN(64, 0, 6);
//     // }

//     // if (menu_flag == 2) {
//     //   menu2();
//     // }
//     vTaskDelay(1);
//   }
// }

void menu1() {

  tem_flag = true;

  int distance;
  distance = Get_Sr04_Value();
  printf("distance:%d\n", distance);

  OLED_ShowCN(48, 0, 3);
  OLED_ShowCN(64, 0, 0);

  u8 HumTem_H, HumTem_L;

  u16 Command_Start[3];
  Command_Start[0] = 0XFA;
  Command_Start[1] = 0XCA;
  Command_Start[2] = 0XC4;

  char HumTem[50];

  HumTem_H = USART_RX_BUF[2];
  HumTem_L = USART_RX_BUF[3] / 10;
  HumTem_H = HumTem_H & 0X00FF;
  sprintf(HumTem, "%d.%d", HumTem_H, HumTem_L);
  USART_RX_STA = 0;

  delay_ms(200);
  USART3_SendData(Command_Start[0]);
  delay_ms(10);
  USART3_SendData(Command_Start[1]);
  delay_ms(10);
  USART3_SendData(Command_Start[2]);

  OLED_ShowStr(32, 3, HumTem, 2, 0);
  OLED_ShowCN(80, 3, 10);

  OLED_ShowCN(32, 6, 3);
  OLED_ShowCN(48, 6, 0);
  if (HumTem_H >= 37) {
    OLED_ShowCN(64, 6, 16);
    OLED_ShowCN(80, 6, 17);
    GPIO_SetBits(GPIOF, GPIO_Pin_8);
  } else {
    OLED_ShowCN(64, 6, 14);
    OLED_ShowCN(80, 6, 15);
  }

  if (HumTem_H >= 100) {
    OLED_CLS();
  }

  Tem_History[History_Flag] = HumTem_H;
  Tem_History[History_Flag] += ((float)HumTem_L / 10);
  History_Flag++;

  while (tem_flag == true)
    ;
  menu_flag = 1;
  OLED_CLS();
}

/**
 * 显示历史记录函数
 * 该函数用于在OLED屏幕上显示存储的历史数据
 * 它根据History_Page的值来决定显示历史记录的哪一页
 * 当menu_flag为2时，持续显示历史记录
 */
void Show_History() {
  // 清除OLED屏幕内容，准备显示新的信息
  OLED_CLS();
  // 定义一个临时字符串数组，用于存储格式化的历史记录信息
  char Tem_Temp[50];
  // 初始化历史记录页码为第0页
  History_Page = 0;
  // 当menu_flag为2时，持续显示历史记录
  while (menu_flag == 2) {
    // 如果当前历史记录页码为第0页
    if (History_Page == 0) {
      // 显示第0页的固定内容
      OLED_ShowCN(48, 0, 20);
      OLED_ShowCN(64, 0, 21);
      // 遍历并显示前3条历史记录
      for (int i = 0; i < 3; i++) {
        // 如果达到当前历史记录标记，则停止显示
        if (i == History_Flag) {
          break;
        }
        // 格式化历史记录信息，并显示在OLED屏幕上
        sprintf(Tem_Temp, "%d:%.2f", i + 1, Tem_History[i]);
        OLED_ShowStr(0, 2 * (i + 1), Tem_Temp, 2, 0);
        OLED_ShowCN(110, 2 * (i + 1), 10);
      }
    } else {
      // 显示除第0页外的其他页历史记录
      for (int i = (4 * History_Page - 1), j = 0;
           i < (4 * (History_Page + 1) - 1); i++, j++) {
        // 调试信息，显示当前遍历的索引和历史记录标记
        printf("i:%d\r\n", i);
        printf("History_Flag:%d\r\n", History_Flag);
        // 如果达到当前历史记录标记，则停止显示
        if (i == History_Flag) {
          printf("break\r\n");
          break;
        }
        // 调试信息，显示当前遍历的索引和行号
        printf("i:%d\n", i);
        printf("j:%d\r\n", j);
        // 格式化历史记录信息，并显示在OLED屏幕上
        sprintf(Tem_Temp, "%d:%.2f", i + 1, Tem_History[i]);
        OLED_ShowStr(0, 2 * j, Tem_Temp, 2, 0);
        OLED_ShowCN(110, 2 * j, 10);
      }
    }
    // 延时3秒后清除屏幕，准备下一次显示
    delay_s(3);
    OLED_CLS();
  }
}

