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


void process_bluetooth_cmd(u8 *cmd);
void Evadible_Mode(void);
		
int main(void) {
  	delay_init();   //延时函数初始化
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级
		uart_init(9600);		//串口初始化
		CH_SR04_Init(); 		// 超声波初始化
		uart3_init(); 			//蓝牙串口3初始化
		NVIC_Config();
		Motor_GPIO_Init(); // 初始化方向引脚
    	PWM_Init();        // 初始化PWM
	
		// printf("AT\r\n");
		// delay_ms(1000);
		// printf("AT+BAUD4\r\n");
		// delay_ms(1000);
		// printf("AT+NAMEzhichu\r\n");
		// delay_ms(1000);
		// printf("AT+PIN1234\r\n");
		// delay_ms(1000);
		while(1)
	{	
		length_res = Senor_Using();
//		printf("length=%d cm\r\n", length_res); 
		
		delay_ms(1000);


	// printf("接收到的数据为：%s\r\n", USART3_RX_BUF);
	// memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	// USART_RX_STA3 = 0;
  
    process_bluetooth_cmd(USART3_RX_BUF);			//处理蓝牙接收到的命令
		
	}
}


void process_bluetooth_cmd(u8 *cmd) {
    if (strcmp((char *)cmd, "1") == 0) {
        Car_Forward();   	// 小车前进

    } else if (strcmp((char *)cmd, "2") == 0) {
        Car_Backward();		// 小车后退

    } else if (strcmp((char *)cmd, "3") == 0) {
        Car_Stop();			// 小车停止

    } else if (strcmp((char *)cmd, "4") == 0) {
        Car_TurnLeft();		// 小车左转

    } else if (strcmp((char *)cmd, "5") == 0) {
        Car_SlightLeft();   // 小车小左转

    } else if (strcmp((char *)cmd, "6") == 0) {
        Car_TurnRight();	// 小车右转

    } else if (strcmp((char *)cmd, "7") == 0) {
        Car_SlightRight();	// 小车小右转
    }
}


void Evadible_Mode(void)
{
	Car_Forward();		// 小车前进
	if(length_res<=15)
	{
        Car_Backward();		// 小车后退
        delay_ms(500);
        Car_AvoidLeft();	// 小车避障左转
        delay_ms(1000);
	}
}
