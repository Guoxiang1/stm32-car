#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define 	run_car     '1'//按键前
#define 	back_car   '2'//按键后
#define 	left_car    '3'//按键左
#define 	right_car   '4'//按键右
#define 	stop_car    '0'//按键停	  	
extern enum{
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT
}enCarState;
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u8  USART3_RX_BUF[USART_REC_LEN];
extern u16 USART_RX_STA;         		//接收状态标记	
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


