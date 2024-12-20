#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define 	run_car     '1'//����ǰ
#define 	back_car   '2'//������
#define 	left_car    '3'//������
#define 	right_car   '4'//������
#define 	stop_car    '0'//����ͣ	  	
extern enum{
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT
}enCarState;
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART3_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;         		//����״̬���	
extern u16 USART3_RX_STA; 
void uart_init(u32 bound);
void uart3_init(u32 bound);
void CarStateOut(void);
void Protocol(void);
void ProtocolCpyData(void);
extern u8 Flag;
extern short LeftMotoPWM,RightMotoPWM;
extern int g_newcarstate;
extern u8 ProtocolString[80];
#endif


