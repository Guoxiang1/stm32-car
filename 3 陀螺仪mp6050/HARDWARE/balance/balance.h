
#ifndef BALANCE_H
#define BALANCE_H

// 其他包含的头文件和定义
#include <stdint.h>

// 声明外部变量
extern float motorSpeed;
extern float angle, gyroRate, balanceAngle;
extern float error, previousError, integral, derivative;

// 函数声明
void MPU6050_Read_All(void);
void Balance_Control(void);

#endif 
