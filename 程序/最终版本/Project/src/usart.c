#include "usart.h"
#include "stm32f10x.h"
#include "beep.h"
#include "delay.h"
#include "ds18b20.h"
#include <string.h>
/**************************************************************************************
 * 描  述 : 初始化USART1并配置USART1中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART1_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);  
	
	USART_DeInit(USART1);          //将外设USART1寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 9600;     //设置串口1波特率为19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //发送接收使能
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	USART_Cmd(USART1, ENABLE);      //使能USART1外设
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * 描  述 : 重定向c库函数printf到USART1
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

int is_in(char *s, char c)
{
	while (*s != ':')
	{
		if (*s == c)
		{
			return 1;
		}
		else
		{
			s++;
		}
	}
	return 0;
}

/**************************************************************************************
 * 描  述 : 重定向c库函数scanf到USART1
 * 入  参 : 
 * 返回值 : 无
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/**************************************************************************************
 * 描  述 : USART1全局中断服务
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART1_IRQHandler(void)
{   
	char reserve=0;
	reserve =USART_ReceiveData(USART1);	//读取接收到的数据  		 
	switch (reserve)
	{
		case 'a':
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  //使能USART1接收中断
				printf("AT+CIPSEND=0,11\r\n");                    //AT指令，开启与手机端的通信   
				delay_ms(300);
				printf("\r\n %.1f  \r\n",DS18B20_Get_Temp());     //发送温度数据给app
				delay_ms(500);	
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
			break;
		case 'c':
				BUZZER_BEEP(26,20);
			break;
	}	
}



