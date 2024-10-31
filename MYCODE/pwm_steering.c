#include "pwm_steering.h"

#include "delay.h"

void Pwm_PB10_TIM2_CH3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//2.ʹ�ܶ�ʱ��2�����IO��ʱ��
	//ʹ�ܶ�ʱ��2ʱ�ӣ�  84MHZ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//ʹ��GPIOAʱ�ӣ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//3.��ʼ��IO��Ϊ���ù������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//���ù������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//4.GPIOA2����ӳ�䵽��ʱ��2
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); 
	
	//5.��ʼ����ʱ����ARR,PSC��  84 000 000 / 840 = 10000 0HZ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 840-1;//Ԥ��Ƶϵ��  100000 HZ   1000ms����100000  ��ô1ms ��Ҫ����100��
	TIM_TimeBaseInitStruct.TIM_Period = 2000-1;//��װ��ֵ  ����ֵ   20ms
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

/*���º���Ϊ0-180�ȶ���Ƕȿ��Ʋ��Ժ���
	*	PWM �ź���0-180����Ĺ�ϵ��
	    ʱ��           ��������
        20ms		    2000
         1ms            100
	*	0.5ms			50		---------------- 0��
	*	1ms   			100		---------------- 45��
	*	1.5ms 			150		---------------- 90��
	*	2ms   			200		---------------- 135��
	*	2.5ms 			250		---------------- 180��
 
	*	���Ƶ����ռ�ձȵļ��㣺
	*	������Ƶ��Ϊ50HZ����PWM����Ϊ20ms��0�ȶ�Ӧ��ռ�ձ�Ϊ2.5%����0.05ms�ĸߵ�ƽ�����
 */
 
//0-180�ȶ�� --> 0-90-180�ȿ��ƺ���
void Server_180_Angle(void)
{
	TIM_SetCompare3(TIM2,50);//0��  0.5ms			50
	delay_s(2);
	
	TIM_SetCompare3(TIM2,150);//90��  1.5ms 			150
	delay_s(2);
	
	TIM_SetCompare3(TIM2,250);//180��  2.5ms 			250
	delay_s(2);
}