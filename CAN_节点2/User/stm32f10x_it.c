/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_led.h"
#include "bsp_SysTick.h"
#include "bsp_it.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_buzzer.h"
#include "bsp_usart1.h"
#include "bsp_pwm_output.h" 
#include "bsp_CAN.h"
 int A=0;
 int B=0;
 int C=0;
 int D=0;
 int Q=0;
extern void TimingDelay_Decrement(void);

extern uint8_t C_flag;    //CAN 控制指令标志位
extern uint8_t flag;    //CAN接收标志位
extern CanTxMsg TxMessage;    //发送缓冲区
extern CanRxMsg RxMessage;    //接收缓冲区


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
		TimingDelay_Decrement();	
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
   /* 从邮箱读出报文 */
	 CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	 
	 /* 比较ID和数据是否为 0x1314 及 DCBA */
	 if( (RxMessage.ExtId == 0x1C00) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 0) )
	    flag = 0;    //接收成功
	else
		  flag = 0xff;    //接收失败
}

void CAN1_RX1_IRQHandler(void)
{
   /* 从邮箱读出报文 */
	 CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
	 
	 /* 比较ID是否为 0x1010 */
	 if( (RxMessage.ExtId == 0x1010) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 1) && (RxMessage.Data[0] == 0x00))
	    C_flag = 0;    //照明指令
	 
	 else if( (RxMessage.ExtId == 0x1010) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 1) && (RxMessage.Data[0] == 0x01))
		  C_flag = 1;    //通风指令
	 
	 else if( (RxMessage.ExtId == 0x1010) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 1) && (RxMessage.Data[0] == 0x02))
		  C_flag = 2;    //灌溉指令
	 
	 else if( (RxMessage.ExtId == 0x1010) && (RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 1) && (RxMessage.Data[0] == 0x03))
		  C_flag = 3;    //卷帘指令
	 else
		  C_flag = 0xff;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
int l=0,k;
float m=0.5;

void delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
} 



void EXTI2_IRQHandler(void)
{
	LED_GPIO_Config();
  LED1(1);
	if(EXTI_GetITStatus(EXTI_Line2) != RESET) //2线产生中断(接近开关打开)
	{ 	
		k=1;
		LCD_DispStr(150, 240, (uint8_t *)"WARNING", RED);
    while(k)
		{
    	LED2(0);//亮
			BUZZER(1);
			delay(1000000);
			LED2(1);//灭
			BUZZER(0);
			delay(1000000);
		 }
		printf("\r\n WARNING \r\n");
		EXTI_ClearITPendingBit(EXTI_Line2);  		//清除中断标志位
	}  
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET) //0线产生中断(按键按下)
	{ 
    k=0;
		EXTI_ClearITPendingBit(EXTI_Line0);  		//清除中断标志位
	}  
}

extern  	int CCR1; 
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	{ 
     switch(D)
				{
					case 0:GPIO_SetBits(GPIOD, GPIO_Pin_11);Q=1; break;
					case 1:GPIO_ResetBits(GPIOD, GPIO_Pin_11);Q=0; break;
				}
				 
				D++;
		
			 if (D>1)
				 
		{
			D=0;
		}
	EXTI_ClearITPendingBit(EXTI_Line5);  		
	 }
 }
void EXTI4_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET) //4线产生中断，按键按下
		{
	      
			
			   switch(A)
				{
					case 0:CCR1=0;Q=0; break;
					case 1:CCR1=300;Q=1; break;
					case 2:CCR1=600;Q=1; break;
					case 3:CCR1=999;Q=1; break;
				}
				  A++;
			
		  if (A>3)
	
			{
						A=0;
			
			}
			printf("%d\n",CCR1);
		EXTI_ClearITPendingBit(EXTI_Line4);  		//清除中断标志位		
			
		
 }
	}

	
	
	void EXTI15_10_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line14) != RESET) //14线产生中断
	{ 
        
			
			  switch(B)
				{
					case 0:GPIO_SetBits(GPIOG, GPIO_Pin_4);GPIO_SetBits(GPIOG, GPIO_Pin_3);Q=1; break;
					case 1:GPIO_ResetBits(GPIOG, GPIO_Pin_4);Q=0; break;
					case 2:GPIO_SetBits(GPIOG, GPIO_Pin_4);GPIO_ResetBits(GPIOG, GPIO_Pin_3);Q=1; break;
					case 3:GPIO_ResetBits(GPIOG, GPIO_Pin_4);Q=0; break;
				}
				  B++;
			
			
		  if (B>3)
	
			{
						B=0;
			
			}
		
		EXTI_ClearITPendingBit(EXTI_Line14);  		//清除中断标志位		
		}
		
		
		
  if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
	{ 
     switch(C)
				{
					case 0:GPIO_SetBits(GPIOD, GPIO_Pin_12);Q=1; break;
					case 1:GPIO_ResetBits(GPIOD, GPIO_Pin_12);Q=0; break;
				}
				 
				C++;
		
			 if (C>1)
				 
		{
			C=0;
		}
	EXTI_ClearITPendingBit(EXTI_Line13);  		
	 }			
		
 }
	

	