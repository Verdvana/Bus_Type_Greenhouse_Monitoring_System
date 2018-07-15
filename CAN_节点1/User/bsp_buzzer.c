/**
  ******************************************************************************
  * @file    bsp_buzzer.c
  * @author  闫金崟
  * @version V1.0
  * @date    2016-4-14
  * @brief   蜂鸣器配置文件
  ******************************************************************************
  * @attention
  * 高电平响
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_buzzer.h"

void BUZZER_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                               //定义结构体
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE);                             //开启E时钟
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //速率50MHz	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	                                         //选中5
	  GPIO_Init(GPIOE, &GPIO_InitStructure);                                             //初始化E5
    GPIO_ResetBits(GPIOE, GPIO_Pin_5);                                                 //复位
}
