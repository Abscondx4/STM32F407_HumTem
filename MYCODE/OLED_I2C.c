/************************************************************************************
 *  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
 *            All rights reserved.
 *
 * Http:    www.heltec.cn
 * Email:   cn.heltec@gmail.com
 * WebShop: heltec.taobao.com
 *
 * File name: OLED_I2C.c
 * Project  : HelTec.uvprij
 * Processor: STM32F103C8T6
 * Compiler : MDK fo ARM
 *
 * Author : 小林
 * Version: 1.00
 * Date   : 2014.4.8
 * Email  : hello14blog@gmail.com
 * Modification: none
 *
 * Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
 *
 * Others: none;
 *
 * Function List:
 *	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
 * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) --
 *向寄存器地址写一个byte的数据
 * 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
 * 4. void WriteDat(unsigned char I2C_Data) -- 写数据
 * 5. void OLED_Init(void) -- OLED屏初始化
 * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
 * 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
 * 8. void OLED_CLS(void) -- 清屏
 * 9. void OLED_ON(void) -- 唤醒
 * 10. void OLED_OFF(void) -- 睡眠
 * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
 *unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
 * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) --
 *显示中文(中文需要先取模，然后放到codetab.h中)
 * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char
 *x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
 *
 * History: none;
 *
 *************************************************************************************/

#include "OLED_I2C.h"
#include "codetab.h"
#include "delay.h"


// 引脚模式变更
void OLED_Iic_Sda_Mode(GPIOMode_TypeDef mode) {
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;        // 10
  GPIO_InitStruct.GPIO_Mode = mode;              // 输出械
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;    // 开漏
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;      // 上拉
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 速度

  GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void OLED_Iic_Start(void) {
  // 输出模式
  OLED_Iic_Sda_Mode(GPIO_Mode_OUT);
  OLED_SCL = 1;
  OLED_SDA_OUT = 1;

  delay_us(5);
  OLED_SDA_OUT = 0;
  delay_us(5);
  OLED_SCL = 0;
}
void OLED_Iic_Stop(void) {
  OLED_Iic_Sda_Mode(GPIO_Mode_OUT); // SDA为输出模式

  OLED_SCL = 0; // 将时钟线和数据线同时置0
  OLED_SDA_OUT = 0;

  delay_us(5);      // 延迟5us
  OLED_SCL = 1;     // 时钟线先置1
  delay_us(5);      // 延迟5us
  OLED_SDA_OUT = 1; // 数据线后置1
}

// 发送一位数据
void OLED_Iic_Send_Ack(u8 ack) {
  // 输出模式
  OLED_Iic_Sda_Mode(GPIO_Mode_OUT);

  OLED_SCL = 0;
  // 时钟为低电平，准备数据(变更)

  if (ack == 1) {
    OLED_SDA_OUT = 1;
  } else {
    OLED_SDA_OUT = 0;
  }

  delay_us(5);
  OLED_SCL = 1;
  delay_us(5);
  OLED_SCL = 0;
}

// 发送一个字节
void OLED_Iic_Send_Byte(u8 txdata) // rxdata=0x87  1 0 0 0 0 1 1 1(先发高位)
{
  u8 i;

  // 输出模式
  OLED_Iic_Sda_Mode(GPIO_Mode_OUT);

  OLED_SCL = 0;
  for (i = 0; i < 8; i++) {
    // 时钟为低电平，准备数据(变更)
    if (txdata & (0x01 << (7 - i))) {
      OLED_SDA_OUT = 1;
    } else {
      OLED_SDA_OUT = 0;
    }

    delay_us(5);
    OLED_SCL = 1;
    delay_us(5);
    OLED_SCL = 0;
  }
}

// 接受一位数据
u8 OLED_Iic_Recv_Ack(void) {
  u8 ack = 0;

  // 输入模式
  OLED_Iic_Sda_Mode(GPIO_Mode_IN);

  OLED_SCL = 0;

  delay_us(5);

  OLED_SCL = 1;

  delay_us(5);
  // 判断引脚电平，获取数据
  if (OLED_SDA_IN == 1) {
    ack = 1;
  } else {
    ack = 0;
  }

  OLED_SCL = 0;

  return ack;
}

// 接受一个字节数据
u8 OLED_Iic_Recv_Byte(void) {
  u8 i, rxdata = 0x00;

  // 输入模式
  OLED_Iic_Sda_Mode(GPIO_Mode_IN);

  OLED_SCL = 0;
  for (i = 0; i < 8; i++) {

    delay_us(5);
    OLED_SCL = 1;
    delay_us(5);
    // 判断引脚电平，获取数据 数据先放高位
    if (OLED_SDA_IN == 1) {
      rxdata |= (0x01 << (7 - i));
    }
    OLED_SCL = 0;
  }

  return rxdata;
}

void I2C_Configuration(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  // 使能GPIO E组时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  // 使能GPIO D组时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10; // 引脚9，引脚10
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;           // 输出
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;          // 推挽
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       // 50MHZ速度
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;            // 上拉
  GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_15; // 引脚1，引脚15
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;           // 输出
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;          // 推挽
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;       // 50MHZ速度
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;            // 上拉
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  // VCC
  PDout(1) = 1;
  // GND
  PDout(15) = 0;

  // 空闲状态
  OLED_SCL = 1;
  OLED_SDA_OUT = 1;
}

void I2C_WriteByte(uint8_t addr, uint8_t data) {
  u8 ack = 0;

  OLED_Iic_Start();

  // 发送设备地址（一个字节）
  OLED_Iic_Send_Byte(OLED_ADDRESS);
  ack = OLED_Iic_Recv_Ack();
  if (ack == 1) {
    printf("send fail1\r\n");
    return;
  }

  // 发送寄存器地址（一个字节）
  OLED_Iic_Send_Byte(addr);
  ack = OLED_Iic_Recv_Ack();
  if (ack == 1) {
    printf("send fail1\r\n");
    return;
  }

  // 发送数据（一个字节）
  OLED_Iic_Send_Byte(data);
  ack = OLED_Iic_Recv_Ack();
  if (ack == 1) {
    printf("send fail1\r\n");
    return;
  }

  OLED_Iic_Stop();
}

void WriteCmd(unsigned char I2C_Command) // 写命令
{
  I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data) // 写数据
{
  I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void) {
  delay_ms(100); // 这里的延时很重要

  WriteCmd(0xAE); // display off
  WriteCmd(0x20); // Set Memory Addressing Mode
  WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing
                  // Mode;10,Page Addressing Mode (RESET);11,Invalid
  WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
  WriteCmd(0xc8); // Set COM Output Scan Direction
  WriteCmd(0x00); //---set low column address
  WriteCmd(0x10); //---set high column address
  WriteCmd(0x40); //--set start line address
  WriteCmd(0x81); //--set contrast control register
  WriteCmd(0xff); // 亮度调节 0x00~0xff
  WriteCmd(0xa1); //--set segment re-map 0 to 127
  WriteCmd(0xa6); //--set normal display
  WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
  WriteCmd(0x3F); //
  WriteCmd(
      0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  WriteCmd(0xd3); //-set display offset
  WriteCmd(0x00); //-not offset
  WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
  WriteCmd(0xf0); //--set divide ratio
  WriteCmd(0xd9); //--set pre-charge period
  WriteCmd(0x22); //
  WriteCmd(0xda); //--set com pins hardware configuration
  WriteCmd(0x12);
  WriteCmd(0xdb); //--set vcomh
  WriteCmd(0x20); // 0x20,0.77xVcc
  WriteCmd(0x8d); //--set DC-DC enable
  WriteCmd(0x14); //
  WriteCmd(0xaf); //--turn on oled panel
}

void OLED_SetPos(unsigned char x, unsigned char y) // 设置起始点坐标
{
  WriteCmd(0xb0 + y);
  WriteCmd(((x & 0xf0) >> 4) | 0x10);
  WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data) // 全屏填充
{
  unsigned char m, n;
  for (m = 0; m < 8; m++) {
    WriteCmd(0xb0 + m); // page0-page1
    WriteCmd(0x00);     // low column start address
    WriteCmd(0x10);     // high column start address
    for (n = 0; n < 128; n++) {
      WriteDat(fill_Data);
    }
  }
}

void OLED_Fill_Radom(int posi_x_start, int posi_x_end, int posi_y_start,
                     int posi_y_end, unsigned char fill_Data) // 任意位置填充
{
  unsigned char m, n;
  for (m = posi_y_start; m < posi_y_end; m++) {
    WriteCmd(0xb0 + m); // page0-page1
    WriteCmd(0x00);     // low column start address
    WriteCmd(0x10);     // high column start address
    for (n = posi_x_start; n < posi_x_end; n++) {
      WriteDat(fill_Data);
    }
  }
}

void OLED_CLS(void) // 清屏
{
  OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : 将OLED从休眠中唤醒
//--------------------------------------------------------------
void OLED_ON(void) {
  WriteCmd(0X8D); // 设置电荷泵
  WriteCmd(0X14); // 开启电荷泵
  WriteCmd(0XAF); // OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void) {
  WriteCmd(0X8D); // 设置电荷泵
  WriteCmd(0X10); // 关闭电荷泵
  WriteCmd(0XAE); // OLED休眠
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y,
// unsigned char ch[], unsigned char TextSize) Calls          : Parameters     :
// x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize --
// 字符大小(1:6*8 ; 2:8*16) Description    :
// 显示codetab.h中的ASCII字符,有6*8和8*16可选择
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
                  unsigned char TextSize, unsigned char Mode) {
  unsigned char c = 0, i = 0, j = 0;
  switch (Mode)
  {
  case 0:
  {
    switch (TextSize) {
    case 1: {
      while (ch[j] != '\0') {
        c = ch[j] - 32;
        if (x > 126) {
          x = 0;
          y++;
        }
        OLED_SetPos(x, y);
        for (i = 0; i < 6; i++)
          WriteDat(F6x8[c][i]);
        x += 6;
        j++;
      }
    } break;
    case 2: {
      while (ch[j] != '\0') {
        c = ch[j] - 32;
        if (x > 120) {
          x = 0;
          y++;
        }
        OLED_SetPos(x, y);
        for (i = 0; i < 8; i++)
          WriteDat(F8X16[c * 16 + i]);
        OLED_SetPos(x, y + 1);
        for (i = 0; i < 8; i++)
          WriteDat(F8X16[c * 16 + i + 8]);
        x += 8;
        j++;
      }
    } break;
    }
  }
    break;
  
  case 1:
  {
      unsigned char s;
      while (ch[j] != '\0')
      {
          c = ch[j] - 32;
          if (x > 120)
          {
              x = 0;
              y++;
          }
          OLED_SetPos(x, y);
          for (i = 0; i < 8; i++)
              WriteDat(F8X16[c * 16 + i + 1264]);
          OLED_SetPos(x, y + 1);
          for (i = 0; i < 8; i++)
              WriteDat(F8X16[c * 16 + i + 8 + 1264]);
          x += 8;
          j++;
    }
  }
  break;
  }
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned
// char N) Calls          : Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7);
// N:汉字在codetab.h中的索引 Description    : 显示codetab.h中的汉字,16*16点阵
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) {
  unsigned char wm = 0;
  unsigned int adder = 32 * N;
  OLED_SetPos(x, y);
  for (wm = 0; wm < 16; wm++) {
    WriteDat(F16x16[adder]);
    adder += 1;
  }
  OLED_SetPos(x, y + 1);
  for (wm = 0; wm < 16; wm++) {
    WriteDat(F16x16[adder]);
    adder += 1;
  }
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned
// char x1,unsigned char y1,unsigned char BMP[]); Calls          : Parameters :
// x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 --
// 起点对角线(结束点)的坐标(x1:1~128,y1:1~8) Description    : 显示BMP位图
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]) {
  unsigned int j = 0;
  unsigned char x, y;

  if (y1 % 8 == 0)
    y = y1 / 8;
  else
    y = y1 / 8 + 1;
  for (y = y0; y < y1; y++) {
    OLED_SetPos(x0, y);
    for (x = x0; x < x1; x++) {
      WriteDat(BMP[j++]);
    }
  }
}
