#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // 标准库头文件
#include <string.h>
#include "cs.h"			//超声波传感器

// Function prototypes

		int length_res;  //用来存放测距结果
		u8 i;
		
		
int main(void) {
  	delay_init();   //延时函数初始化
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级
		uart_init(9600);		//串口初始化
		CH_SR04_Init();
		NVIC_Config();
		while(1)
	{	
		length_res = Senor_Using();
		{
		printf("length=\r\n"); 
		printf("%d\n",length_res);
		delay_ms(100);

	}
}
}

