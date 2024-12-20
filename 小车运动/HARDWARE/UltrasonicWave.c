/******************** (2015)BST LAB **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			          
 * 实验平台：BST BALANCE CAR
 * 硬件连接：
            PB0  - TRIG      
            PB1  - ECHO      
 
*********************************************************************************/
#include "UltrasonicWave.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "stm32f10x_exti.h"
#include "moto.h"


#define	TRIG_PORT      GPIOB		//定义TRIG_PORT为PB     
#define	ECHO_PORT      GPIOB		//定义ECHO_PORT为PB 
#define	TRIG_PIN       GPIO_Pin_0   //定义TRIG 对应IO口 PB0      
#define	ECHO_PIN       GPIO_Pin_1	//定义ECHO 对应IO口 PB1  
   
int count;

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB0接TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOB, &GPIO_InitStructure);	                 //初始化外设GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //PB1接ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			 //设为输入
  GPIO_Init(GPIOB,&GPIO_InitStructure);						 //初始化GPIOB
	
	 //GPIOB.1	  中断线以及中断初始化配置
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line1;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //优先级组别	
   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//使能按键所在的外部中断通道
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//抢占优先级3 
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//子优先级1 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
   NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}
											    
/***********EXTI1 中断程序****************/
void EXTI1_IRQHandler(void) //当TRIG_PIN脚发出高电平时，ECHO_PIN引脚就变为高电平，触发中断
{
	
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			
			TIM_SetCounter(TIM1,0);
			TIM_Cmd(TIM1, ENABLE);   //开启时钟	Tim1
		
			   count=	1;               
            while(count)	                 //等待低电平
			{
					if(TIM_GetCounter(TIM1)>=20) //10*100us=1ms
					{ 
					    TIM_Cmd(TIM1, DISABLE); 	// 关闭时钟 Tim1
						count=0;					// 循环数清零
					} //ECHO_PIN引脚接收到返回的超声波，引脚输入低电平
					else  count=GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN); //检测PB1是否高电平 高 则count为1 低 则count为0  	
					//低电平结束，此时TIM1->CNT中储存着高电平持续时间的计数次数       
			}
			TIM_Cmd(TIM1, DISABLE);			                                 //定时器 Tim1 关闭
			
		
	  //EXTI->IMR&=~(1<<1);  
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //清除EXTI1线路挂起位
	
}

}

/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
	
 */
void UltrasonicWave_StartMeasure(void)
{
	
  GPIO_SetBits(GPIOB,GPIO_Pin_0);	   //本指令与：GPIO_SetBits(PB,GPIO_Pin_1) 等同IG_PORT,TRIG_PIN); 		  //送>10US的高电平  本指令与：GPIO_SetBits(PB,GPIO_Pin_0) 等同
  delay_us(13);		                      //延时11US
  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
float juli = 0;
short chaoValue = 0;
void chaoshengbo(void)
{  
	
		//juli=TIM_GetCounter(TIM1)*0.1*34/2.0;=》juli=TIM_GetCounter(TIM1)*34/20.0；
      	juli=TIM_GetCounter(TIM1)*0.1*34/2;
		USART3->DR = juli;
	    if(juli <= 8.00)								  //判断若距离小于8cm，小车输出向后PWM值。
	    	chaoValue = -300;					 //距离小于8cm 
		else if(juli >= 10 & juli <= 20)
			chaoValue = 400;						 //距离大于8cm 
	    else 
			chaoValue = 0;						 //距离大于8cm 
  	
 }

/************************(2015)BST LAB************/
