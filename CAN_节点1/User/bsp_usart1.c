 #include "bsp_usart1.h"
 
 /************************************************USART1初始化********************************************/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);   //开USART1和PA时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                       //PA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                               //速率50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                      //PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                           //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;                                    //波特率=115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长=8bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //停止位=1
	USART_InitStructure.USART_Parity = USART_Parity_No ;                            //奇偶校验关闭
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流关闭
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //串口配置=双线全双工
	USART_Init(USART1, &USART_InitStructure);                                       //写入寄存器
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //开启接收中断
//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);	                                  //怎么没有开启发送中断？
	USART_Cmd(USART1, ENABLE);                                                      //打开USART1
}



/*******************************************重定向c库函数printf到USART1****************************************/
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t) ch);		// 发送一个字节数据到USART1

		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//等待发送完毕
	
		return (ch);
}

/*******************************************重定向c库函数scanf到USART1*****************************************/
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);//等待串口1输入数据

		return (int)USART_ReceiveData(USART1);
}
