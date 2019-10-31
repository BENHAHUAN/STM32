#include "bsp.h"

#define SYSCLK 72

void Delay_ms(uint32_t _ms){
    uint32_t i;
	SysTick_Config(SYSCLK*1000);
		for( i = 0; i < _ms; i++){
		while(!((SysTick->CTRL)&(1<<16)));
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void Delay_us(uint32_t _us){
    uint32_t i;
	SysTick_Config(SYSCLK);
	for( i = 0; i < _us; i++){
		while(!((SysTick->CTRL)&(1<<16)));
	}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

}

