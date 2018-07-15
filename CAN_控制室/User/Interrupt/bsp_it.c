#include "stm32f10x.h"
#include "bsp_it.h"

/********************************嵌套向量中断初始化**********************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);                         //中断向量分组2
	
	/* 关闭报警按键中断 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;                        //3线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;               //抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);
	
	
	/* 光电开关报警中断 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;                        //2线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;               //抢占优先级1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);
	
	/* 照明指令 发送按键中断 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;                        //4线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);                                       	//写入寄存器
	
	/* 通风指令 发送按键中断 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;                      //5线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);
	
	/* 灌溉指令 发送按键中断 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                    //13线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);                                         //写入寄存器
	
	/* 卷帘指令 发送按键中断 */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;                    //14线
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                      //响应优先级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         //使能
  NVIC_Init(&NVIC_InitStructure);                                         //写入寄存器
	
	 /* 中断设置 CAN1_RX0中断 */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                      //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	 /* 中断设置 CAN1_RX0中断 */
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;               //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;                      //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
/***********************************外部中断初始化********************************************/

void EXTI_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //开PC引脚和AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF |RCC_APB2Periph_GPIOG,ENABLE); //开PA引脚和PG时钟	
	
	NVIC_Configuration();                                                       //必须在中断初始化函数里调用
	
	/* 关闭报警 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // 上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_Init(GPIOE, &GPIO_InitStructure);	                                    //选引脚PE3

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;                                  //选线3
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);                                             //写入寄存器
  
	/* 光电开关报警 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              //上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_Init(GPIOG, &GPIO_InitStructure);	                                    //选引脚PG2

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource2);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                                  //选线2
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);                                             //写入寄存器
	
	/* 照明 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // 上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //选引脚PA4

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line4;                                  //选线4
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);                                             //写入寄存器
	
	/* 通风 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // 上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //选引脚PA5

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;                                  //选线5
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);                                             //写入寄存器
	
	/* 灌溉 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // 上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
  GPIO_Init(GPIOF, &GPIO_InitStructure);	                                    //选引脚PF13

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOF, GPIO_PinSource13);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line13;                                 //选线13
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);                                             //写入寄存器
	
	/* 卷帘 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	                              // 上拉输入
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);	                                    //选引脚PA14

  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);                 
  EXTI_InitStructure.EXTI_Line = EXTI_Line14;                                 //选线5
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                         //中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //下降沿中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                   //开中断
  EXTI_Init(&EXTI_InitStructure);
}
