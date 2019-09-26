#include "stm32f10x.h"
#include "delay.h"
#include "ds18b20.h"
#include "usart.h"
#include "beep.h"
#include <stdbool.h>

/**************************************************************************************
 * ��  �� : GPIO/USART1��ʼ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE); 						 


  GPIO_DeInit(GPIOB);	 //������GPIOA�Ĵ�������Ϊȱʡֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOA);	 //������GPIOA�Ĵ�������Ϊȱʡֵ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //�������
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB , GPIO_Pin_9);   //��ʼ״̬��Ϩ��ָʾ��LED1
	
}


/**************************************************************************************
 * ��  �� : ESP8266��ʼ��
 * ��  �� : ��
 * ����ֵ : ��
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
 * ��  �� : MAIN����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
int main(void)
{
	int a,Res;
	SystemInit();			//����ϵͳʱ��72MHZ
	USART1_Init();    //��ʼ������TIM
	GPIO_Configuration();
	
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(500);	
	espInit();
	BEEP_Init();
	//BUZZER_BEEP(25,40);
	
	while( DS18B20_Init())	               //�жϴ������Ƿ񼤻�
	{
		//BUZZER_BEEP(40,40);
		GPIO_SetBits(GPIOB , GPIO_Pin_9); 
	}	
	GPIO_SetBits(GPIOB , GPIO_Pin_9); 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
	while(1)
	{	
		if(DS18B20_Get_Temp()>=40)           //�������¶ȴ���40ʱ�����ֻ��˷��͵�ǰ�����¶�ֵ��������������ʾ����         
		{
			BUZZER_BEEP(26,100);
			printf("AT+CIPSEND=0,11\r\n");
			delay_ms(300);
			printf("\r\n temperature %.1f\r\n",DS18B20_Get_Temp());
			delay_ms(500);				
		}
		if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != RESET){  //��ѯ���ڴ������־λ
				a =USART_ReceiveData(USART1);//��ȡ���յ�������
				Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
				printf("%c",Res);
				if(Res==43) 
			{
				printf("AT+CIPSEND=0,11\r\n");
				delay_ms(300);
				printf("\r\n temperature %.1f\r\n",DS18B20_Get_Temp());
				delay_ms(500);	
			}
		}
	}
}	
		
		
		//printf("\r\n temperature %.1f\r\n",DS18B20_Get_Temp());				 
//		delay_ms(500);	     	// 1.5s ��ȡһ���¶�ֵ 
 

/*********************************END FILE********************************************/
