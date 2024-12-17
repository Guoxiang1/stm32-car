#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件
#include <string.h>
#include "cs.h"			//超声波传感器
#include "pwm.h"
#include "motor.h"
#include "bluetooth.h"

// Function prototypes

		int length_res;  //用来存放测距结果
		u8 i;
		
		
int main(void) {
  	delay_init();   //延时函数初始化
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级
		uart_init(9600);		//串口初始化
		CH_SR04_Init(); 		// 超声波初始化
		uart3_init(); 			//蓝牙串口3初始化
		NVIC_Config();
		Motor_GPIO_Init(); // 初始化方向引脚
    PWM_Init();        // 初始化PWM
	
		printf("AT\r\n");
		delay_ms(1000);
		printf("AT+BAUD4\r\n");
		delay_ms(1000);
		printf("AT+NAMEzhichu\r\n");
		delay_ms(1000);
		printf("AT+PIN1234\r\n");
		delay_ms(1000);
		while(1)
	{	
		length_res = Senor_Using();
		printf("length=%d cm\r\n", length_res); 
		
		delay_ms(1000);
		    
			MotorA_SetDirection(1); // 电机A正转
      MotorA_SetSpeed(500);   // 速度50%
                                                                                                     
     MotorB_SetDirection(2); // 电机B反转
		MotorB_SetSpeed(700);   // 速度70%

	}

}
