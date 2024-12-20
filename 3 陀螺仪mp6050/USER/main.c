#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "cs.h"			//超声波传感器
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

int length_res;  //用来存放测距结果
u8 i;
int speed;
float Middle_angle=0;       //目标角度

float Balance_Kp=480;	//800	  //计算平衡控制的电机 PWM PD 控制 kp 是 P 系数 
float Balance_Kd=3;		//1  //kd 是 D 系数
float Velocity_Kp=-300;//180
float Velocity_Ki=-300/200;//0.9


void process_bluetooth_cmd(void);
void Evadible_Mode(void);
int Balance(float Angle,float Gyro);
		
int main(void) {
  	delay_init();   //延时函数初始化
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级
		uart_init(9600);		//串口初始化
		CH_SR04_Init(); 		// 超声波初始化
		uart3_init(); 			//蓝牙串口3初始化
		NVIC_Config();
		PWM_Init();        // 初始化PWM
		i2cInit();							   //IIC初始化 用于挂靠在总线上的设备使用
		delay_ms(10);						   //延时10ms
		MPU6050_Init();						   //MPU6050 DMP陀螺仪初始化
		Motor_GPIO_Init(); // 初始化电机引脚
    	


	
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
/*超声波相关*/
//		length_res = Senor_Using();
//		printf("length=%d cm\r\n", length_res); 
//		delay_ms(1000);


/*蓝牙控制相关*/
	 	// cmd = USART3_RX_BUF;
    	// process_bluetooth_cmd();			//处理蓝牙接收到的命令
		// // 清空缓冲区
        // memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
        // USART_RX_STA3 = 0;


/*调用平衡控制函数*/ 

				// Balance_Control();
				// printf("motorSpeed=%0.1f cm\r\n", motorSpeed);


   
        // 添加适当的延迟，确保控制频率
     //   delay_ms(10);
		
	}
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：倾角和电压
返回  值：1：异常  0：正常
**************************************************************************/
/*电机初始化函数*/
float PWM_MAX = 7200 ;
float PWM_MIN = -7200 ;
/*限幅函数*/
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
		MPU6050_Pose(); // 读取姿态数据
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

        // 清除中断标志
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
}

void Evadible_Mode(void)   //避障模式
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

//直立控制功能算法======================================================================================================================
int Balance(float Angle,float Gyro)
{
float Angle_bias,Gyro_bias;
int balance;
Angle_bias=Middle_angle-Angle; //求出平衡的角度中值 和机械相关
Gyro_bias=0-Gyro;
balance=-Balance_Kp*Angle_bias-Gyro_bias*Balance_Kd; //计算平衡控制的电机 PWM PD 控制 kp 是 P 系数 kd 是 D 系数

return balance;
}

//速度算法======================================================================================================================

/**
 * @function:int Velocity(int Encoder_left,int Encoder_right,int Mechanical_velocity)
 * @description:速度环PI控制
 * @param {int} Encoder_left 数据 :左电机编码器的值
 * @param {int} Encoder_right 数据 :右电机编码器的值
 * @param {int} Mechanical_velocity 数据 :目标速度 ，因为只是让小车尽快平衡并静止，这里目标速度也设为0
 * @return {int} Up_balance :速度环控制PWM
 */
int velocity(int encoder_left,int encoder_right)
{  
//	Velocity_Ki=Velocity_Kp/200;
	int Velocity,Encoder_Least; //速度环控制PWM,获取最新速度偏差
	static float Encoder,Encoder_Integral; //一阶低通滤波后的速度值，速度的积分；因为积分累加和滤波的需要，故设置为静态量，存储在全局区域类似于全局变量
    Encoder_Least =(encoder_left+encoder_right)-0;                    //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.8;		                                                //===一阶低通滤波器       
		Encoder += Encoder_Least*0.2;	                                    //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                       //===积分出位移 积分时间：10ms
		if(Encoder_Integral>12000)  	Encoder_Integral=12000;             //===积分限幅
		if(Encoder_Integral<-12000)	Encoder_Integral=-12000;              //===积分限幅	
		Velocity=Encoder*Velocity_Kp+Encoder_Integral*Velocity_Ki;        //===速度控制	
	  return Velocity;
}
//转向环
/**************************************************************************
函数功能：转向控制  修改转向速度，请修改Turn_Amplitude即可
入口参数：左轮编码器、右轮编码器、Z轴陀螺仪
返回  值：转向控制PWM
作    者：平衡小车之家
**************************************************************************/
int turn(int encoder_left,int encoder_right,float gyro)//转向控制
{
	 static float Turn_Target;
     static float		 Turn;
//     static float	 Encoder_temp;
//	 static float Turn_Convert=0.9;
//	 static float Turn_Count;
	  float Kp=42,Kd=0;     
	  
  	//=============转向PD控制器=======================//
		Turn=-Turn_Target*Kp-gyro*Kd;                 //===结合Z轴陀螺仪进行PD控制
	  return Turn;
}
