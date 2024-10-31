#include "adc_light.h"

/*
PF7  ADC3  IN5   ����������
*/
void Adc3_Light_Init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStruct;
	ADC_InitTypeDef  			ADC_InitStruct;
	ADC_CommonInitTypeDef 		ADC_CommonInitStruct;
	
	// ADC3ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	// ����ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 
	
	//2.GPIO�ڳ�ʼ��  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//PA7����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;//ģ������ģʽ
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
	
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//��Ƶϵ�� 84MHZ/4 = 21MHZ  ADCƵ�ʲ�����36MHZ
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//�ر�DMA����
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;//����ģ���ź�֮��Ĳ������
	
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b ;//ѡ��12λ�ֱ��ʣ������4095
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;// ���ݶ����ʽ��ѡ���Ҷ���
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//����ת��ģʽ��Ҫ�ر�
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ޱ��ش���
	
//	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//Ӳ������;

	ADC_InitStruct.ADC_NbrOfConversion = 1;//ת��ͨ����Ŀ
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//ɨ��ѡ��,�ر�
	ADC_Init(ADC3, &ADC_InitStruct);
	
	//ʹ��ADC3
	ADC_Cmd(ADC3,ENABLE);
	
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_56Cycles);
}


u16 Get_Adc_Light_Value(void)
{
    u16 value;
    
    //�������ת����
    ADC_SoftwareStartConv(ADC3);

    //�ȴ�ת������
    while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
    
    
    value = ADC_GetConversionValue(ADC3);
    
    return value;
}
//��ȡ����ǿ��
uint16_t Reality_ADLight(uint16_t ADCnum)
{
	  return 100 - ADCnum / 40;
}
