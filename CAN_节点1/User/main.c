
#include "stm32f10x.h"
#include "bsp_SysTick.h"
//#include "bsp_dht11.h"
#include "bsp_DS18B20.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_it.h"
#include "bsp_led.h"
#include "bsp_adc.h"
#include "bsp_BH1750.h"
#include "bsp_usart1.h"
#include "bsp_buzzer.h"
#include "bsp_dht22.h"
#include "bsp_CAN.h"
#include "bsp_bjdj.h"
#include "bsp_pwm_output.h" 

#define CLI()      __set_PRIMASK(1)		 
#define SEI() __set_PRIMASK(0)	

int W;
int X;
int Y;
int Z;
extern int CCR1;
extern int Q;
int E;

void LCD_Interface(void);
void Data_synthesis(void);
void Auto_Control(void);
void Control(void);

uint8_t C_flag = 0xff;    //CAN 控制指令标志位
extern uint8_t flag;    //CAN接收标志位
extern CanTxMsg TxMessage;    //发送缓冲区
extern CanRxMsg RxMessage;    //接收缓冲区
extern __IO uint16_t ADC_ConvertedValue[2];   //ADC电压值
uint8_t Concentration,Humidity;    //CO2浓度，土壤湿度
//DHT11_Data_TypeDef DHT11_Data;
DHT22_Data_TypeDef DHT22_Data;    //DHT22数据结构体
float temp;    //GY-30光照度



int main(void)
{	 
	 /****************************系统初始化*******************************/
	 SysTick_Init();	 
	 LCD_Init();
	 CLI();
	 SEI();
	 EXTI_Config();  
	 USART1_Config();
	 dir_GPIO_Config();
	 end_GPIO_Config();
	 DJ_GPIO_Config();
	 jdq_GPIO_Config();
	 SysTick_Init();
	  
   LCD_Clear(0, 0, 240, 320, BACKGROUND);	
   LCD_Interface();

	 //DHT11_GPIO_Config(); 
	 DHT22_GPIO_Config();
	 DS18B20_Init();
	 ADC1_Init();
	 LED_GPIO_Config();	
	 BH1750_Init();  	
	 BUZZER_GPIO_Config();	
   USART1_Config();	
	 CAN_Config();	
	 
	 /************************LCD显示&&数据发送****************************/
	 while(1)
	 {	
		  TIM3_PWM_Init();
		  
		  Control();

	    //Read_DHT11(&DHT11_Data);
			Read_DHT22(&DHT22_Data);

			Data_synthesis();
		
		  LCD_Clear(150, 40, 240, 320, BACKGROUND);	
				
			   //LCD_DisNum(150, 40, DHT11_Data.temp_int, RED);
	  	   LCD_DisNum(150, 40, DHT22_Data.temp, RED);
			   LCD_DispStr(170, 40, (uint8_t *)"C", BLUE);
				 //LCD_DisNum(150, 60, DHT11_Data.humi_int, RED);
				 LCD_DisNum(150, 60, DHT22_Data.humi, RED);
				 LCD_DispStr(170, 60, (uint8_t *)"RH", BLUE);
				 LCD_DisNum(150, 100, DS18B20_Get_Temp(), RED);
				 LCD_DispStr(170, 100, (uint8_t *)"C", BLUE);
				 LCD_DisNum(150, 120, Humidity, RED);
				 LCD_DispStr(170, 120, (uint8_t *)"RH", BLUE);
				 LCD_DisNum(150, 160, Concentration, RED);
				 LCD_DispStr(175, 160, (uint8_t *)"PPM", BLUE);
				 LCD_DisNum(150, 200, temp, RED);
				 LCD_DispStr(180, 200, (uint8_t *)"LX", BLUE);
			
     printf("\r\n 123 \r\n");			
		 while( flag == 0xff );
			
			/* 设置要通过CAN发送的信息 */
	    CAN_SetMsg();

	    /* 发送消息 */
	    CAN_Transmit(CAN1, &TxMessage);
			printf("\r\n 999 \r\n");
			
	    flag = 0xff;
			
		Auto_Control();      //自动控制
	  Delay_ms(1000);
	 }
}
	

void LCD_Interface(void)
{
      LCD_DispStr(35, 5, (uint8_t *)"Bus Greenhouse Control System", YELLOW);
	    LCD_DispStr(10, 40, (uint8_t *)"AIR: ", GREEN);	
	    LCD_DispStr(60, 40, (uint8_t *)"Temperature:", BLUE);
	    LCD_DispStr(60, 60, (uint8_t *)"Humidity:", BLUE);	
			LCD_DispStr(10, 100, (uint8_t *)"SOIL:", GREEN);
	    LCD_DispStr(60, 100, (uint8_t *)"Temperature:", BLUE);
	    LCD_DispStr(60, 120, (uint8_t *)"Humidity:", BLUE);	
			LCD_DispStr(10, 160, (uint8_t *)"CO2:", GREEN);	
			LCD_DispStr(60, 160, (uint8_t *)"Concentration:", BLUE);
			LCD_DispStr(10, 200, (uint8_t *)"LIGHT:", GREEN);	
			LCD_DispStr(60, 200, (uint8_t *)"Illumination:", BLUE);
			//delay(0x111111);
}

void Data_synthesis() 
{
			Concentration =(uint8_t)(((float) ADC_ConvertedValue[0]/4096*3.3+0.05)*198);
		  Humidity      =(uint8_t)(((float) ADC_ConvertedValue[1]/4096*3.3)*(-158.73)+266.67);
	
	    if(Humidity>100)
			{
				Humidity=100;
			}
	
      DHT22_Data.temp=(DHT22_Data.temp_h<<8|DHT22_Data.temp_l)/10;
	    DHT22_Data.humi=(DHT22_Data.humi_h<<8|DHT22_Data.humi_l)/10;
	
	    Single_Write_BH1750(0x01);  // power on
		  Single_Write_BH1750(0x10);  // H-resolutionmode
		  Delay_ms(180);
		  Mread();   //连续读出数据，存储在BUF中
		  dis_data = BUF[0];
		  dis_data = (dis_data<<8)+BUF[1];   //合成数据
		  temp = (float)dis_data/1.2;
}

void Auto_Control()
{
  if (Q==0)
		{
			//////////////////////////////////////
			if (Humidity<30)     //turangshi//
	    {
						LED3(1);
				   GPIO_SetBits(GPIOD, GPIO_Pin_12);                                        //置位
			}
	    else if (Humidity>40)
			{
				 LED3(0);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_12);                                        //置位
			}
////////////////////////////////////////
			if (Concentration > 400)     //co2//
	    {
						LED2(1);
				GPIO_SetBits(GPIOD, GPIO_Pin_11);
			}
	    else if (Concentration < 300)
			{
				 LED2(0);
				 GPIO_ResetBits(GPIOD,GPIO_Pin_11);
			}
	////////////////////////////////////
      if(temp<500) 
		  { 
			  if(E==1)
			  {
			   GPIO_SetBits(GPIOG, GPIO_Pin_4);
		     GPIO_SetBits(GPIOG, GPIO_Pin_3);
			   Delay_ms(5000);
			   GPIO_ResetBits(GPIOG, GPIO_Pin_4);
			  }
			  LED4(0);
      
			  if ((temp<200)&(temp>150))     //buguang//
	      {
					LED1(1);
				  CCR1=100;
			  }
	      else if ((temp<150)&(temp>100))
			  {
			    LED1(1);
				  CCR1=333;
			  }		
        else if ((temp<100)&(temp>50))		
        {
		      LED1(1);
			    CCR1=666;
		    }	

        else if((temp<50)&(temp>10))
			  {
			    LED1(1);
			    CCR1=999;
			  }	
				else if(temp<10)
			  {
			    LED1(0);
			    CCR1=0;
			  }	
        else if((temp>200)&(temp<500))			
			  {
			    LED1(0);
			    CCR1=0;
			  }
			 E=0;
		  }
		
		else if(temp>500)
		{
      CCR1=0;
			LED1(0);
		  LED4(1);
			if(E==0)
			{
			  GPIO_SetBits(GPIOG, GPIO_Pin_4);
		    GPIO_ResetBits(GPIOG, GPIO_Pin_3);
			  Delay_ms(5000);
			  GPIO_ResetBits(GPIOG, GPIO_Pin_4);
			}	  
			LED4(0);
			E=1;
		}
	 }
}

void Control()
{

switch(C_flag)
			{
					 case 0:
								switch(W)
								{
									case 0:CCR1=0;Q=0; break;
					        case 1:CCR1=300;Q=1; break;
					        case 2:CCR1=600;Q=1; break;
					        case 3:CCR1=999;Q=1; break;
								}
								W++;
								if (W>3)
									W=0;
								break;
						
						case 1:
					
			      switch(X)
				{
					case 0:GPIO_SetBits(GPIOD, GPIO_Pin_11);Q=1; break;
					case 1:GPIO_ResetBits(GPIOD, GPIO_Pin_11);Q=0; break;
				}
				 
				   X++;
		
			     if (X>1)
				   X=0;
					break;						
				   case 2:
			 
						 
						 switch(Y)
			 {
					   case 0:GPIO_SetBits(GPIOD, GPIO_Pin_12);Q=1; break;
					   case 1:GPIO_ResetBits(GPIOD, GPIO_Pin_12);Q=0; break;
			 }
				 
				     Y++;
		
			       if (Y>1)
				     Y=0;
		        break;
						
									
					case 3:
					switch(Z)
						{
							case 0:GPIO_SetBits(GPIOG, GPIO_Pin_4);GPIO_SetBits(GPIOG, GPIO_Pin_3);Q=1; break;
							case 1:GPIO_ResetBits(GPIOG, GPIO_Pin_4);Q=0; break;
							case 2:GPIO_SetBits(GPIOG, GPIO_Pin_4);GPIO_ResetBits(GPIOG, GPIO_Pin_3);Q=1; break;
							case 3:GPIO_ResetBits(GPIOG, GPIO_Pin_4);Q=0; break;
						}
						Z++;
			
			
		      if (Z>3)
						Z=0;
				   break;
			  }
			C_flag = 0xff;
}
