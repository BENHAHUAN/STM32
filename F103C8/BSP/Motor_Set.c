#include "includes.h"

void Motor_Set(int32_t PWM)
{
	if(PWM > 0)
	{
		GPIOB->BSRR = GPIO_Pin_14;
		GPIOB->BRR =  GPIO_Pin_15;
		TIM1->CCR1 = PWM + 1400;
	}
	else if(PWM < 0)
	{
		GPIOB->BSRR = GPIO_Pin_15;
		GPIOB->BRR =  GPIO_Pin_14;
		TIM1->CCR1 = 1400 + abs(PWM);
	}
	else
	{
		TIM1->CCR1 = 1;
	}
//	else
//	{
//		TIM1->CCR1 = 1;
//	}
}	