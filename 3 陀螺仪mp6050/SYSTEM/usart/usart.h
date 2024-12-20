#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
//����֧�֣�

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	

extern u8  USART_RX_BUF_2[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA_2;         		//����״̬���	

void uart_init(u32 bound);
void uart_init_2(u32 bound);

void USART_SendString(char *str);
void USART_SendNumber(u8 num);
void PrintChar(char *s);
void UART3_Send_Char(char *s);
void USART3_Send_Byte(unsigned char byte) ;
void USART1_Send_Byte(unsigned char byte);
void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
static char *itoa(int value, char *string, int radix);

#endif


