/**
  ******************************************************************************
  * @file    bsp_BH1750.c
  * @author  Sweet
  * @version V1.0
  * @date    2016-4-20
  * @brief   BH1750芯片应用
  ******************************************************************************
  * @attention
  * 仅供参考 （在发送字节及接收字节处，仅调用了接受应答函数，而并无判断应答情况或是否有应答）
  *          （连续读取函数，为什么会读三个字节）
  ******************************************************************************
  */

#include "bsp_BH1750.h"
#include "bsp_SysTick.h"

uint8_t BUF[8];  //接收数据缓存区
uint16_t dis_data;   //变量
uint16_t mcy;   //进位标志位

/*
 * 函数名：GPIO_Config
 *   描述：配置BH1750用到的I/O口
 *   输入：无
 *   输出：无
 */
void GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* 开启GPIO的时钟 */
	 RCC_APB2PeriphClockCmd(BH1750_CLK, ENABLE);
	 
	 /* 配置SDA与SCL为推挽输出 */
	 GPIO_InitStructure.GPIO_Pin = SDA | SCL;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure); 
}

/*
 * 函数名：BH1750_Mode_IPU
 * 描述  ：使BH1750-SDA引脚变为上拉输入模式
 * 输入  ：无
 * 输出  ：无
 */
static void BH1750_Mode_IPU(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* 配置为上拉输入 */
	 GPIO_InitStructure.GPIO_Pin = SDA;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：BH1750_Mode_Out_PP
 * 描述  ：使BH1750-SDA引脚变为推挽输出模式
 * 输入  ：无
 * 输出  ：无
 */
static void BH1750_Mode_Out_PP(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* 配置为推挽输出 */
	 GPIO_InitStructure.GPIO_Pin = SDA;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	 GPIO_Init(BH1750_PORT, &GPIO_InitStructure);   
}

/*
 * 函数名：BH1750_Start
 *   描述：开始信号
 *   输入：无
 *   输出：无
 */
void BH1750_Start(void)
{
   GPIO_SetBits(BH1750_PORT, SDA);    //拉高数据线
	 GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
	 Delay_us(5);    //延时
	 GPIO_ResetBits(BH1750_PORT, SDA);    //产生下降沿
	 Delay_us(5);    //延时	
	 GPIO_ResetBits(BH1750_PORT, SCL);    //拉低时钟线  
   Delay_us(5);    //延时		
}

/*
 * 函数名：BH1750_Stop
 *   描述：停止信号
 *   输入：无
 *   输出：无
 */
void BH1750_Stop(void)
{
   GPIO_ResetBits(BH1750_PORT, SDA);    //拉低数据线
	 GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
	 Delay_us(5);    //延时	
	 GPIO_SetBits(BH1750_PORT, SDA);    //产生上升沿
	 Delay_us(5);    //延时	
}

/*
 * 函数名：BH1750_SendACK
 *   描述：发送应答信号
 *   输入：ACK(0：ACK   1：NAK)
 *   输出：无
 */
void BH1750_SendACK(int ACK)
{
   BH1750_Mode_Out_PP();
	
	 if(ACK == 1)
		      GPIO_SetBits(BH1750_PORT, SDA);
	 else if(ACK == 0)
		      GPIO_ResetBits(BH1750_PORT, SDA);
	 else
		      return;
	 
	 GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
	 Delay_us(5);    //延时	
	 GPIO_ResetBits(BH1750_PORT, SCL);    //拉低时钟线 
	 Delay_us(5);    //延时	
}

/*
 * 函数名：BH1750_RecvACK
 *   描述：接收应答信号
 *   输入：无
 *   输出：mcy
 */
int BH1750_RecvACK()
{
   BH1750_Mode_IPU();
	
	 GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
	 Delay_us(5);    //延时	
	 
	/* 读应答信号 */ 
	if(GPIO_ReadInputDataBit(BH1750_PORT, SDA) == Bit_SET)   
		 mcy = 1;
	 else
		 mcy = 0; 
	 
	 GPIO_ResetBits(BH1750_PORT, SCL);    //拉低时钟线 
	 Delay_us(5);    //延时	
	 
	 BH1750_Mode_Out_PP();
	 
	 return mcy;
}

/*
 * 函数名：BH1750_SendByte
 *   描述：向IIC总线发送一个字节的数据
 *   输入：data
 *   输出：无
 */
void BH1750_SendByte(uint8_t data)
{
   uint8_t i;
	
	 for(i=0;i<8;i++)
	 {
      if(0x80 & data)
				GPIO_SetBits(BH1750_PORT, SDA);
			else
				GPIO_ResetBits(BH1750_PORT, SDA);
			
			data <<= 1;
			
			GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
	    Delay_us(5);    //延时	
	    GPIO_ResetBits(BH1750_PORT, SCL);    //拉低时钟线 
	    Delay_us(5);    //延时	
   }
	 BH1750_RecvACK();
}

/*
 * 函数名：BH1750_RecvByte
 *   描述：从IIC总线接收一个字节的数据
 *   输入：无
 *   输出：data
 */
uint8_t BH1750_RecvByte(void)
{
   uint8_t i, bit;
	 uint8_t data = 0;
	
	 BH1750_Mode_IPU();
	 
	 GPIO_SetBits(BH1750_PORT, SDA);    //使能内部上拉，准备读取数据
	 
	 for(i=0;i<8;i++)   //8位计数器
	 {
      data <<= 1;
		  GPIO_SetBits(BH1750_PORT, SCL);    //拉高时钟线
      Delay_us(5);    //延时	
      
      if(GPIO_ReadInputDataBit(BH1750_PORT, SDA) == Bit_SET)
        bit = 0x01;
      else
        bit = 0x00;
      
      data |= bit;    //读数据		
      
      GPIO_ResetBits(BH1750_PORT, SCL);    //拉低时钟线 
	    Delay_us(5);    //延时		
   }
	 BH1750_Mode_Out_PP();
	 
	 return data;
}

/*
 * 函数名：Single_Write_BH1750
 *   描述：单个写入数据
 *   输入：REG_Address
 *   输出：无
 */
void Single_Write_BH1750(uint8_t REG_Address)
{
   BH1750_Start();    //发送起始信号
	 BH1750_SendByte(SlaveAddress);    //发送设备地址+写信号
	 BH1750_SendByte(REG_Address);    //内部寄存器地址
	 //BH1750_SendByte(REG_data);    //内部寄存器数据
	 BH1750_Stop();    //发送停止信号
}

/*
 * 函数名：BH1750_Init
 *   描述：初始化BH1750  （根据需要修改）
 *   输入：无
 *   输出：无
 */
void BH1750_Init(void)
{
	 GPIO_Config();
	 Single_Write_BH1750(0x01); 	
}

/*
 * 函数名：Mread
 *   描述：连续读出BH1750内部数据
 *   输入：无
 *   输出：无
 */
void Mread(void)
{
   uint8_t i;
	 
	 BH1750_Start();    //发送起始信号
	 BH1750_SendByte(SlaveAddress+1);    //发送设备地址+读信号
	 
	 /* 连续读取6个地址数据，存储在BUF中 */
	 for(i=0;i<3;i++)    
	 {
      BUF[i] = BH1750_RecvByte();   //BUF[0]存储0x32地址中的数据
		  if(i == 3)
         BH1750_SendACK(1);    //最后一个数据需要回NOACK
			else
				 BH1750_SendACK(0);    //回应ACK
   }
	 BH1750_Stop();    //发送停止信号
	 Delay_ms(5);
}
