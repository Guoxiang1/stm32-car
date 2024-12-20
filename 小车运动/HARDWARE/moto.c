#include "moto.h"
#include "stm32f10x.h"
void MOTOR_GPIO_Init(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOB的外设时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

	/*选择要控制的GPIOB引脚*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	

	/*设置引脚模式为通用推挽输出*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*设置引脚速率为50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*调用库函数，初始化GPIOB*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* 电平设置	*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_12 | GPIO_Pin_15);
	//GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_15);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_15);	 
}

/*
 * 函数名：TIM2_GPIO_Config
 * 描述  ：配置TIM2复用输出PWM时用到的I/O
 * 输入  ：无
 * 输出  ：无
 */
 void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM2 clock enable */
	//PCLK1经过2倍频后作为TIM2的时钟源等于72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
 * 函数名：TIM2_PWM_Init
 * 描述  ：配置TIM2输出的PWM信号的模式，如周期、极性、占空比
 * 输入  ：无
 * 输出  ：无
 */
void TIM2_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	//u16 CCR1_Val = 500;        
	//u16 CCR2_Val = 800;
	u16 CCR3_Val = 0;
	u16 CCR4_Val = 0;
	//u16 PrescalerValue;
/* -----------------------------------------------------------------------
    TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 71999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 1 KHz.
	CC1 update rate = TIM2 counter clock / CCR1_Val
    TIM3 Channelx duty cycle = (TIM2_CCRx/ TIM2_ARR)* 100 = x%
    
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999 ;       //ARR 当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//比较输出使能
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR3_Val时为高电平

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道3
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//预装载使能	

  /* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR4_Val时为高电平

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //使能通道4
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//预装载使能

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // 使能TIM2重载寄存器ARR

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //使能定时器2	
}

/*
 * 函数名：TIM2_PWM_Init
 * 描述  ：TIM2 输出PWM信号初始化，只要调用这个函数
 *         TIM2的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM2_PWM_CCR_Set(u16 CCR3,u16 CCR4)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u16 CCR3_Val ;
	u16 CCR4_Val ;
	CCR3_Val = CCR3;
	CCR4_Val = CCR4;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //使能通道3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //设置通道4的电平跳变值，输出另外一个占空比的PWM

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //使能通道4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}
