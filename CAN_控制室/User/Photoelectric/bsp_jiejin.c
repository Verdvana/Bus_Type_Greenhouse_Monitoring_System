#include "stm32f10x.h"
#include "bsp_jiejin.h"

void E18_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
