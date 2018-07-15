/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-20
  * @brief   SysTick 系统滴答时钟1us中断函数库,中断时间可自由配置，
  *          常用的有 1us 10us 1ms 中断。     
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
	
#include "bsp_SysTick.h"

static __IO uint32_t TimingDelay;

/**
  * @brief  启动系统滴答定时器 SysTick 
	* @param  无
	* @retval 无
	*/
void SysTick_Init(void)
{
   /**
	   * SystemFrequency / 1000     1ms中断一次
	   * SystemFrequency / 100000   10us中断一次 
	   * SystemFrequency / 1000000  1us中断一次
	   */
	 if(SysTick_Config(SystemCoreClock / 1000000))
	 {
      /* 若返回1 则证明超出计数最大值 报错 */
		  while(1);
   }
	 // 关闭滴答定时器
	 SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  us延时程序，1us为一个单位
  * @param  
  *   @arg  nTime: Delay_us();
  * @retval 无
  */
void Delay_us(__IO uint32_t nTime)
{
   TimingDelay = nTime;
	 
	 // 使能滴答定时器
	 SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	 
	 while(TimingDelay != 0);
}

/**
  * @brief  获取节拍程序
  * @param  无
  * @retval 无
  * @attention  在SysTick中断函数 SysTick_Handler()调用
  */
void TimingDelay_Decrement(void)
{
   if(TimingDelay != 0x00)
	 {
      TimingDelay--;
   }
}
