#ifndef __BSP_H
#define __BSP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "stm32f10x.h"
extern uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */
#define delay_us Delay_us
#define delay_ms Delay_ms
#include "stm32_dsp.h"
#include "bsp_gpio.h"
//#include "bsp_usart.h"
#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_SysTick.h"
#include "bsp_dwt.h"
#include "bsp_lora.h"
#include "usartDriver.h"
#include "DataScope_DP.h"
#include "PID.h"
#include "wdd35d.h"
#include "bsp_tim_pwm.h"
#include "bsp_tim_rx.h"
#include "bsp_pwm_input.h"
#include "Motor_Set.h"
#include "bsp_Encoder.h"
#include "BSP_CAN.h"

//#include "EventRecorder.h"
//#include "VL53L0X.h"
//#include "FFT.h"
void bsp_init(void);
void bsp_Idle(void);
#endif
