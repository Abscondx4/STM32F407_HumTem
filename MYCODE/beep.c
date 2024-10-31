#include "beep.h"

/*
PF8 ���Ʒ�����  ������� ����
�ߵ�ƽ 1  ��������
�͵�ƽ 0  ����������
*/
void Beep_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  // 1.ʹ��GPIOF��ʱ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_ResetBits(GPIOF, GPIO_Pin_8);
}
