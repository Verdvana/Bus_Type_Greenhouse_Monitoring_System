#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __USART_H */
