#include "motor.h"



// ��ʼ��GPIO����
void Motor_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ʹ��GPIOAʱ��
    
    // ����AIN1��AIN2���ţ����A��
    GPIO_InitStructure.GPIO_Pin = AIN1_PIN | AIN2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // �������
    GPIO_Init(AIN1_PORT, &GPIO_InitStructure);

    // ����BIN1��BIN2���ţ����B��
    GPIO_InitStructure.GPIO_Pin = BIN1_PIN | BIN2_PIN;
    GPIO_Init(BIN1_PORT, &GPIO_InitStructure);
}

// ���A�������
void MotorA_SetDirection(uint8_t dir) {
    if (dir == 1) { // ��ת
        GPIO_SetBits(AIN1_PORT, AIN2_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN);
    } else if (dir == 2) { // ��ת
        GPIO_SetBits(AIN1_PORT, AIN1_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN2_PIN);
    } else { // ֹͣ
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN | AIN2_PIN);
    }
}

// ���B�������
void MotorB_SetDirection(uint8_t dir) {
    if (dir == 1) { // ��ת
        GPIO_SetBits(BIN1_PORT, BIN2_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN);
    } else if (dir == 2) { // ��ת
        GPIO_SetBits(BIN1_PORT, BIN1_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN2_PIN);
    } else { // ֹͣ
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN | BIN2_PIN);
    }
}
