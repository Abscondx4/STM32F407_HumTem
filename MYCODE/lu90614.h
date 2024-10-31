#ifndef __LU90614_H
#define __LU90614_H

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"

#define USART_REC_LEN 200 // 定义最大接收字节数 200
#define EN_USART1_RX 1    // 使能（1）/禁止（0）串口1接收

extern u8 USART_RX_BUF
    [USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA; // 接收状态标记
extern u16 USART_RX_LEN; // 接收状态标记

extern u8 rbuffer[100];
extern int r_count;
extern u8 r_flag;

void LV90614_Init(int baudRate);
void USART3_SendData(u16 data);

#endif