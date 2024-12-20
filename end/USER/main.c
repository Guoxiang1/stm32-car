/******************** (C) COPYRIGHT BST BALANCECAR **************************
 * �ļ���  ��main.c
**********************************************************************************/
//#include "stm32f10x.h"
#include "mpu6050.h"
#include "i2c_mpu6050.h"
#include "motor.h"
#include "upstandingcar.h"
#include "SysTick.h"
//#include "led.h"
#include "adc.h"
#include "usart.h"
#include "i2c.h"
#include "timer.h"
#include "UltrasonicWave.h"


float gyz;
int acc;
int acc1;

/*Э�����*/
//extern u8 newLineReceived = 0;

/*
 * ��������main
 * ����  ��������
 Kp �D�D �������ı���ϵ��
 Ti ���� �������Ļ���ʱ�䣬Ҳ�ƻ���ϵ��
 Td �D�D ��������΢��ʱ�䣬Ҳ��΢��ϵ��
 */
int main(void)
{	
       
	
	SystemInit();                   //=====ϵͳ��ʼ��
	Timerx_Init(5000,7199);				   //��ʱ��TIM1
	UltrasonicWave_Configuration(); 	   //��������ʼ������ IO�ڼ��ж�����			    

	USART1_Config();						//����1��ʼ�� ��λ��
	USART3_Config();						//����3��ʼ�� ������USART3������ͬIO��

	TIM2_PWM_Init();					   //PWM�����ʼ��
	MOTOR_GPIO_Config();				  //���IO�ڳ�ʼ��
//	LED_GPIO_Config();
	Adc_Init();
	//TIM3_External_Clock_CountingMode();	   //������������ⲿ�жϿ�PA7ʹ��TIM3��ʱ������Ϊ����������
	//TIM4_External_Clock_CountingMode();	   //�ҵ����������ⲿ�жϿ�PB7ʹ��TIM4��ʱ������Ϊ����������
	TIM3_Encoder_Init();                       //��������ȡ������ PA6 7 
	TIM4_Encoder_Init();                       //��������ȡ������ PB6 7	
	////////////////////DMP/////////////////////////////////
	i2cInit();							   //IIC��ʼ�� ���ڹҿ��������ϵ��豸ʹ��
	delay_nms(10);						   //��ʱ10ms
	MPU6050_Init();						   //MPU6050 DMP�����ǳ�ʼ��

	SysTick_Init();						  //SysTick������ʼ��	
	CarUpstandInit();					  //С��ֱ��������ʼ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	 //ʹ�����㷨ʱ��

	while (1)
	{

// 		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		MPU6050_Pose();						 //��ȡMPU6050�Ƕ�״̬
// 		gy0=gyro[0];
// 		UltrasonicWave_StartMeasure();	   //���ó��������ͳ��� ��Trig�� <10us �ߵ�ƽ		 
// 		chaoshengbo();			       //���㳬����������
//    printf("%d",ucBluetoothValue);
//		printf("\t");
//		printf("%f",BST_fSpeedControlOutNew);
//		printf("\t");
//		printf("%f",BST_fCarAngle);
//		printf("\t");
// 		printf("%f",BST_fLeftMotorOut);
//		printf("\t");
//		printf("\n"); 
		
		if (newLineReceived)  //����ģʽ
		{
			ProtocolCpyData();   //��ȡ�������͵�����
			Protocol();          //�����������͵�����
		}
		/*ͨ��״̬����С��*/
		CarStateOut();	//С������	
		SendAutoUp();   //�Զ��ϱ�
		
	 }
 								    
}