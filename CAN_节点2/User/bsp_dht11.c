/**
  ******************************************************************************
  * @file    bsp_dht11.c
  * @author  闫金崟
  * @version V1.0
  * @date    2016-4-16
  * @brief   DHT11配置文件
  ******************************************************************************
  * @attention
  * 上电第一次初始化就TM没成功过，总得复位一下,可能和DHT22一个毛病
  ******************************************************************************
  */

#include "stm32f10x.h"
#include "bsp_dht11.h"

void DHT11_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(DHT11_Clk, ENABLE); 													   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(DHT11_Port, &GPIO_InitStructure);		
  
	  GPIO_SetBits(DHT11_Port, DHT11_Pin);	             //拉高等待    
}

static void DHT11_Mode_IPU(void)                       //输入模式
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT11_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(DHT11_Port, &GPIO_InitStructure);	 
}

static void DHT11_Mode_Out_PP(void)                    //输出模式
{
 	  GPIO_InitTypeDef GPIO_InitStructure;												   
  	GPIO_InitStructure.GPIO_Pin = DHT11_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(DHT11_Port, &GPIO_InitStructure);	 	 
}

/***************************************读位数据************************************/

static uint8_t Read_Byte(void)                                 //读一字节
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	  
		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0);     //拉低开始读，等待高电平

		Delay_us(40);   	                                         //40us后还是高电平为1，变为低电平是0

		if(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1)         //为1
		{
			while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1);   //等待高电平结束

			temp|=(uint8_t)(0x01<<(7-i));                            //把第7-i位置1，MSB先行 
		}
		else	                                                     //为0
		{			   
			temp&=(uint8_t)~(0x01<<(7-i));                           //把第7-i位置0，MSB先行
		}
	}
	return temp;
}

/*****************************************读字数据*************************************/

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{  
	DHT11_Mode_Out_PP();                 //输出模式

	GPIO_ResetBits(DHT11_Port,DHT11_Pin);    
	Delay_ms(18);                        //拉低18ms

	GPIO_SetBits(DHT11_Port,DHT11_Pin); 
  Delay_us(30);                        //拉高30us

	DHT11_Mode_IPU();                    //输入模式

 if(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0)               //判断DHT11响应
	{
		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==0);         //等待拉低

		while(GPIO_ReadInputDataBit(DHT11_Port,DHT11_Pin)==1);         //等待拉高

		/*开始接收数据*/   
		DHT11_Data->humi_int= Read_Byte();

		DHT11_Data->humi_deci= Read_Byte();

		DHT11_Data->temp_int= Read_Byte();

		DHT11_Data->temp_deci= Read_Byte();

		DHT11_Data->check_sum= Read_Byte();
    /*接收数据完成*/ 

		DHT11_Mode_Out_PP();        
		GPIO_SetBits(DHT11_Port,DHT11_Pin); 

		if(DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int+ DHT11_Data->temp_deci)//校验
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}
