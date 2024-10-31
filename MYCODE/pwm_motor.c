#include "pwm_motor.h"


/*
��������ˣ�IN1 PA2  TIM2 CH3    ----UART2_TXD2   P5  TXD2
*/
void Pwm_PA2_TIM2_CH3_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.ʹ�ܶ�ʱ��2�����IO��ʱ��
	//ʹ�ܶ�ʱ��2ʱ�ӣ�  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//ʹ��GPIOAʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//3.��ʼ��IO��Ϊ���ù������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//4.GPIOA2����ӳ�䵽��ʱ��2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 
	
	//5.��ʼ����ʱ����ARR,PSC��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//Ԥ��Ƶϵ��  1MHZ
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//��װ��ֵ  ����ֵ   1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ ����  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//6.��ʼ������Ƚϲ���  ���ͨ��1��ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//���ͨ��ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR1��ֵ ����ȽϼĴ�����ֵ ��ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//������� �ߵ�ƽ
	
	TIM_OC3Init(TIM2, &TIM_OCInitStruct);
	
	//7.ʹ��Ԥװ�ؼĴ���
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable); 
	
	//8.ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ    
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	
	//9.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2,ENABLE);
}

/*
����ķ��ˣ�PA3  TIM2 CH4    ----UART2_RXD2   P5  RXD2
*/
void Pwm_PA3_TIM2_CH4_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.ʹ�ܶ�ʱ��2�����IO��ʱ��
	//ʹ�ܶ�ʱ��2ʱ�ӣ�  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//ʹ��GPIOAʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//3.��ʼ��IO��Ϊ���ù������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//4.GPIOA3����ӳ�䵽��ʱ��2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 
	
	//5.��ʼ����ʱ����ARR,PSC��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//Ԥ��Ƶϵ��  1MHZ
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//��װ��ֵ  ����ֵ   1ms
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ ����  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	//6.��ʼ������Ƚϲ���  ���ͨ��1��ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//���ͨ��ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR1��ֵ ����ȽϼĴ�����ֵ ��ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//������� �ߵ�ƽ
	
	TIM_OC4Init(TIM2, &TIM_OCInitStruct);
	
	//7.ʹ��Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable); 
	
	//8.ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ    
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	
	//9.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM2,ENABLE);
}