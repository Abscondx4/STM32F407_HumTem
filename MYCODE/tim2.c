#include "tim2.h"

//��ʱ1����
void Tim2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//2.ʹ�ܶ�ʱ��ʱ�� 42MHZ X 2 = 84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//3.��ʼ����ʱ��������ARR,PSC��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 8400-1;//Ԥ��Ƶϵ��  84000 000HZ / 8400 =   1000 0HZ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ  ����/���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 2000 - 1 ;//�Զ���װ�ؼĴ�����ֵ  0~65535  ��ʱ200MS
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//4.������ʱ���жϣ�����NVIC��
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn; //��ʱ��2���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_Init(&NVIC_InitStruct);
	
	//5.������������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	//6.ʹ�ܶ�ʱ����
	TIM_Cmd(TIM2,ENABLE);
}
//7.��д�жϷ�������
void TIM2_IRQHandler(void)
{
	//�ж��Ƿ񴥷����ж�����
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		//ҵ��Ĵ���
		//��תLED0��״̬
		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);	
	}
	
	//����жϱ�־λ
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


