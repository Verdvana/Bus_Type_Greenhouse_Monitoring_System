/**
  ******************************************************************************
  * @file    bsp_adc.c
  * @author  ãÆ½ğ•
  * @version V1.2
  * @date    2016-4-24
  * @brief   Ä£Êı×ª»»ÅäÖÃÎÄ¼ş
  ******************************************************************************
  * @attention
  * Ê¹ÓÃÁËADC1µÄ12¡¢13Í¨µÀ²É¼¯£¬Í¨¹ıDMA1´«Êä
  ******************************************************************************
  */


#include "bsp_adc.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

__IO uint16_t ADC_ConvertedValue[2];

static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                              //¿ªDMA1Ê±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);     //¿ªADC1ºÍPCÊ±ÖÓ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                       //Ñ¡PC2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   //Ä£ÄâÊäÈëÄ£Ê½
	GPIO_Init(GPIOC, &GPIO_InitStructure);				                                  //Ğ´Èë¼Ä´æÆ÷
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                       //Ñ¡PC3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   //Ä£ÄâÊäÈëÄ£Ê½
	GPIO_Init(GPIOC, &GPIO_InitStructure);				                                  //Ğ´Èë¼Ä´æÆ÷
}


static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
		
	DMA_DeInit(DMA1_Channel1);                                                      //ÅäÖÃDMA1Í¨µÀ1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			              //ADC1µØÖ·
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	              //ÄÚ´æµØÖ·
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	              //ÍâÉèµØÖ·¹Ì¶¨
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				              //ÄÚ´æµØÖ·¹Ì¶¨
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	    //°ë×Ö
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										              //Ñ­»·´«Êä
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);                                                 //DMAÊ¹ÄÜ

	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		                         	//¶ÀÁ¢ADCÄ£Ê½
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				                        //¿ªÆôÉ¨ÃèÄ£Ê½£¬É¨ÃèÄ£Ê½ÓÃÓÚ¶àÍ¨µÀ²É¼¯
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		                        	//¿ªÆôÁ¬Ğø×ª»»Ä£Ê½£¬¼´²»Í£µØ½øĞĞADC×ª»»
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	            //²»Ê¹ÓÃÍâ²¿´¥·¢×ª»»
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	                        //²É¼¯Êı¾İÓÒ¶ÔÆë
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 								                        //Òª×ª»»µÄÍ¨µÀÊıÄ2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                               //ÅäÖÃADCÊ±ÖÓ£¬ÎªPCLK2µÄ8·ÖÆµ£¬¼´9MHz
	 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_55Cycles5);    //ÅäÖÃADC1µÄÍ¨µÀ12Îª55.	5¸ö²ÉÑùÖÜÆÚ£¬ĞòÁĞÎª1
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 2, ADC_SampleTime_55Cycles5);    //ÅäÖÃADC1µÄÍ¨µÀ13Îª55.	5¸ö²ÉÑùÖÜÆÚ£¬ĞòÁĞÎª2
	
	ADC_DMACmd(ADC1, ENABLE);                                                       //ADC1/DMAÊ¹ÄÜ

	ADC_Cmd(ADC1, ENABLE);                                                          //ADC1Ê¹ÄÜ
	
	ADC_ResetCalibration(ADC1);                                                     //¸´Î»Ğ£×¼¼Ä´æÆ÷

	while(ADC_GetResetCalibrationStatus(ADC1));                                     //µÈ´ıĞ£×¼¼Ä´æÆ÷¸´Î»Íê³É
	
	ADC_StartCalibration(ADC1);                                                     //ADCĞ£×¼

	while(ADC_GetCalibrationStatus(ADC1));                                          //µÈ´ıĞ£×¼Íê³É
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                         //ÓÉÓÚÃ»ÓĞ²ÉÓÃÍâ²¿´¥·¢£¬ËùÒÔÊ¹ÓÃÈí¼ş´¥·¢ADC×ª»»
}

void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
