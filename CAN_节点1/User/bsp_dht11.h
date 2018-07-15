#ifndef __DHT11_H
#define __DHT11_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

#define DHT11_Clk   RCC_APB2Periph_GPIOE
#define DHT11_Pin   GPIO_Pin_6
#define DHT11_Port  GPIOE

void DHT11_GPIO_Config(void);
static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
static uint8_t Read_Byte(void);

#endif /* __DHT11_H */
