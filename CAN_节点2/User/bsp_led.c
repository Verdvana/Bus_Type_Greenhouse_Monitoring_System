/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  闫金崟
  * @version V1.2
  * @date    2016-4-14
  * @brief   LED灯配置文件
  ******************************************************************************
  * @attention
  * 高电平点亮
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_led.h"

void LED_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;                                               //定义结构体
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF|
	                          RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|
	                                               RCC_APB2Periph_GPIOA, ENABLE);        //开启ABCEF时钟
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //速率50MHz	
	
/**********************************************选引脚*********************************************/
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	                                         //选中6
		GPIO_Init(GPIOA, &GPIO_InitStructure);                                             //初始化 A6													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                                          //选中0
		GPIO_Init(GPIOB, &GPIO_InitStructure);                                             //初始化 B0									   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;                               //选中4、5
		GPIO_Init(GPIOC, &GPIO_InitStructure);	                                           //初始化 C4、C5
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                                          //选中4
		GPIO_Init(GPIOE, &GPIO_InitStructure);	                                           //初始化 E4
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                         //选中12
		GPIO_Init(GPIOF, &GPIO_InitStructure);                                             //初始化 F12
		
	  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_0); 
		GPIO_ResetBits(GPIOC, GPIO_Pin_4|GPIO_Pin_5);
		GPIO_ResetBits(GPIOE, GPIO_Pin_4); 
		GPIO_ResetBits(GPIOF, GPIO_Pin_12);                                                //複位
}
