#include "adc_poten.h"

void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	
	// ADC1时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// 引脚时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	
	//2.GPIO口初始化  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//PA5引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;//模拟输入模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//输出类型为推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;//速度
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;//分频系数 84MHZ/4 = 21MHZ  ADC频率不超过36MHZ
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//关闭DMA请求
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_15Cycles;//两个模拟信号之间的采样间隔
	
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b ;//选用12位分辨率，即最大4095
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;// 数据对齐格式，选用右对齐
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//连续转换模式，要关闭
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//无边沿触发
	ADC_InitStruct.ADC_NbrOfConversion = 1;//转换通道数目
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//扫描选择,关闭
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_56Cycles);

	
}


u16 Get_Adc_Value(void)
{
    u16 value;
    
    //开启软件转换：
    ADC_SoftwareStartConv(ADC1);

    //等待转换结束
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    
    value = ADC_GetConversionValue(ADC1);
    
    return value;
}
