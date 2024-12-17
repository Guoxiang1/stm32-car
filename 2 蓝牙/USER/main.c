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


void process_bluetooth_cmd(u8 *cmd);
void Evadible_Mode(void);
		
int main(void) {
  	delay_init();   //��ʱ������ʼ��
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�
		uart_init(9600);		//���ڳ�ʼ��
		CH_SR04_Init(); 		// ��������ʼ��
		uart3_init(); 			//��������3��ʼ��
		NVIC_Config();
		Motor_GPIO_Init(); // ��ʼ����������
    	PWM_Init();        // ��ʼ��PWM
	
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
		length_res = Senor_Using();
//		printf("length=%d cm\r\n", length_res); 
		
		delay_ms(1000);


	// printf("���յ�������Ϊ��%s\r\n", USART3_RX_BUF);
	// memset(USART3_RX_BUF, 0, sizeof(USART3_RX_BUF));
	// USART_RX_STA3 = 0;
  
    process_bluetooth_cmd(USART3_RX_BUF);			//�����������յ�������
		
	}
}


void process_bluetooth_cmd(u8 *cmd) {
    if (strcmp((char *)cmd, "1") == 0) {
        Car_Forward();   	// С��ǰ��

    } else if (strcmp((char *)cmd, "2") == 0) {
        Car_Backward();		// С������

    } else if (strcmp((char *)cmd, "3") == 0) {
        Car_Stop();			// С��ֹͣ

    } else if (strcmp((char *)cmd, "4") == 0) {
        Car_TurnLeft();		// С����ת

    } else if (strcmp((char *)cmd, "5") == 0) {
        Car_SlightLeft();   // С��С��ת

    } else if (strcmp((char *)cmd, "6") == 0) {
        Car_TurnRight();	// С����ת

    } else if (strcmp((char *)cmd, "7") == 0) {
        Car_SlightRight();	// С��С��ת
    }
}


void Evadible_Mode(void)
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
