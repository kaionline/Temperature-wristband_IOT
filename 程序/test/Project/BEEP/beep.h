#include "sys.h"
#include "delay.h"

#define LEDPORT	GPIOB	//����IO�ӿ�
#define BEEP	GPIO_Pin_12	//����IO�ӿ�



void BEEP_Init(void);//��ʼ��
void BUZZER_BEEP(int a,int b);
