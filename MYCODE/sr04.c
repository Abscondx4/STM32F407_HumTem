#include "sr04.h"

/*
STM32				超声波
PA2输出				Trig
PA3输入				echo
*/
void Sr04_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  // 1.使能时钟 GPIO
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  // 2.配置 PA2 输出
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;         // 引脚
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     // 输出模式
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    // 输出类型为推挽
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;    // 下拉
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz; // 速度

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // PA3输入
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;         // 引脚
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;      // 输入模式
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;    // 下拉
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz; // 速度

  GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 定时器要初始化 用于计数
  // 2.使能定时器时钟 42MHZ X 2 = 84MHZ
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // 3.初始化定时器，配置ARR,PSC。
  TIM_TimeBaseInitStruct.TIM_Prescaler =
      84 - 1; // 预分频系数  1MHZ  -->1秒钟 计数 1000 000
  TIM_TimeBaseInitStruct.TIM_CounterMode =
      TIM_CounterMode_Up; // 计数方式  递增/向上计数
  TIM_TimeBaseInitStruct.TIM_Period = 50000 - 1; // 自动重装载寄存器的值 0~65535
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频因子
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

  // 6.不使能定时器
  TIM_Cmd(TIM3, DISABLE);
}

int Get_Sr04_Value() {
  int t = 0;
  // PA2输出低电平
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);
  delay_us(8);
  // PA2输出高电平
  GPIO_SetBits(GPIOA, GPIO_Pin_2);
  delay_us(20);

  // PA2输出低电平
  GPIO_ResetBits(GPIOA, GPIO_Pin_2);

  // 设置定时器的CNT为0
  TIM3->CNT = 0;

  // PA3等待高电平到来，参考按键松开代码 while( 读电平  );
  while (1) {
    uint8_t ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    if (ret == Bit_SET)
      break;

    delay_us(5);
    t++;

    if (t >= 1000) // 5000us   5ms 如果一直收不到高电平也要退出
    {
      return 0;
    }
  }
  // 使能定时器开始计数
  TIM_Cmd(TIM3, ENABLE);

  // PA3等待低电平到来，while( 读电平  );
  t = 0;
  while (1) {
    uint8_t ret = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    if (ret == RESET) // 低电平就退出
    {
      break;
    }
    delay_us(5);
    t++;

    if (t >= 1000) {
      return 0;
    }
  }
  // 获取定时器CNT值，从而得到高电平持续时间    temp = TIM3->CNT;
  int temp = TIM3->CNT;

  // 关闭定时器
  TIM_Cmd(TIM3, DISABLE);

  // 通过公式计算出超声波测量距离  厘米
  return temp / 58;
}
