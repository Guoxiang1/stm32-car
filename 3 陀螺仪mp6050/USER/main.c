#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "cs.h"			//������������
#include "pwm.h"
#include "motor.h"
#include "bluetooth.h"
#include "control.h"
#include "balance.h"
#include "mpu6050.h"
#include "i2c.h"
#include <math.h>
#include "stm32f10x_tim.h"
#include "stm32f10x.h"  // Include the header file that defines TIM_TypeDef

// Function prototypes

// #define TIM1 ((TIM_TypeDef *)TIM1_BASE)
// #define TIM3 ((TIM_TypeDef *)TIM3_BASE)
// #define TIM4 ((TIM_TypeDef *)TIM4_BASE)

int length_res;  //������Ų����
u8 i;
int speed;
float Middle_angle=0;       //Ŀ��Ƕ�

float Balance_Kp=480;	//800	  //����ƽ����Ƶĵ�� PWM PD ���� kp �� P ϵ�� 
float Balance_Kd=3;		//1  //kd �� D ϵ��
float Velocity_Kp=-300;//180
float Velocity_Ki=-300/200;//0.9


void process_bluetooth_cmd(void);
void Evadible_Mode(void);
int Balance(float Angle,float Gyro);
		
int main(void) {
  	delay_init();   //��ʱ������ʼ��
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�
		uart_init(9600);		//���ڳ�ʼ��
		CH_SR04_Init(); 		// ��������ʼ��
		uart3_init(); 			//��������3��ʼ��
		NVIC_Config();
		PWM_Init();        // ��ʼ��PWM
		i2cInit();							   //IIC��ʼ�� ���ڹҿ��������ϵ��豸ʹ��
		delay_ms(10);						   //��ʱ10ms
		MPU6050_Init();						   //MPU6050 DMP�����ǳ�ʼ��
		Motor_GPIO_Init(); // ��ʼ���������
    	


	
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
/*���������*/
//		length_res = Senor_Using();
//		printf("length=%d cm\r\n", length_res); 
//		delay_ms(1000);


/*�����������*/
	 	// cmd = USART3_RX_BUF;
    	// process_bluetooth_cmd();			//�����������յ�������
		// // ��ջ�����
        // memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
        // USART_RX_STA3 = 0;


/*����ƽ����ƺ���*/ 

				// Balance_Control();
				// printf("motorSpeed=%0.1f cm\r\n", motorSpeed);


   
        // ����ʵ����ӳ٣�ȷ������Ƶ��
     //   delay_ms(10);
		
	}
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������Ǻ͵�ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
/*�����ʼ������*/
float PWM_MAX = 7200 ;
float PWM_MIN = -7200 ;
/*�޷�����*/
void Limit(float *motoA,float *motoB)
{
	if(*motoA>PWM_MAX) *motoA=PWM_MAX;
	if(*motoA<PWM_MIN) *motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX) *motoB=PWM_MAX;
	if(*motoB<PWM_MIN) *motoB=PWM_MIN;
}
void TIM1_UP_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        int16_t gyrox, gyroy, gyroz;
        int16_t Encoder_L, Encoder_R;
        float Balance_Pwm, Velocity_Pwm;
		float Moto1, Moto2;
		MPU6050_Pose(); // ��ȡ��̬����
        {
        	Balance_Pwm = Balance(Roll, gyrox);
            Encoder_L = (short)TIM_GetCounter(TIM3);
            Encoder_R = (short)TIM_GetCounter(TIM4);
            Velocity_Pwm = velocity(Encoder_L, Encoder_R);
            TIM_SetCounter(TIM3, 0);
            TIM_SetCounter(TIM4, 0);
            // Turn_Pwm = turn(Encoder_L, Encoder_R, yaw);
            Moto1 = Balance_Pwm + Velocity_Pwm;
            Moto2 = Balance_Pwm + Velocity_Pwm;
            Limit(&Moto1, &Moto2);
            Set_Pwm(Moto1, Moto2);
        }

        // ����жϱ�־
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

void Evadible_Mode(void)   //����ģʽ
{
	Car_Forward();		// С��ǰ��
	if(length_res<=15)
	{
        Car_Backward();		// С������
        delay_ms(500);
        Car_AvoidLeft();	// С��������ת
        delay_ms(1000);
	}
}

//ֱ�����ƹ����㷨======================================================================================================================
int Balance(float Angle,float Gyro)
{
float Angle_bias,Gyro_bias;
int balance;
Angle_bias=Middle_angle-Angle; //���ƽ��ĽǶ���ֵ �ͻ�е���
Gyro_bias=0-Gyro;
balance=-Balance_Kp*Angle_bias-Gyro_bias*Balance_Kd; //����ƽ����Ƶĵ�� PWM PD ���� kp �� P ϵ�� kd �� D ϵ��

return balance;
}

//�ٶ��㷨======================================================================================================================

/**
 * @function:int Velocity(int Encoder_left,int Encoder_right,int Mechanical_velocity)
 * @description:�ٶȻ�PI����
 * @param {int} Encoder_left ���� :������������ֵ
 * @param {int} Encoder_right ���� :�ҵ����������ֵ
 * @param {int} Mechanical_velocity ���� :Ŀ���ٶ� ����Ϊֻ����С������ƽ�Ⲣ��ֹ������Ŀ���ٶ�Ҳ��Ϊ0
 * @return {int} Up_balance :�ٶȻ�����PWM
 */
int velocity(int encoder_left,int encoder_right)
{  
//	Velocity_Ki=Velocity_Kp/200;
	int Velocity,Encoder_Least; //�ٶȻ�����PWM,��ȡ�����ٶ�ƫ��
	static float Encoder,Encoder_Integral; //һ�׵�ͨ�˲�����ٶ�ֵ���ٶȵĻ��֣���Ϊ�����ۼӺ��˲�����Ҫ��������Ϊ��̬�����洢��ȫ������������ȫ�ֱ���
    Encoder_Least =(encoder_left+encoder_right)-0;                    //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.8;		                                                //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.2;	                                    //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                       //===���ֳ�λ�� ����ʱ�䣺10ms
		if(Encoder_Integral>12000)  	Encoder_Integral=12000;             //===�����޷�
		if(Encoder_Integral<-12000)	Encoder_Integral=-12000;              //===�����޷�	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;        //===�ٶȿ���	
	  return Velocity;
}
//ת��
/**************************************************************************
�������ܣ�ת�����  �޸�ת���ٶȣ����޸�Turn_Amplitude����
��ڲ��������ֱ����������ֱ�������Z��������
����  ֵ��ת�����PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//ת�����
{
	 static float Turn_Target;
     static float		 Turn;
//     static float	 Encoder_temp;
//	 static float Turn_Convert=0.9;
//	 static float Turn_Count;
	  float Kp=42,Kd=0;     
	  
  	//=============ת��PD������=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===���Z�������ǽ���PD����
	  return Turn;
}
