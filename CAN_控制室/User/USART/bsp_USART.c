/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-3-9
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
	
#include "bsp_USART.h"

/**
  * @brief  USART1 GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 USART_InitTypeDef USART_InitStructure;
	
	 /* 配置USART1时钟 */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	 /* 配置USART1的Tx (PA.09) 为复用推挽模式 */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* 配置USART1的Rx (PA.10) 为浮空输入 */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 /* USART1模式配置 */
	 USART_InitStructure.USART_BaudRate = 115200;
	 USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	 USART_InitStructure.USART_StopBits = USART_StopBits_1;
	 USART_InitStructure.USART_Parity = USART_Parity_No;
	 USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	 USART_Init(USART1, &USART_InitStructure);
	 USART_Cmd(USART1, ENABLE);
}

///重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
   /* 发送一个字节数据到USART1 */
	 USART_SendData(USART1, (uint8_t) ch);
	
	 /* 等待发送完毕 */
	 while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	 
	 return (ch);
}

///重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
   /* 等待串口1输入数据 */
	 while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	 return (int)USART_ReceiveData(USART1);
}
