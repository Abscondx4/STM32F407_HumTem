#include "usart.h"

#include "stdio.h"

#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
    x = x; 
} 
//重定义fputc函数   printf 是一个宏
int fputc(int ch, FILE *f)
{     
    USART_SendData(USART1,ch);  //通过串口发送数据
    //等待数据发送完毕
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);      
    return ch;
}


/*
自定义数据通信协议

帧头	命令		硬件类型(子命令)	数据长度		数据		校验码		帧尾

H		C			L									 01						:	


帧头：代表这一帧数据包的开头  占1个字节    'H'
命令： 代表你要干嘛     占1个字节   
			控制命令		C
			获取信息命令 	G
			....
硬件类型（子命令）：对上面的命令再加以区分
			控制命令下的子命令 
						L		代表控制LED灯
						B		代表控制BEEP
						D		代表控制电机
						U		代表控制舵机
			获取信息命令的子命令
						。。。。
数据长度：代表 你要发送/接受数据的 字节数			 这里我们先不讨论   占0个字节
数据： 你要发送或者接收的源数据
校验码： 对传输的数据 进行  检错      CRC  奇校验  偶校验       这里也不讨论 占0个字节
帧尾： 这一帧数据的尾部   占1个字节     默认为 ':'


*/





/*
PA9  USART1_TX  发送端
PA10 USART1_RX  接收端
*/
void Usart1_Init(uint32_t baudRate)
{
	GPIO_InitTypeDef 	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;
	NVIC_InitTypeDef 	NVIC_InitStruct;
	
	//3. 串口时钟使能，GPIO 时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//5.GPIO 初始化设置：要设置模式为复用功能
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//复用功能输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP ;//推挽
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
	//4.设置引脚复用器映射：调用 GPIO_PinAFConfig 函数
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_USART1);
	
	//6.串口参数初始化：设置波特率，字长，奇偶校验等参数
	USART_InitStruct.USART_BaudRate = baudRate;//波特率
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据位  8
	USART_InitStruct.USART_StopBits = USART_StopBits_2;//停止位  2
	USART_InitStruct.USART_Parity = USART_Parity_No;//校验位  无校验
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;// 允许发送和接收 --全双工
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控 无
	USART_Init(USART1, &USART_InitStruct);
	
	
	//7.开启中断并且初始化 NVIC，使能中断（如果需要开启串口中断才需要这个步骤）
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//中断通道  中断源
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x01;//响应优先级

	NVIC_Init(&NVIC_InitStruct);
	
	//8.配置为接收中断（表示有数据过来，CPU要中断进行接收）
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  

	//9.使能串口
	USART_Cmd(USART1, ENABLE);	
}

//10.编写中断处理函数：函数名格式为 USARTx_IRQHandler(x 对应串口号)。
//中断服务函数中不能写延时  也不能写死循环
//void USART1_IRQHandler(void)
//{
//    //判断接收标志位是否置1
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//        //接收数据
//		u8 r_byte = USART_ReceiveData(USART1);
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
//		
//		//再转发 --回射
//		//USART_SendData(USART1, r_byte);
//    }
//	
//	//清除接收标志位
//    USART_ClearITPendingBit(USART1, USART_IT_RXNE);   
//}