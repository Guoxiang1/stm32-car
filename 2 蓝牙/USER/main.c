#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�
#include <string.h>
#include "cs.h"			//������������
#include "pwm.h"
#include "motor.h"
#include "bluetooth.h"

// Function prototypes

		int length_res;  //������Ų����
		u8 i;
		
		
int main(void) {
  	delay_init();   //��ʱ������ʼ��
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�
		uart_init(9600);		//���ڳ�ʼ��
		CH_SR04_Init(); 		// ��������ʼ��
		uart3_init(); 			//��������3��ʼ��
		NVIC_Config();
		Motor_GPIO_Init(); // ��ʼ����������
    PWM_Init();        // ��ʼ��PWM
	
		printf("AT\r\n");
		delay_ms(1000);
		printf("AT+BAUD4\r\n");
		delay_ms(1000);
		printf("AT+NAMEzhichu\r\n");
		delay_ms(1000);
		printf("AT+PIN1234\r\n");
		delay_ms(1000);
		while(1)
	{	
		length_res = Senor_Using();
		printf("length=%d cm\r\n", length_res); 
		
		delay_ms(1000);
		    
			MotorA_SetDirection(1); // ���A��ת
      MotorA_SetSpeed(500);   // �ٶ�50%
                                                                                                     
     MotorB_SetDirection(2); // ���B��ת
		MotorB_SetSpeed(700);   // �ٶ�70%

	}

}
