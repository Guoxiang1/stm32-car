#include "motor.h"



// ?????GPIO????
void Motor_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // ???GPIOA???
    
    // ????AIN1??AIN2????????A??
    GPIO_InitStructure.GPIO_Pin = AIN1_PIN | AIN2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // ???????
    GPIO_Init(AIN1_PORT, &GPIO_InitStructure);

    // ????BIN1??BIN2????????B??
    GPIO_InitStructure.GPIO_Pin = BIN1_PIN | BIN2_PIN;
    GPIO_Init(BIN1_PORT, &GPIO_InitStructure);
}

// ???A???????
void MotorA_SetDirection(uint8_t dir) {
    if (dir == 1) { // ???
        GPIO_SetBits(AIN1_PORT, AIN2_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN);
    } else if (dir == 2) { // ???
        GPIO_SetBits(AIN1_PORT, AIN1_PIN);
        GPIO_ResetBits(AIN1_PORT, AIN2_PIN);
    } else { // ??
        GPIO_ResetBits(AIN1_PORT, AIN1_PIN | AIN2_PIN);
    }
}

// ???B???????
void MotorB_SetDirection(uint8_t dir) {
    if (dir == 1) { // ???
        GPIO_SetBits(BIN1_PORT, BIN2_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN);
    } else if (dir == 2) { // ???
        GPIO_SetBits(BIN1_PORT, BIN1_PIN);
        GPIO_ResetBits(BIN1_PORT, BIN2_PIN);
    } else { // ??
        GPIO_ResetBits(BIN1_PORT, BIN1_PIN | BIN2_PIN);
    }
}

// 小车前进
void Car_Forward(void) {
    MotorA_SetDirection(1);
    MotorA_SetSpeed(200);
    MotorB_SetDirection(1);
    MotorB_SetSpeed(200);
}

// 小车后退
void Car_Backward(void) {
    MotorA_SetDirection(2);
    MotorA_SetSpeed(700);
    MotorB_SetDirection(2);
    MotorB_SetSpeed(700);
}

// 小车停止
void Car_Stop(void) {
    MotorA_SetSpeed(0);
    MotorB_SetSpeed(0);
}

// 小车左转
void Car_TurnLeft(void) {
    MotorA_SetDirection(2);
    MotorA_SetSpeed(700);
    MotorB_SetDirection(1);
    MotorB_SetSpeed(700);
}

// 小车小左转
void Car_SlightLeft(void) {
    MotorA_SetDirection(1);
    MotorA_SetSpeed(500);
    MotorB_SetDirection(1);
    MotorB_SetSpeed(700);
}

// 小车右转
void Car_TurnRight(void) {
    MotorA_SetDirection(1);
    MotorA_SetSpeed(700);
    MotorB_SetDirection(2);
    MotorB_SetSpeed(700);
}

// 小车小右转
void Car_SlightRight(void) {
    MotorA_SetDirection(1);
    MotorA_SetSpeed(700);
    MotorB_SetDirection(1);
    MotorB_SetSpeed(500);
}

// 小车避障左转
void Car_AvoidLeft(void) {
    MotorA_SetDirection(2);
    MotorA_SetSpeed(500);
    MotorB_SetDirection(1);
    MotorB_SetSpeed(700);
}

// 小车避障右转
void Car_AvoidRight(void) {
    MotorA_SetDirection(1);
    MotorA_SetSpeed(700);
    MotorB_SetDirection(2);
    MotorB_SetSpeed(500);
}
