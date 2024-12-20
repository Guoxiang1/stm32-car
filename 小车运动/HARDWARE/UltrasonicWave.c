/******************** (2015)BST LAB **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			          
 * ʵ��ƽ̨��BST BALANCE CAR
 * Ӳ�����ӣ�
            PB0  - TRIG      
            PB1  - ECHO      
 
*********************************************************************************/
#include "UltrasonicWave.h"
#include "usart.h"
#include "timer.h"
#include "delay.h"
#include "stm32f10x_exti.h"
#include "moto.h"


#define	TRIG_PORT      GPIOB		//����TRIG_PORTΪPB     
#define	ECHO_PORT      GPIOB		//����ECHO_PORTΪPB 
#define	TRIG_PIN       GPIO_Pin_0   //����TRIG ��ӦIO�� PB0      
#define	ECHO_PIN       GPIO_Pin_1	//����ECHO ��ӦIO�� PB1  
   
int count;

/*
 * ��������UltrasonicWave_Configuration
 * ����  ��������ģ��ĳ�ʼ��
 */
void UltrasonicWave_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//�ر�jtag
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					 //PB0��TRIG
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //��Ϊ�������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	         
  GPIO_Init(GPIOB, &GPIO_InitStructure);	                 //��ʼ������GPIO 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				     //PB1��ECH0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			 //��Ϊ����
  GPIO_Init(GPIOB,&GPIO_InitStructure);						 //��ʼ��GPIOB
	
	 //GPIOB.1	  �ж����Լ��жϳ�ʼ������
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);

 	 EXTI_InitStructure.EXTI_Line=EXTI_Line1;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
   //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //���ȼ����	
   NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	//��ռ���ȼ�3 
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;					//�����ȼ�1 
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
   NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}
											    
/***********EXTI1 �жϳ���****************/
void EXTI1_IRQHandler(void) //��TRIG_PIN�ŷ����ߵ�ƽʱ��ECHO_PIN���žͱ�Ϊ�ߵ�ƽ�������ж�
{
	
     if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
			
			TIM_SetCounter(TIM1,0);
			TIM_Cmd(TIM1, ENABLE);   //����ʱ��	Tim1
		
			   count=	1;               
            while(count)	                 //�ȴ��͵�ƽ
			{
					if(TIM_GetCounter(TIM1)>=20) //10*100us=1ms
					{ 
					    TIM_Cmd(TIM1, DISABLE); 	// �ر�ʱ�� Tim1
						count=0;					// ѭ��������
					} //ECHO_PIN���Ž��յ����صĳ���������������͵�ƽ
					else  count=GPIO_ReadInputDataBit(ECHO_PORT,ECHO_PIN); //���PB1�Ƿ�ߵ�ƽ �� ��countΪ1 �� ��countΪ0  	
					//�͵�ƽ��������ʱTIM1->CNT�д����Ÿߵ�ƽ����ʱ��ļ�������       
			}
			TIM_Cmd(TIM1, DISABLE);			                                 //��ʱ�� Tim1 �ر�
			
		
	  //EXTI->IMR&=~(1<<1);  
	
	EXTI_ClearITPendingBit(EXTI_Line1);  //���EXTI1��·����λ
	
}

}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
	
 */
void UltrasonicWave_StartMeasure(void)
{
	
  GPIO_SetBits(GPIOB,GPIO_Pin_0);	   //��ָ���룺GPIO_SetBits(PB,GPIO_Pin_1) ��ͬIG_PORT,TRIG_PIN); 		  //��>10US�ĸߵ�ƽ�T  ��ָ���룺GPIO_SetBits(PB,GPIO_Pin_0) ��ͬ
  delay_us(13);		                      //��ʱ11US
  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
float juli = 0;
short chaoValue = 0;
void chaoshengbo(void)
{  
	
		//juli=TIM_GetCounter(TIM1)*0.1*34/2.0;=��juli=TIM_GetCounter(TIM1)*34/20.0��
      	juli=TIM_GetCounter(TIM1)*0.1*34/2;
		USART3->DR = juli;
	    if(juli <= 8.00)								  //�ж�������С��8cm��С��������PWMֵ��
	    	chaoValue = -300;					 //����С��8cm 
		else if(juli >= 10 & juli <= 20)
			chaoValue = 400;						 //�������8cm 
	    else 
			chaoValue = 0;						 //�������8cm 
  	
 }

/************************(2015)BST LAB************/
