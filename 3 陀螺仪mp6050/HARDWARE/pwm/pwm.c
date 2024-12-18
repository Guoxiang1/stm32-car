#include "pwm.h"                  // Device header



void PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIOA 和 TIM2 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置PWMA和PWMB引脚为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = PWMA_PIN | PWMB_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMA_PORT, &GPIO_InitStructure);

    // 定时器2初始化 (PWM模式)
    TIM_TimeBaseStructure.TIM_Period = 999; // PWM周期
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 1MHz频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // PWM模式配置 (通道3 - PWMA)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // PWM模式配置 (通道4 - PWMB)
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_Cmd(TIM2, ENABLE); // 使能TIM2
}

// 设置电机速度（占空比：0~1000）
void MotorA_SetSpeed(uint16_t speed) {
    TIM_SetCompare3(TIM2, speed); // 通道3，控制PWMA引脚
}

void MotorB_SetSpeed(uint16_t speed) {
    TIM_SetCompare4(TIM2, speed); // 通道4，控制PWMB引脚
}
