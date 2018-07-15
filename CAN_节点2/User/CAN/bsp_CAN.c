/**
  ******************************************************************************
  * @file    bsp_CAN.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-30
  * @brief   CAN功能的相关配置
  ******************************************************************************
  * @attention
  *
  * 仅供参考  （ 收发缓冲区的外部定义  优先级的配置问题 ）
  *
  ******************************************************************************
  */

#include "bsp_CAN.h"
#include "bsp_dht22.h"

uint8_t flag = 0xff;    //接收标志位
CanTxMsg TxMessage;    //发送缓冲区
CanRxMsg RxMessage;    //接收缓冲区

//DHT11_Data_TypeDef DHT11_Data;
extern DHT22_Data_TypeDef DHT22_Data;    //DHT22数据结构体
extern uint8_t Concentration,Humidity;    //CO2浓度，土壤湿度
extern uint8_t tpmsb, tplsb;    //土壤温度
extern uint8_t BUF[8];    //GY-30光照度


/**
  * 函数名：CAN_GPIO_Config
	* 描述：CAN的GPIO配置，上拉输入，推挽输出
	* 输入：无
	* 输出：无
	*/
static void CAN_GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* 外设时钟配置 */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	 
	 /* IO设置 */
	 GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	 
	 /* 配置CAN引脚：RX */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    //上拉输入
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	 /* 配置CAN引脚：TX */
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //复用推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * 函数名：CAN_NVIC_Config
	* 描述：CAN的NVIC配置，第2优先级组，0，0优先级组
	* 输入：无
	* 输出：无
	*/
static void CAN_NVIC_Config(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
	 
	 /* 设置抢占优先级 */
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 
	 /* 中断设置 CAN1_RX0中断 */
	 NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //抢占优先级0
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //子优先级0
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
		 
	 /* 中断设置 CAN1_RX1中断 */
	 NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;    //抢占优先级2
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    //子优先级1
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
}

/**
  * 函数名：CAN_Mode_Config
	* 描述：CAN的模式配置
	* 输入：无
	* 输出：无
	*/
static void CAN_Mode_Config(void)
{
   CAN_InitTypeDef CAN_InitStructure;
	 
	 /********************** CAN通信参数设置 *********************/
	 
	 /* CAN寄存器初始化 */
	 CAN_DeInit(CAN1);
	 CAN_StructInit(&CAN_InitStructure);
	 /* CAN单元初始化 */
	 CAN_InitStructure.CAN_TTCM = DISABLE;    //MAR_TTCM  时间触发通信模式使能  关闭
	 CAN_InitStructure.CAN_ABOM = ENABLE;    //MCR_ABOM  自动离线管理
	 CAN_InitStructure.CAN_AWUM = ENABLE;    //MCR_AWUM  自动唤醒模式
	 CAN_InitStructure.CAN_NART = DISABLE;    //MCR_NART  禁止报文自动重传  DISABLE=自动重传
   CAN_InitStructure.CAN_RFLM = DISABLE;    //MCR_RFLM  接收FIFO锁定模式  DISABLE=溢出时新报文会覆盖原有报文
   CAN_InitStructure.CAN_TXFP = DISABLE;    //MCR_TXFP  发送FIFO优先级  DISABLE=优先级取决于报文标识符
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;    //正常工作模式
   CAN_InitStructure.CAN_SJW = CAN_SJW_2tq;    //BTR_SJW  重新同步跳跃宽度2个时间单元
   CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;    //BTR_TS1  时间段1占用了6个时间单元
   CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;    //BTR_TS2  时间段2占用了3个时间单位	
	 CAN_InitStructure.CAN_Prescaler = 4;    //BTR_BRP 波特率分频器  定义时间单元的时间长度 36/(1+6+3)/4=0.8Mbps
	 CAN_Init(CAN1, &CAN_InitStructure);
}

/**
  * 函数名：CAN_Filter_Config
	* 描述：CAN的过滤器配置
	* 输入：无
	* 输出：无
	*/
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
	 
	 /********************* CAN过滤器0初始化 *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 0;    //过滤器组0
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //工作在标识符屏蔽模式
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //过滤器宽为单个32位
	 
	 /* 使能报文标识符过滤器，扩展ID不是如下就抛弃掉，是的话，存入FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x1C00<<3) & 0XFFFF0000) >> 16;    //要过滤的ID高位
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x1C00<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //要过滤的地位
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //过滤的高16位每位必须匹配
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //过滤的低16位每位必须匹配
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;    //过滤器被关联到FIFO0
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //使能过滤器
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);    //CAN通信中断使能
	
	 
	 /********************* CAN过滤器1初始化 *******************/
	 CAN_FilterInitStructure.CAN_FilterNumber = 1;    //过滤器组0
	 CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;    //工作在标识符屏蔽模式
	 CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;    //过滤器宽为单个32位
	 
	 /* 使能报文标识符过滤器，扩展ID不是如下就抛弃掉，是的话，存入FIFO0 */
	 CAN_FilterInitStructure.CAN_FilterIdHigh = (((uint32_t)0x1010<<3) & 0XFFFF0000) >> 16;    //要过滤的ID高位
	 CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)0x1010<<3) | CAN_ID_EXT | CAN_RTR_DATA) & 0XFFFF;    //要过滤的地位
	 CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0XFFFF;    //过滤的高16位每位必须匹配
	 CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0XFFFF;    //过滤的低16位每位必须匹配
	 CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1;    //过滤器被关联到FIFO1
	 CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;    //使能过滤器
	 CAN_FilterInit(&CAN_FilterInitStructure);
	 CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);    //CAN通信中断使能 
}

/**
  * 函数名：CAN_SetMsg
	* 描述：“主机”的CAN通信报文内容设置
	* 输入：无
	* 输出：无
	*/
void CAN_SetMsg(void)
{
   //Txmessage.StdId = 0x00;
	 TxMessage.ExtId = 0x0314;    //使用的扩展ID
	 TxMessage.IDE = CAN_ID_EXT;    //扩展模式
	 TxMessage.RTR = CAN_RTR_DATA;    //发送的是数据
	 TxMessage.DLC = 8;    //数据长度为8字节
	 
	 TxMessage.Data[0] = (uint8_t)DHT22_Data.temp;
	 TxMessage.Data[1] = (uint8_t)DHT22_Data.humi;
	 TxMessage.Data[2] = tplsb;
	 TxMessage.Data[3] = tpmsb;
	 TxMessage.Data[4] = Concentration;
	 TxMessage.Data[5] = Humidity;
	 TxMessage.Data[6] = BUF[0];
	 TxMessage.Data[7] = BUF[1];
}

/**
  * 函数名：CAN_Config
	* 描述：完整配置CAN功能
	* 输入：无
	* 输出：无
	*/
void CAN_Config(void)
{
   CAN_GPIO_Config();
	 CAN_NVIC_Config();
	 CAN_Mode_Config();
	 CAN_Filter_Config();
}

