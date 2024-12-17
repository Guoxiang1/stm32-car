#include "stm32f10x.h"                  // Device header

extern u16 USART_RX_STA3;
extern u8 USART3_RX_BUF[200];

void uart3_init(void);
