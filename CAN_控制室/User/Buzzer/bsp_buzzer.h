#ifndef __BUZZER_H
#define	__BUZZER_H

#include "stm32f10x.h"

#define BUZZER(a)	if (a)	\
					GPIO_SetBits(GPIOE,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOE,GPIO_Pin_5)
					
void BUZZER_GPIO_Config(void);


#endif /* __BUZZER_H */
