#include "stm32f10x.h"
#include "delay.h"
#include "ds18b20.h"
#include "usart.h"
#include "beep.h"
#include <stdbool.h>

/**************************************************************************************
 * 描  述 : GPIO/USART1初始化配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); 						 


  GPIO_DeInit(GPIOB);	 //将外设GPIOA寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOA);	 //将外设GPIOA寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB , GPIO_Pin_9);   //初始状态，熄灭指示灯LED1
	
}


/**************************************************************************************
 * 描  述 : ESP8266初始化
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void espInit()   
{
	printf("AT+CIPMUX=1\r\n");
	delay_ms(500);
	printf("AT+CIPMUX=1\r\n");
	delay_ms(500);
	printf("AT+CIPSERVER=1,8080\r\n");
	delay_ms(500);
	printf("AT+CIPSERVER=1,8080\r\n");
	delay_ms(500);
}
/**************************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
int main(void)
{
	SystemInit();			//设置系统时钟72MHZ
	USART1_Init();    //初始化配置TIM
	GPIO_Configuration();//初始化温度传感器
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  //关闭USART1接收中断
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(300);	
	printf("AT+CIPSEND=0,11\r\n");
USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //关闭USART1接收中断
	espInit();      //初始化WIFI模块
	BEEP_Init();   //初始化蜂鸣器
	
	while( DS18B20_Init())	               //判断传感器是否激活
	{
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
	}	
	GPIO_SetBits(GPIOB , GPIO_Pin_9); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	while(1)
	{	
		printf("AT+CIPSEND=0,11\r\n");
		if(DS18B20_Get_Temp()>=40)           //当人体温度大于40时，向手机端发送当前人体温度值，并蜂鸣器响以示警告         
		{
			BUZZER_BEEP(26,40);
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  //关闭USART1接收中断
			printf("AT+CIPSEND=0,11\r\n");
			delay_ms(300);
			printf("\r\n %.1f  \r\n",DS18B20_Get_Temp());
			delay_ms(500);	
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
			
		}
	}
}	


/*********************************END FILE********************************************/
