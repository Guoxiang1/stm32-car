#include "motor.h"



// 初始化GPIO函数
void Motor_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能GPIOA时钟
    
    // 配置AIN1、AIN2引脚（电机A）
    GPIO_InitStructure.GPIO_Pin = AIN1_PIN | AIN2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_Init(AIN1_PORT, &GPIO_InitStructure);

    // 配置BIN1、BIN2引脚（电机B）
    GPIO_InitStructure.GPIO_Pin = BIN1_PIN | BIN2_PIN;
    GPIO_Init(BIN1_PORT, &GPIO_InitStructure);
}

// 电机A方向控制
void MotorA_SetDirection(uint8_t dir) {
    if (dir == 1) { // 正转
        GPIO_SetBits(AIN1_PORT, AIN2_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN);
    } else if (dir == 2) { // 反转
        GPIO_SetBits(AIN1_PORT, AIN1_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN2_PIN);
    } else { // 停止
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN | AIN2_PIN);
    }
}

// 电机B方向控制
void MotorB_SetDirection(uint8_t dir) {
    if (dir == 1) { // 正转
        GPIO_SetBits(BIN1_PORT, BIN2_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN);
    } else if (dir == 2) { // 反转
        GPIO_SetBits(BIN1_PORT, BIN1_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN2_PIN);
    } else { // 停止
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN | BIN2_PIN);
    }
}
