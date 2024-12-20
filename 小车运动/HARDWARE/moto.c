#include "moto.h"
#include "stm32f10x.h"
void MOTOR_GPIO_Init(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOB������ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

	/*ѡ��Ҫ���Ƶ�GPIOB����*/															   
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	

	/*��������ģʽΪͨ���������*/
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

	/*������������Ϊ50MHz */   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯������ʼ��GPIOB*/
  	GPIO_Init(GPIOB, &GPIO_InitStructure);		  

	/* ��ƽ����	*/
	GPIO_ResetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_12 | GPIO_Pin_15);
	//GPIO_SetBits(GPIOB,GPIO_Pin_12 | GPIO_Pin_15);
	//GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_15);	 
}

/*
 * ��������TIM2_GPIO_Config
 * ����  ������TIM2�������PWMʱ�õ���I/O
 * ����  ����
 * ���  ����
 */
 void TIM2_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM2 clock enable */
	//PCLK1����2��Ƶ����ΪTIM2��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 

  /*GPIOA Configuration: TIM2 channel 3 and 4 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
 * ��������TIM2_PWM_Init
 * ����  ������TIM2�����PWM�źŵ�ģʽ�������ڡ����ԡ�ռ�ձ�
 * ����  ����
 * ���  ����
 */
void TIM2_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM�źŵ�ƽ����ֵ */
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
  TIM_TimeBaseStructure.TIM_Period = 999 ;       //ARR ����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM2 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR3_ValʱΪ�ߵ�ƽ

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��3
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥװ��ʹ��	

  /* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR4_ValʱΪ�ߵ�ƽ

  TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��4
  TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);//Ԥװ��ʹ��

  TIM_ARRPreloadConfig(TIM2, ENABLE);			 // ʹ��TIM2���ؼĴ���ARR

  /* TIM2 enable counter */
  TIM_Cmd(TIM2, ENABLE);                   //ʹ�ܶ�ʱ��2	
}

/*
 * ��������TIM2_PWM_Init
 * ����  ��TIM2 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM2���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void TIM2_PWM_CCR_Set(u16 CCR3,u16 CCR4)
{
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	u16 CCR3_Val ;
	u16 CCR4_Val ;
	CCR3_Val = CCR3;
	CCR4_Val = CCR4;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 //ʹ��ͨ��3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	/* PWM1 Mode configuration: Channel4 */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	  //����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	  //ʹ��ͨ��4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
}
