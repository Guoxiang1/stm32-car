#include "sys.h"
#include "usart.h"	 
#include "moto.h"
#include <string.h>
#include "UltrasonicWave.h"

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //如果使能了接收
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
u16 USART3_RX_STA=0;       //接收状态标记 


u8 ProtocolString[80] = {0};
/*小车运行状态枚举*/



int g_newcarstate = 0; //  1前2后3左4右0停止
short LeftMotoPWM = 0,RightMotoPWM = 0;
void uart_init(u32 bound){
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	USART_DeInit(USART1);															//复位串口 复位串口 复位串口 1
	
	//USART1_TX(输出)   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//PA.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//初始化GPIOA.9
	
	//USART1_RX(输入)	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//初始化GPIOA.10
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;							//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				    //收发模式
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	
	USART_Init(USART1, &USART_InitStructure); 										//初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//开启串口接受中断
	USART_Cmd(USART1, ENABLE);                    									//使能串口1 
	
}

void uart3_init(u32 bound){
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
		/* config GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE);
	USART_DeInit(USART3);															//复位串口 复位串口 复位串口 1
	
	//USART3_TX(输出)   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//初始化GPIOA.9
	
	//USART3_RX(输入)	  GPIOB.11初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;										//PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//初始化GPIOA.10
	
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;							//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据指定的参数初始化VIC寄存器
	
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				    //收发模式
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	
	USART_Init(USART3, &USART_InitStructure); 										//初始化串口3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    									//使能串口13
	
}

void USART1_IRQHandler(void)                										//串口1中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)							//接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART1);           									//读取接收到的数据
		if((USART_RX_STA&0x8000) == 0)												//接收未完成
		{
			/*  当收到回车（回车的表示由 2 个字节组成：0X0D 和 0X0A） */
			if(USART_RX_STA&0x4000)													//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;										//接收错误,重新开始
				else 
				{
					USART_RX_STA|=0x8000;											//接收完成了USART_RX_STA第15位接受状态位置1,回车之后的内容不再接收
					USART_ClearITPendingBit(USART1,USART_IT_RXNE);
				}
			}
			else																	//没接收到了0x0d
			{
				if(Res==0x0d)USART_RX_STA|=0x4000;                                  //判断Res接收到的是否为回车(2 个字节组成：0X0D 和 0X0A)
				else
				{
					/* USART_RX_STA&0X3FFF 确保USART_RX_STA的最高2位为0 */
					USART_RX_BUF[USART_RX_STA&0X3FFF]= Res;							//将接收到的一个字节放入USART_RX_BUF数组
					USART_RX_STA++;													//数组下标加一
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;				//数组下标越界，接收数据错误,重新开始接收	
				}
			}
		}
	}

} 


u8 Flag = 0;
void USART3_IRQHandler(void)
{  
	if(USART_GetFlagStatus(USART3,USART_FLAG_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART3_RX_BUF[0] = USART3->DR;
		Flag = 1;
	}
	
 
}	 
void ProtocolCpyData(void)
{
	memcpy(ProtocolString, USART3_RX_BUF, sizeof(USART3_RX_BUF));
	memset(USART3_RX_BUF, 0x00, sizeof(USART3_RX_BUF));
}
/***************************************************************************
串口协议数据解析
***************************************************************************/ 
void Protocol(void)
{
	//USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//禁能接收中断

	//判断数据包有效性

	switch (USART3_RX_BUF[0])
	{
		case run_car:	 g_newcarstate = enRUN;break;
		case back_car:  g_newcarstate = enBACK;break;
		case left_car:  g_newcarstate = enLEFT;break;
		case right_car: g_newcarstate = enRIGHT;break;
		case stop_car:  g_newcarstate = enSTOP;break;
		default: g_newcarstate = enSTOP; break;
		
	}

}


void CarStateOut(void)
{
	u16 s16LeftVoltage = 0;
	u16 s16RightVoltage = 0;
	switch (g_newcarstate)
	{
		case enSTOP: //停止
		{
			TIM_SetCompare3(TIM2,0);			  //TIM2与 u16RightMotorValue对比，不相同则翻转波形，调节PWM占空比
			TIM_SetCompare4(TIM2,0);
		} break; 					   

		case enRUN: //向前速度 800
		{
			LeftMotoPWM = 800;
			RightMotoPWM = 800;
		}break;	   

		case enLEFT://左转 
		{
			LeftMotoPWM = 0;
			RightMotoPWM = 300;

		}break;   
		
		case enRIGHT: //右转
		{
			LeftMotoPWM = 300;
			RightMotoPWM = 0;
		}break;	
		
		case enBACK: //后退速度 -800
		{
			LeftMotoPWM = -400;
			RightMotoPWM = -400;
		}break;
		
		case enTLEFT: //左旋
		{
			TIM_SetCompare3(TIM2,0);			  //TIM2与 u16RightMotorValue对比，不相同则翻转波形，调节PWM占空比
			TIM_SetCompare4(TIM2,400);
		}break;
		case enTRIGHT: //右旋
		{
			TIM_SetCompare3(TIM2,400);			  //TIM2与 u16RightMotorValue对比，不相同则翻转波形，调节PWM占空比
			TIM_SetCompare4(TIM2,0);
		}break;
		
		default: TIM2_PWM_CCR_Set(0,0); break; 					   //停止
	}
	
	LeftMotoPWM+=chaoValue;
	RightMotoPWM+=chaoValue;
    if(LeftMotoPWM<0)										 //当左电机PWM输出为负时 PB14设为正 PB15设为负 （PB14 15 分别控制TB6612fng驱动芯片，逻辑0 1可控制左电机正转反转）
    {	
	    GPIO_SetBits(GPIOB, GPIO_Pin_15 );				    
        GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
        s16LeftVoltage = (-LeftMotoPWM);
    }
    else 
    {	
        GPIO_SetBits(GPIOB, GPIO_Pin_14 );				    	 //当左电机PWM输出为正时 PB14设为负 PB15设为正 （PB14 15 分别控制TB6612fng驱动芯片，逻辑0 1可控制左电机正转反转）
        GPIO_ResetBits(GPIOB, GPIO_Pin_15 ); 
        s16LeftVoltage = LeftMotoPWM;
    }

    if(RightMotoPWM<0)
    {															 //当右电机PWM输出为负时 PB12设为正 PB13设为负 （PB12 13 分别控制TB6612fng驱动芯片，逻辑0 1可控制左电机正转反转）
      GPIO_SetBits(GPIOB, GPIO_Pin_12 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 );
      s16RightVoltage = (-RightMotoPWM);
    }
    else														//当右电机PWM输出为正时 PB12设为负 PB13设为正 （PB12 13 分别控制TB6612fng驱动芯片，逻辑0 1可控制左电机正转反转）
    {
	    GPIO_SetBits(GPIOB, GPIO_Pin_13 );				    
        GPIO_ResetBits(GPIOB, GPIO_Pin_12 );	
        s16RightVoltage = RightMotoPWM;
    }
	
	TIM2_PWM_CCR_Set(s16LeftVoltage,s16RightVoltage);
	LeftMotoPWM = 0;
	RightMotoPWM = 0;
}

#endif	

