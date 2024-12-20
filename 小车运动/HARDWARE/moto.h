#ifndef __MOTO_H
#define __MOTO_H

#include "stm32f10x.h"
void MOTOR_GPIO_Init(void);
void TIM2_GPIO_Config(void);
void TIM2_PWM_Init(void);
void TIM2_PWM_CCR_Set(u16 CCR3,u16 CCR4);


#endif
