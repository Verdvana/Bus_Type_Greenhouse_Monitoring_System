#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

#define HIGH   1
#define LOW    0

#define DS18B20_CLK     RCC_APB2Periph_GPIOD
#define DS18B20_PIN     GPIO_Pin_13
#define DS18B20_PORT    GPIOD

//´ø²Îºê
#define DS18B20_DATA_OUT(a) if(a)   \
	         GPIO_SetBits(DS18B20_PORT , DS18B20_PIN);   \
                            else    \
			     GPIO_ResetBits(DS18B20_PORT , DS18B20_PIN)

#define DS18B20_DATA_IN()    GPIO_ReadInputDataBit(DS18B20_PORT , DS18B20_PIN)

uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);

#endif /* __DS18B20_H */

														