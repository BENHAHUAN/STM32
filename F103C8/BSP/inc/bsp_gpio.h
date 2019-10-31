#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

void GPIO_Config(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode);
void EXIT_RX2_Config(void);
void EXIT_RX1_Config(void);
#endif

