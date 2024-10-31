#include "bluetooth.h"

/*
PB10  USART3_TX  发送端
PB11  USART3_RX  接收端
*/
void BlueTooth_Init(int baudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 	NVIC_InitStruct;
	
	//3. 串口时钟使能，GPIO 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//5.GPIO 初始化设置：要设置模式为复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct); 
	
	//4.设置引脚复用器映射：调用 GPIO_PinAFConfig 函数
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_USART3);
	
	//6.串口参数初始化：设置波特率，字长，奇偶校验等参数
	USART_InitStruct.USART_BaudRate = baudRate;//波特率
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位  8
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位  1
	USART_InitStruct.USART_Parity = USART_Parity_No;//校验位  无校验
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;// 允许发送和接收 --全双工
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控 无
	USART_Init(USART3, &USART_InitStruct);

	//7.开启中断并且初始化 NVIC，使能中断（如果需要开启串口中断才需要这个步骤）
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//中断通道  中断源
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;//响应优先级

	NVIC_Init(&NVIC_InitStruct);
	
	//8.配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  

	//9.使能串口
	USART_Cmd(USART3, ENABLE);	
}


//中断服务函数
//void USART3_IRQHandler(void)
//{
//    //判断接收标志位是否置1
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
//    {
//        //接收数据  -来自于蓝牙的数据
//		u8 r_byte = USART_ReceiveData(USART3);
//		
//		//把每次接收的数据拼接到数组中rbuffer
//		rbuffer[r_count++] = r_byte;
//		
//		//代表接收到一帧数据
//		if(r_byte == ':')
//		{
//			r_flag = true;
//			printf("rbuffer:%s\r\n",rbuffer); //fputc
//		}
//		//再转发 --回射
//		USART_SendData(USART3, r_byte);
//    }
//	
//	//清除接收标志位
//    USART_ClearITPendingBit(USART3, USART_IT_RXNE);   
//}