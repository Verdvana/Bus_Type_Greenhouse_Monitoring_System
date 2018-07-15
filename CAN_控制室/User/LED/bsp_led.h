#ifndef __LED_H
#define	__LED_H

#include "stm32f10x.h"

#define LED_W(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_4)

#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_4);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_4)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOC,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOC,GPIO_Pin_5)
					
#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)
					
#define LED4(a)	if (a)	\
					GPIO_SetBits(GPIOF,GPIO_Pin_12);\
					else		\
					GPIO_ResetBits(GPIOF,GPIO_Pin_12)
					
#define LED(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_6);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_6)

void LED_GPIO_Config(void);

#endif /* __LED_H */
