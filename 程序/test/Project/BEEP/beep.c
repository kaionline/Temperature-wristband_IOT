#include "beep.h"


void BEEP_Init(void){ //LED�ƵĽӿڳ�ʼ��
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = BEEP; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //ѡ��IO�ӿڹ�����ʽ       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
	GPIO_Init(LEDPORT, &GPIO_InitStructure);			
	GPIO_SetBits(GPIOB,BEEP);    //�رշ�����(����Ϊ�ߵ�ƽ)
}


void BUZZER_BEEP(int a,int b)
{
		while(a--)
	{
		GPIO_ResetBits(GPIOB,BEEP);;   //��������(����Ϊ�͵�ƽ)
		delay_ms(b);
		 GPIO_SetBits(GPIOB,BEEP);
		delay_ms(b);
	}
	GPIO_SetBits(GPIOB,BEEP);    //�رշ�����(����Ϊ�ߵ�ƽ)
	
}
 



/*
ѡ��IO�ӿڹ�����ʽ��
GPIO_Mode_AIN ģ������
GPIO_Mode_IN_FLOATING ��������
GPIO_Mode_IPD ��������
GPIO_Mode_IPU ��������
GPIO_Mode_Out_PP �������
GPIO_Mode_Out_OD ��©���
GPIO_Mode_AF_PP �����������
GPIO_Mode_AF_OD ���ÿ�©���
*/
