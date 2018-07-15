#ifndef __BH1750_H
#define __BH1750_H

#include "stm32f10x.h"
#include "bsp_SysTick.h"

#define BH1750_PORT    GPIOF
#define BH1750_CLK     RCC_APB2Periph_GPIOF
#define SDA    GPIO_Pin_0
#define SCL    GPIO_Pin_1
#define SlaveAddress    0x46  //定义器件在IIC总线中的地址，根据ALT ADDRESS地址引脚不同修改 */ 
/* ALT ADDRESSS 引脚接地时地址为0x46，接电源时为0xB8*/

extern uint8_t BUF[8];  //接收数据缓存区
extern uint16_t dis_data;   //变量
extern uint16_t mcy;   //进位标志位

void BH1750_Init(void);
void Conversion(uint8_t temp_data);
uint8_t Single_Read_BH1750(uint8_t REG_Address);   //单个读取内部寄存器数据
void Single_Write_BH1750(uint8_t REG_Address);   //单个写入数据
void Mread(void);   //连续读取内部寄存器数据
void GPIO_Config(void);

#endif /* __BH1750_H */
