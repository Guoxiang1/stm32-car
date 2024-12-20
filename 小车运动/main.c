#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "moto.h"
#include "UltrasonicWave.h"
#include "timer.h"
int main()
{
	SystemInit();    //=====ϵͳ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	               
	Timerx_Init(4999,7199);
	
	uart_init(9600);
	uart3_init(9600);
	UltrasonicWave_Configuration();
	delay_init();
	
	MOTOR_GPIO_Init();	
	TIM2_GPIO_Config();
	TIM2_PWM_Init();
	TIM2_PWM_CCR_Set(400,400);
	
	while(1)
	{
		UltrasonicWave_StartMeasure();
		chaoshengbo();
		
		if(Flag)
		{
			//ProtocolCpyData();
			Protocol();          //�����������͵�����
			Flag = 0;			   
//			USART3->DR = USART3_RX_BUF[0]; 
//			while((USART3->SR&0X40)==0);//ѭ������,ֱ���������
		}
		
		CarStateOut();
		delay_ms(1000);
	}
}
