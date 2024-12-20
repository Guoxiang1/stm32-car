#include "balance.h"
#include "control.h"
#include "mpu6050.h"
#include "motor.h"
#include <math.h>
#include "pwm.h"

// 定义比例控制系数
#define KP 50.0
// 定义积分控制系数
#define KI 0.0
// 定义微分控制系数
#define KD 0.1

// 如果没有定义M_PI，则定义其值为圆周率
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float motorSpeed;

// 当前角度、角速度以及平衡所需的角度
float angle, gyroRate, balanceAngle;
// 控制系统中的误差、上一个周期的误差、积分项和微分项
float error, previousError, integral, derivative;
// 电机的速度


// 平衡控制函数，用于保持系统稳定
void Balance_Control(void) {
    // 调用函数读取当前的加速度和角速度数据
    MPU6050_Pose();

    // 使用atan2计算出基于Y轴与Z轴的倾斜角度，并转换成度数
    angle = atan2((double)accel[1], (double)accel[2]) * 180 / M_PI;

    // 根据陀螺仪X轴的数据计算角速度（假设陀螺仪单位是每秒度）
    gyroRate = gyro[1] / 131.0;

    // 结合上次的平衡角度加上这次的角速度变化，再融合一次新的角度测量结果得到更新后的平衡角度
    // 这里采用了互补滤波器来平滑角度估计
    balanceAngle = 0.98 * (balanceAngle + gyroRate * 0.01) + 0.02 * angle;

    // 计算当前的误差，这里直接使用了平衡角度作为误差
    error = balanceAngle;

    // 积分项累加当前误差
    integral += error * 0.01;

    // 微分项计算当前误差与上一周期误差之差
    derivative = (error - previousError) / 0.01;

    // 应用PID控制器公式计算所需的电机速度调整量
    motorSpeed = KP * error + KI * integral + KD * derivative;

    // 设置电机方向和速度以达到纠正偏差的效果
    if (motorSpeed > 0) {
        MotorA_SetDirection(1); // 前进方向
        MotorB_SetDirection(1); // 前进方向
				MotorA_SetSpeed((int)fabs(motorSpeed));	// 设置电机速度
				MotorB_SetSpeed((int)fabs(motorSpeed));		// 设置电机速度
    } else {
        MotorA_SetDirection(2); // 后退方向
        MotorB_SetDirection(2); // 后退方向
				MotorA_SetSpeed((int)fabs(motorSpeed));	// 设置电机速度
				MotorB_SetSpeed((int)fabs(motorSpeed));		// 设置电机速度

    




    // 更新previousError以便在下一轮循环中使用
    previousError = error;
}
}
