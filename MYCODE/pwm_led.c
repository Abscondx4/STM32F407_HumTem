#include "pwm_led.h"

/*
LED0   PF9   TIM14  CH1
*/
void Pwm_PF9_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.ʹ�ܶ�ʱ��14�����IO��ʱ��
	//ʹ�ܶ�ʱ��14ʱ�ӣ�  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	//ʹ��GPIOFʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	//3.��ʼ��IO��Ϊ���ù������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
	
	//4.GPIOF9����ӳ�䵽��ʱ��14
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 
	
	//5.��ʼ����ʱ����ARR,PSC��
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//Ԥ��Ƶϵ��  1MHZ
	TIM_TimeBaseInitStruct.TIM_Period = 1000-1;//��װ��ֵ  ����ֵ   1ms
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//������ʽ ����  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//ʱ�ӷ�Ƶ����
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	//6.��ʼ������Ƚϲ���  ���ͨ��1��ʼ��
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//���ͨ��ʹ��
	TIM_OCInitStruct.TIM_Pulse = 0;//CCR1��ֵ ����ȽϼĴ�����ֵ ��ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//������� �͵�ƽ
	
	TIM_OC1Init(TIM14, &TIM_OCInitStruct);
	
	//7.ʹ��Ԥװ�ؼĴ���
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable); 
	
	//8.ʹ���Զ���װ�ص�Ԥװ�ؼĴ�������λ    
	TIM_ARRPreloadConfig(TIM14,ENABLE);
	
	//9.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM14,ENABLE);

}
