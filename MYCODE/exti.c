#include "exti.h"
#include "OLED_I2C.h"
#include "delay.h"
#include <stdio.h>

// �����ٶ�
int speed = 800;
// ���Ʒ���
bool direct = true;

int menu_flag = 0;
void Exti_PA0_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  // 1.GPIOA��ʱ��ʹ���Լ�ϵͳ���ÿ�������ʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2.GPIOA0��ʼ��  ����ģʽ ����
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;        // 0����
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;     // ����ģʽ
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ����

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 3.��ϵͳ���ÿ����� ��� GPIOA ���ж���0������
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  // 4.�ⲿ�ж�/�¼�������EXTI�ĳ�ʼ�� �ж���0 �ж�ģʽ
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;              // �ж���0
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // ʹ��
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // �ж�ģʽ
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVICǶ���жϿ������ĳ�ʼ�� �����ж�Դ���ж�ͨ���� �жϵ����ȼ�
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; // �ж�ͨ��  �ж�Դ
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // ʹ��
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // ��Ӧ���ȼ�
  NVIC_Init(&NVIC_InitStruct);
}

// ���� ��ת
// 6.��д�жϷ�����--��CPU�Զ�����
void EXTI0_IRQHandler(void) {
  // ��û�Ҫ�ж�һ���Ƿ������ж���������Ƿ�ʹ�����ж�
  if (EXTI_GetITStatus(EXTI_Line0) == SET) {
    // ��תLED0��״̬
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

  // ����жϱ�־λ
  EXTI_ClearITPendingBit(EXTI_Line0);
}

void Exti_PE234_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  EXTI_InitTypeDef EXTI_InitStruct;
  NVIC_InitTypeDef NVIC_InitStruct;

  // 1.GPIOA��ʱ��ʹ���Լ�ϵͳ���ÿ�������ʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // 2.GPIOA0��ʼ��  ����ģʽ ����
  GPIO_InitStruct.GPIO_Pin =
      GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7; // 0����
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;              // ����ģʽ
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;          // ����

  GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 3.��ϵͳ���ÿ����� ��� GPIO ���ж��ߵ�����
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);

  // 4.�ⲿ�ж�/�¼�������EXTI�ĳ�ʼ�� �ж���0 �ж�ģʽ
  EXTI_InitStruct.EXTI_Line = EXTI_Line2;              // �ж���0
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // ʹ��
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // �ж�ģʽ
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVICǶ���жϿ������ĳ�ʼ�� �����ж�Դ���ж�ͨ���� �жϵ����ȼ�
  NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn; // �ж�ͨ��  �ж�Դ
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // ʹ��
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // ��Ӧ���ȼ�
  NVIC_Init(&NVIC_InitStruct);

  // 3.��ϵͳ���ÿ����� ��� GPIO ���ж��ߵ�����
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);

  // 4.�ⲿ�ж�/�¼�������EXTI�ĳ�ʼ�� �ж���0 �ж�ģʽ
  EXTI_InitStruct.EXTI_Line = EXTI_Line3;              // �ж���3
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // ʹ��
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // �ж�ģʽ
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVICǶ���жϿ������ĳ�ʼ�� �����ж�Դ���ж�ͨ���� �жϵ����ȼ�
  NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn; // �ж�ͨ��  �ж�Դ
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // ʹ��
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // ��Ӧ���ȼ�
  NVIC_Init(&NVIC_InitStruct);

  // 3.��ϵͳ���ÿ����� ��� GPIO ���ж��ߵ�����
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

  // 4.�ⲿ�ж�/�¼�������EXTI�ĳ�ʼ�� �ж���0 �ж�ģʽ
  EXTI_InitStruct.EXTI_Line = EXTI_Line4;              // �ж���4
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // ʹ��
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // �ж�ģʽ
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVICǶ���жϿ������ĳ�ʼ�� �����ж�Դ���ж�ͨ���� �жϵ����ȼ�
  NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn; // �ж�ͨ��  �ж�Դ
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // ʹ��
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // ��Ӧ���ȼ�
  NVIC_Init(&NVIC_InitStruct);

  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);

  // 4.�ⲿ�ж�/�¼�������EXTI�ĳ�ʼ�� �ж���0 �ж�ģʽ
  EXTI_InitStruct.EXTI_Line = EXTI_Line1;              // �ж���4
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;               // ʹ��
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     // �ж�ģʽ
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
  EXTI_Init(&EXTI_InitStruct);

  // 5.NVICǶ���жϿ������ĳ�ʼ�� �����ж�Դ���ж�ͨ���� �жϵ����ȼ�
  NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn; // �ж�ͨ��  �ж�Դ
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  // ʹ��
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01; // ��ռ���ȼ�
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;        // ��Ӧ���ȼ�
  NVIC_Init(&NVIC_InitStruct);
}

void EXTI1_IRQHandler(void) {
  // ��û�Ҫ�ж�һ���Ƿ������ж���������Ƿ�ʹ�����ж�
  if (EXTI_GetITStatus(EXTI_Line1) == SET) {
    GPIO_ToggleBits(GPIOF, GPIO_Pin_10);
  }
  EXTI_ClearITPendingBit(EXTI_Line1);
}

// ���Ʒ�ת
// 6.��д�жϷ�����--��CPU�Զ�����
void EXTI2_IRQHandler(void) {
  // ��û�Ҫ�ж�һ���Ƿ������ж���������Ƿ�ʹ�����ж�
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

  // ����жϱ�־λ
  EXTI_ClearITPendingBit(EXTI_Line2);
}
// ����
// 6.��д�жϷ�����--��CPU�Զ�����
void EXTI3_IRQHandler(void) {
  // ��û�Ҫ�ж�һ���Ƿ������ж���������Ƿ�ʹ�����ж�
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

  // ����жϱ�־λ
  EXTI_ClearITPendingBit(EXTI_Line3);
}

// ���Ƽ���
// 6.��д�жϷ�����--��CPU�Զ�����
void EXTI4_IRQHandler(void) {
  // ��û�Ҫ�ж�һ���Ƿ������ж���������Ƿ�ʹ�����ж�
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

  // ����жϱ�־λ
  EXTI_ClearITPendingBit(EXTI_Line4);
}
