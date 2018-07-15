/**
  ******************************************************************************
  * @file    bsp_dht22.c
  * @author  闫金崟
  * @version V1.0
  * @date    2016-5-6
  * @brief   DHT22配置文件
  ******************************************************************************
  * @attention
  * 有两秒的不稳定时间
  ******************************************************************************
  */

#include "bsp_dht22.h"

/********************************初始化*****************************/
void DHT22_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT22_Clk, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = DHT22_Pin;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(DHT22_Port, &GPIO_InitStructure);		  
	
	  GPIO_SetBits(DHT22_Port, DHT22_Pin);	    //拉高等待    
}


static void DHT22_Mode_IPU(void)         //输入模式
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	  GPIO_InitStructure.GPIO_Pin = DHT22_Pin;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; 
	  GPIO_Init(DHT22_Port, &GPIO_InitStructure);	 
}

static void DHT22_Mode_Out_PP(void)       //输出模式
{
 	  GPIO_InitTypeDef GPIO_InitStructure;												   
  	GPIO_InitStructure.GPIO_Pin = DHT22_Pin;	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(DHT22_Port, &GPIO_InitStructure);	 	 
}

/***************************************读位数据************************************/

static uint8_t Read_Byte(void)                    //读一字节
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	  
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0);     //拉低开始读，等待高电平
		Delay_us(40);   	                                      //40us后还是高电平为1，变为低电平是0
		if(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1)         //为1
		{
			while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1);   //等待高电平结束
			temp|=(uint8_t)(0x01<<(7-i));                         //把第7-i位置1，MSB先行 
		}
		else	                                                  //为0
		{			   
			temp&=(uint8_t)~(0x01<<(7-i));                        //把第7-i位置0，MSB先行
		}
	}
	return temp;
}

/*****************************************读字数据*************************************/

uint8_t Read_DHT22(DHT22_Data_TypeDef *DHT22_Data)
{  
	DHT22_Mode_Out_PP();                 //输出模式

	GPIO_ResetBits(DHT22_Port,DHT22_Pin);    
	Delay_ms(1);                        //拉低1ms

	GPIO_SetBits(DHT22_Port,DHT22_Pin); 
  Delay_us(30);                        //拉高30us

	DHT22_Mode_IPU();                    //输入模式
	  
	if(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0)               //判断DHT22响应
	{
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==0);         //等待拉低
		while(GPIO_ReadInputDataBit(DHT22_Port,DHT22_Pin)==1);         //等待拉高

		/*开始接收数据*/   
		DHT22_Data->humi_h= Read_Byte();

		DHT22_Data->humi_l= Read_Byte();

		DHT22_Data->temp_h= Read_Byte();

		DHT22_Data->temp_l= Read_Byte();

		DHT22_Data->check_sum= Read_Byte();
    /*接收数据完成*/ 
		
		DHT22_Mode_Out_PP();        
		GPIO_SetBits(DHT22_Port,DHT22_Pin); 
		
		if(DHT22_Data->check_sum == DHT22_Data->humi_h + DHT22_Data->humi_l + DHT22_Data->temp_h+ DHT22_Data->temp_l)//校验
			return SUCCESS;
		else 
			return ERROR;
	}
	else
	{		
		return ERROR;
	}   
}
