#include "sys.h"
#include "delay.h"

#define LEDPORT	GPIOB	//定义IO接口
#define BEEP	GPIO_Pin_12	//定义IO接口



void BEEP_Init(void);//初始化
void BUZZER_BEEP(int a,int b);
