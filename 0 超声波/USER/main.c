#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x.h" // ��׼��ͷ�ļ�
#include <string.h>
#include "cs.h"			//������������

// Function prototypes

		int length_res;  //������Ų����
		u8 i;
		
		
int main(void) {
  	delay_init();   //��ʱ������ʼ��
   	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж����ȼ�
		uart_init(9600);		//���ڳ�ʼ��
		CH_SR04_Init();
		NVIC_Config();
		while(1)
	{	
		length_res = Senor_Using();
		{
		printf("length=\r\n"); 
		printf("%d\n",length_res);
		delay_ms(100);

	}
}
}

