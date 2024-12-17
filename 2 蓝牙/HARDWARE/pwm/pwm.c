#include "pwm.h"                  // Device header



void PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // ʹ�� GPIOA �� TIM2 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // ����PWMA��PWMB����Ϊ�����������
    GPIO_InitStructure.GPIO_Pin = PWMA_PIN | PWMB_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(PWMA_PORT, &GPIO_InitStructure);

    // ��ʱ��2��ʼ�� (PWMģʽ)
    TIM_TimeBaseStructure.TIM_Period = 999; // PWM����
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 1MHzƵ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // PWMģʽ���� (ͨ��3 - PWMA)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // ��ʼռ�ձ�Ϊ0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // PWMģʽ���� (ͨ��4 - PWMB)
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_Cmd(TIM2, ENABLE); // ʹ��TIM2
}

// ���õ���ٶȣ�ռ�ձȣ�0~1000��
void MotorA_SetSpeed(uint16_t speed) {
    TIM_SetCompare3(TIM2, speed); // ͨ��3������PWMA����
}

void MotorB_SetSpeed(uint16_t speed) {
    TIM_SetCompare4(TIM2, speed); // ͨ��4������PWMB����
}
