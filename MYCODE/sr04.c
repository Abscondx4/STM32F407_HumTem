#include "sr04.h"

/*
STM32				������
PA2���				Trig
PA3����				echo
*/
void Sr04_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  // 1.ʹ��ʱ�� GPIO
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  // 2.���� PA2 ���
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;         // ����
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     // ���ģʽ
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    // �������Ϊ����
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;    // ����
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz; // �ٶ�

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA3����
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;         // ����
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;      // ����ģʽ
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;    // ����
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz; // �ٶ�

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // ��ʱ��Ҫ��ʼ�� ���ڼ���
  // 2.ʹ�ܶ�ʱ��ʱ�� 42MHZ X 2 = 84MHZ
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // 3.��ʼ����ʱ��������ARR,PSC��
  TIM_TimeBaseInitStruct.TIM_Prescaler =
      84 - 1; // Ԥ��Ƶϵ��  1MHZ  -->1���� ���� 1000 000
  TIM_TimeBaseInitStruct.TIM_CounterMode =
      TIM_CounterMode_Up; // ������ʽ  ����/���ϼ���
  TIM_TimeBaseInitStruct.TIM_Period = 50000 - 1; // �Զ���װ�ؼĴ�����ֵ 0~65535
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // ʱ�ӷ�Ƶ����
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

  // 6.��ʹ�ܶ�ʱ��
  TIM_Cmd(TIM3, DISABLE);
}

int Get_Sr04_Value() {
  int t = 0;
  // PA2����͵�ƽ
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);
  delay_us(8);
  // PA2����ߵ�ƽ
  GPIO_SetBits(GPIOA, GPIO_Pin_2);
  delay_us(20);

  // PA2����͵�ƽ
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);

  // ���ö�ʱ����CNTΪ0
  TIM3->CNT = 0;

  // PA3�ȴ��ߵ�ƽ�������ο������ɿ����� while( ����ƽ  );
  while (1) {
    uint8_t ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    if (ret == Bit_SET)
      break;

    delay_us(5);
    t++;

    if (t >= 1000) // 5000us   5ms ���һֱ�ղ����ߵ�ƽҲҪ�˳�
    {
      return 0;
    }
  }
  // ʹ�ܶ�ʱ����ʼ����
  TIM_Cmd(TIM3, ENABLE);

  // PA3�ȴ��͵�ƽ������while( ����ƽ  );
  t = 0;
  while (1) {
    uint8_t ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    if (ret == RESET) // �͵�ƽ���˳�
    {
      break;
    }
    delay_us(5);
    t++;

    if (t >= 1000) {
      return 0;
    }
  }
  // ��ȡ��ʱ��CNTֵ���Ӷ��õ��ߵ�ƽ����ʱ��    temp = TIM3->CNT;
  int temp = TIM3->CNT;

  // �رն�ʱ��
  TIM_Cmd(TIM3, DISABLE);

  // ͨ����ʽ�������������������  ����
  return temp / 58;
}
