#include "stm32f10x.h"                  // Device header

// ¶¨ÒåPWMÒý½Å
#define PWMA_PORT GPIOA
#define PWMA_PIN  GPIO_Pin_2
#define PWMB_PORT GPIOA
#define PWMB_PIN  GPIO_Pin_3


void PWM_Init(void);

void MotorA_SetSpeed(uint16_t speed);

void MotorB_SetSpeed(uint16_t speed);
