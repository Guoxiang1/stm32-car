#include "stm32f10x.h"

// 定义电机A和电机B的引脚
#define AIN1_PORT GPIOB
#define AIN1_PIN  GPIO_Pin_14

#define AIN2_PORT GPIOB
#define AIN2_PIN  GPIO_Pin_15

#define BIN1_PORT GPIOB
#define BIN1_PIN  GPIO_Pin_13

#define BIN2_PORT GPIOB
#define BIN2_PIN  GPIO_Pin_12


void Motor_GPIO_Init(void);

void MotorA_SetDirection(uint8_t dir);

void MotorB_SetDirection(uint8_t dir);
