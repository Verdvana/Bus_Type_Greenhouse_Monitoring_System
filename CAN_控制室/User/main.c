/**
  ******************************************************************************
  * @file    main.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-30
  * @brief   CAN总线通信（管理室）
  ******************************************************************************
  * @attention
  *
  * 仅供参考
  *
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_CAN.h"
#include "bsp_USART.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_buzzer.h"
#include "bsp_it.h"

#define CLI()      __set_PRIMASK(1)		 
#define SEI() __set_PRIMASK(0)

void Data_Processing(void);

uint16_t dis_data;
short s_tem;
float Concentration,Humidity,temp,f_tem;

extern uint8_t flag;
extern CanTxMsg TxMessage;    //发送缓冲区   
extern CanRxMsg RxMessage;    //接收缓冲区

/**
  * @brief  主函数
	* @param  无
	* @retval 无
	*/	
int main(void)
{ 
	 /* 系统定时器初始化 */
	 SysTick_Init();
	 
	 CLI();
	 SEI();
	 EXTI_Config(); 
	 LED_GPIO_Config();
	
   /* 初始化串口 */
	 USART1_Config();
	 BUZZER_GPIO_Config();	
	
	 /* 配置CAN接口 */
	 CAN_Config();
	 
	 printf("\r\n 啦啦啦 \r\n");
	 
	 while(1)
	 {
		  /* 轮流发送请求报文 */
	    CAN_SetMsg();
			
	    /* 发送消息 */
	    CAN_Transmit(CAN1, &TxMessage);
		  printf("\r\n 000 \r\n");
		  
	    /* 等待节点端的信息 */
	    while(flag == 0xff);		 
		  
	    if(flag == 0)
	    {
				 Data_Processing();    //对接收的数据进行处理转换
	
         printf("\r\n “节点1” 返回的数据 \r\n");
	       printf("\r\n 空气温度：%d℃ \r\n 空气湿度：%d％RH \r\n", RxMessage.Data[0], RxMessage.Data[1]);
				 printf("\r\n 土壤温度：%f℃ \r\n 土壤湿度：%d％RH \r\n", f_tem, RxMessage.Data[5]);
				 printf("\r\n CO2浓度：%dPPM \r\n 光照度：%f％LX \r\n", RxMessage.Data[4], temp);
				 printf("\r\n \r\n");;
      }

	    else if(flag == 1)
	    {

				 Data_Processing();    //对接收的数据进行处理转换 
				 
         printf("\r\n “节点2” 返回的数据 \r\n");
	       printf("\r\n 空气温度：%d℃ \r\n 空气湿度：%d％RH \r\n", RxMessage.Data[0], RxMessage.Data[1]);
				 printf("\r\n 土壤温度：%f℃ \r\n 土壤湿度：%d％RH \r\n", f_tem, RxMessage.Data[5]);
				 printf("\r\n CO2浓度：%dPPM \r\n 光照度：%f％LX \r\n", RxMessage.Data[4], temp);
				 printf("\r\n \r\n");;
      }
			
			flag = 0xff;
			Delay_ms(1000); 
		}
	  
}

void Data_Processing(void)
{
	 /* 土壤温度 */
   s_tem = RxMessage.Data[3] << 8;
   s_tem = s_tem | RxMessage.Data[2];
   f_tem = s_tem * 0.0625;
	 
		
   /* 光照度 */	
	 dis_data = RxMessage.Data[6];
	 dis_data = (dis_data<<8)+RxMessage.Data[7];  
	 temp = (float)dis_data/1.2;
}
