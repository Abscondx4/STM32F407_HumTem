#include "exti.h"
#include "OLED_I2C.h"
#include "delay.h"
#include <stdio.h>

// 控制速度
int speed = 800;
// 控制方向
bool direct = true;

int menu_flag = 0;
void Exti_PA0_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  // 1.GPIOA的时钟使能以及系统配置控制器的时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2.GPIOA0初始化  输入模式 浮空
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;        // 0引脚
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;     // 输入模式
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 浮空

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 3.由系统配置控制器 完成 GPIOA 与中断线0的连接
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  // 4.外部中断/事件控制器EXTI的初始化 中断线0 中断模式
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;              // 中断线0
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // 使能
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVIC嵌套中断控制器的初始化 设置中断源（中断通道） 中断的优先级
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; // 中断通道  中断源
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // 使能
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 响应优先级
  NVIC_Init(&NVIC_InitStruct);
}

// 控制 正转
// 6.编写中断服务函数--由CPU自动调用
void EXTI0_IRQHandler(void) {
  // 最好还要判断一下是否发生了中断请求而且是否使能了中断
  if (EXTI_GetITStatus(EXTI_Line0) == SET) {
    // 翻转LED0的状态
    if (tem_flag == false)
    {
      if (menu_flag >=2) {
        menu_flag = 0;
      }
      else
      {
        menu_flag++;
      }

      delay_ms(10);
      OLED_CLS();
      if (time_flag)
      {
        Time_Set_In();
        time_flag = false;
      }
      
    }
    printf("menu_flag: %d\n", menu_flag);
  }

  // 清空中断标志位
  EXTI_ClearITPendingBit(EXTI_Line0);
}

void Exti_PE234_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  // 1.GPIOA的时钟使能以及系统配置控制器的时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2.GPIOA0初始化  输入模式 浮空
  GPIO_InitStruct.GPIO_Pin =
      GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7; // 0引脚
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;              // 输入模式
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;          // 浮空

  GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 3.由系统配置控制器 完成 GPIO 与中断线的连接
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);

  // 4.外部中断/事件控制器EXTI的初始化 中断线0 中断模式
  EXTI_InitStruct.EXTI_Line = EXTI_Line2;              // 中断线0
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // 使能
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVIC嵌套中断控制器的初始化 设置中断源（中断通道） 中断的优先级
  NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn; // 中断通道  中断源
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // 使能
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 响应优先级
  NVIC_Init(&NVIC_InitStruct);

  // 3.由系统配置控制器 完成 GPIO 与中断线的连接
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

  // 4.外部中断/事件控制器EXTI的初始化 中断线0 中断模式
  EXTI_InitStruct.EXTI_Line = EXTI_Line3;              // 中断线3
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // 使能
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVIC嵌套中断控制器的初始化 设置中断源（中断通道） 中断的优先级
  NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn; // 中断通道  中断源
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // 使能
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 响应优先级
  NVIC_Init(&NVIC_InitStruct);

  // 3.由系统配置控制器 完成 GPIO 与中断线的连接
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

  // 4.外部中断/事件控制器EXTI的初始化 中断线0 中断模式
  EXTI_InitStruct.EXTI_Line = EXTI_Line4;              // 中断线4
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // 使能
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVIC嵌套中断控制器的初始化 设置中断源（中断通道） 中断的优先级
  NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn; // 中断通道  中断源
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // 使能
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 响应优先级
  NVIC_Init(&NVIC_InitStruct);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

  // 4.外部中断/事件控制器EXTI的初始化 中断线0 中断模式
  EXTI_InitStruct.EXTI_Line = EXTI_Line1;              // 中断线4
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // 使能
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // 中断模式
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVIC嵌套中断控制器的初始化 设置中断源（中断通道） 中断的优先级
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn; // 中断通道  中断源
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // 使能
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // 抢占优先级
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // 响应优先级
  NVIC_Init(&NVIC_InitStruct);
}

void EXTI1_IRQHandler(void) {
  // 最好还要判断一下是否发生了中断请求而且是否使能了中断
  if (EXTI_GetITStatus(EXTI_Line1) == SET) {
    GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
  }
  EXTI_ClearITPendingBit(EXTI_Line1);
}

// 控制反转
// 6.编写中断服务函数--由CPU自动调用
void EXTI2_IRQHandler(void) {
  // 最好还要判断一下是否发生了中断请求而且是否使能了中断
  if (EXTI_GetITStatus(EXTI_Line2) == SET) {
    if (tem_flag == false)
    {
      if (time_flag)
      {
        Rtc_Time_Up();
      }
      if (menu_flag == 2)
      {
        printf("page up\r\n");
        if (History_Flag > 3 && (History_Page * 4 - 1) < History_Flag)
        {
          if (History_Page == 9)
            ;
          else{
            
            History_Page++;
            printf("page:%d\r\n", History_Page);
            OLED_CLS();
          }
        }
        
      }
      
    }
    
  }

  // 清空中断标志位
  EXTI_ClearITPendingBit(EXTI_Line2);
}
// 加速
// 6.编写中断服务函数--由CPU自动调用
void EXTI3_IRQHandler(void) {
  // 最好还要判断一下是否发生了中断请求而且是否使能了中断
  if (EXTI_GetITStatus(EXTI_Line3) == SET) {
    if (tem_flag == false) {
      if (time_flag) {
        Rtc_Time_Down();
      }
      if (menu_flag == 2) {
        if (History_Page == 0)
          ;
        else
        {
          History_Page--;
          printf("page:%d\r\n", History_Page);
          OLED_CLS();
        }
        
      }
    }
  }

  // 清空中断标志位
  EXTI_ClearITPendingBit(EXTI_Line3);
}

// 控制减速
// 6.编写中断服务函数--由CPU自动调用
void EXTI4_IRQHandler(void) {
  // 最好还要判断一下是否发生了中断请求而且是否使能了中断
  if (EXTI_GetITStatus(EXTI_Line4) == SET) {
    delay_ms(80);
      if (tem_flag) {
        tem_flag = false;
        GPIO_ResetBits(GPIOF, GPIO_Pin_8);
      } else {
        if (time_flag)
        {
          Time_Select();
        }
        
      }
    GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
    }

  // 清空中断标志位
  EXTI_ClearITPendingBit(EXTI_Line4);
}
