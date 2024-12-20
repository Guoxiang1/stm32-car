/******************** (C) COPYRIGHT 2012 ***************************
 * 文件名  ：SysTick.c
 * 描述    ：SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
 *           常用的有 1us 10us 1ms 中断。         

SysTick 系统滴答时钟嵌在NVIC。它是一个24位向下递减的定时器，每计数一次需要时间是1/SYSTICK，SYSTICK是系统定时器时钟，
其可以直接取自系统时钟也可以8分频后取出。当定时器计数到0，将从LOAD寄存器中自动重装初值。
如果开启SysTick中断，计数到0就产生一中断信号

SysTick相关寄存器

CTRL  	SysTick控制及状态寄存器
LOAD  	SysTick重装载数值寄存器
VAL   	SysTick当前数值寄存器
CALIB 	SysTick校准数值寄存器
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用 
 */
void SysTick_Init(void)		  //1ms定时时钟
{
	/* SystemFrequency / 100     10ms中断一次
	   SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock / 200))	// ST3.5.0库版本   1s/x= a ms  
 	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/*
 * 函数名：Delay_us
 * 描述  ：us延时程序,10us为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 *       ：外部调用 
 */

void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
}


/*
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/******************* (C) COPYRIGHT 2012 *****END OF FILE************/
