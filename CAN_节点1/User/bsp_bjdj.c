#include "stm32f10x.h"
#include "bsp_bjdj.h"

void dir_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //定义结构体                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE); //定义时钟    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //频率50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	   //选中引脚                                 
	
		GPIO_Init(GPIOG, &GPIO_InitStructure);   //初始化G3                                       


}

void end_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //定义结构体                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE); //定义时钟    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //频率50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	   //选中引脚                                 
	
		GPIO_Init(GPIOG, &GPIO_InitStructure);   //初始化G4                                         

  
}




void jdq_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;                                               //定义结构体

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);        //开启D时钟
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                   //推挽输出
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                                  //速率50MHz	
	
/////////////////////////////选引脚//////////////////////////////////////////////
	

					
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                                          //选中12

		GPIO_Init(GPIOD, &GPIO_InitStructure);	                                           //初始化 D12

		GPIO_SetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_12);                                        //置位
}


void DJ_GPIO_Config(void)
{		
	  GPIO_InitTypeDef GPIO_InitStructure;     //定义结构体                                          

		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD,ENABLE); //定义时钟    
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
	  
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //频率50hz                               


GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	   //选中引脚                                 
	
		GPIO_Init(GPIOD, &GPIO_InitStructure);   //初始化                                       
}
