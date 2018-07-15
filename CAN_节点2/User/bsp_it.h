#ifndef __IT_H
#define __IT_H

#include "stm32f10x.h"

void NVIC_Configuration(void);     //NVIC初始化
void EXTI_Config(void);            //中断初始化

#endif /* __IT_H */
