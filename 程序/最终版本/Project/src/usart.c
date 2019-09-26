#include "usart.h"
#include "stm32f10x.h"
#include "beep.h"
#include "delay.h"
#include "ds18b20.h"
#include <string.h>
/**************************************************************************************
 * ��  �� : ��ʼ��USART1������USART1�ж����ȼ�
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART1_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);  
	
	USART_DeInit(USART1);          //������USART1�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 9600;     //���ô���1������Ϊ19200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //���ͽ���ʹ��
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
	USART_Cmd(USART1, ENABLE);      //ʹ��USART1����
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************************
 * ��  �� : �ض���c�⺯��printf��USART1
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
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
 * ��  �� : �ض���c�⺯��scanf��USART1
 * ��  �� : 
 * ����ֵ : ��
 **************************************************************************************/
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}



/**************************************************************************************
 * ��  �� : USART1ȫ���жϷ���
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART1_IRQHandler(void)
{   
	char reserve=0;
	reserve =USART_ReceiveData(USART1);	//��ȡ���յ�������  		 
	switch (reserve)
	{
		case 'a':
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  //ʹ��USART1�����ж�
				printf("AT+CIPSEND=0,11\r\n");                    //ATָ��������ֻ��˵�ͨ��   
				delay_ms(300);
				printf("\r\n %.1f  \r\n",DS18B20_Get_Temp());     //�����¶����ݸ�app
				delay_ms(500);	
				USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
			break;
		case 'c':
				BUZZER_BEEP(26,20);
			break;
	}	
}



