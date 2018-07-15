#include "bsp_SysTick.h"

static __IO u32 TimingDelay;
 
void SysTick_Init(void)
{
	if (SysTick_Config(SystemCoreClock / 1000000))
	{  
		while (1);
	}
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;   // 关闭滴答定时器  
}


void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	 // 使能滴答定时器  

	while(TimingDelay != 0);
}

/*****************************************************************************************/

void TimingDelay_Decrement(void)               //需要添加到中断服务SysTick_Handler中
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}
