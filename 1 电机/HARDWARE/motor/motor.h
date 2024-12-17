 #include "stm32f10x.h"
 
#define AIN1_PORT GPIOA
#define AIN1_PIN  GPIO_Pin_0

#define AIN2_PORT GPIOA
#define AIN2_PIN  GPIO_Pin_1

#define BIN1_PORT GPIOA
#define BIN1_PIN  GPIO_Pin_3

#define BIN2_PORT GPIOA
#define BIN2_PIN  GPIO_Pin_4


void Motor_GPIO_Init();
void MotorA_SetDirection(uint8_t dir);
void MotorB_SetDirection(uint8_t dir) ;