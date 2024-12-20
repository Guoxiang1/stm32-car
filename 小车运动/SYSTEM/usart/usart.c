#include "sys.h"
#include "usart.h"	 
#include "moto.h"
#include <string.h>
#include "UltrasonicWave.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //���ʹ���˽���
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
u16 USART3_RX_STA=0;       //����״̬��� 


u8 ProtocolString[80] = {0};
/*С������״̬ö��*/



int g_newcarstate = 0; //  1ǰ2��3��4��0ֹͣ
short LeftMotoPWM = 0,RightMotoPWM = 0;
void uart_init(u32 bound){
  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	USART_DeInit(USART1);															//��λ���� ��λ���� ��λ���� 1
	
	//USART1_TX(���)   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;										//PA.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��GPIOA.9
	
	//USART1_RX(����)	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PA.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��GPIOA.10
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;							//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				    //�շ�ģʽ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	
	USART_Init(USART1, &USART_InitStructure); 										//��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);									//�������ڽ����ж�
	USART_Cmd(USART1, ENABLE);                    									//ʹ�ܴ���1 
	
}

void uart3_init(u32 bound){
  //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
		/* config GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB	, ENABLE);
	USART_DeInit(USART3);															//��λ���� ��λ���� ��λ���� 1
	
	//USART3_TX(���)   GPIOB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;										//PB.10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;									//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//��ʼ��GPIOA.9
	
	//USART3_RX(����)	  GPIOB.11��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;										//PB.11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;							//��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);											//��ʼ��GPIOA.10
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;								
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;					//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;							//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				    //�շ�ģʽ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	
	USART_Init(USART3, &USART_InitStructure); 										//��ʼ������3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);									//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    									//ʹ�ܴ���13
	
}

void USART1_IRQHandler(void)                										//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)							//�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);           									//��ȡ���յ�������
		if((USART_RX_STA&0x8000) == 0)												//����δ���
		{
			/*  ���յ��س����س��ı�ʾ�� 2 ���ֽ���ɣ�0X0D �� 0X0A�� */
			if(USART_RX_STA&0x4000)													//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;										//���մ���,���¿�ʼ
				else 
				{
					USART_RX_STA|=0x8000;											//���������USART_RX_STA��15λ����״̬λ��1,�س�֮������ݲ��ٽ���
					USART_ClearITPendingBit(USART1,USART_IT_RXNE);
				}
			}
			else																	//û���յ���0x0d
			{
				if(Res==0x0d)USART_RX_STA|=0x4000;                                  //�ж�Res���յ����Ƿ�Ϊ�س�(2 ���ֽ���ɣ�0X0D �� 0X0A)
				else
				{
					/* USART_RX_STA&0X3FFF ȷ��USART_RX_STA�����2λΪ0 */
					USART_RX_BUF[USART_RX_STA&0X3FFF]= Res;							//�����յ���һ���ֽڷ���USART_RX_BUF����
					USART_RX_STA++;													//�����±��һ
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;				//�����±�Խ�磬�������ݴ���,���¿�ʼ����	
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
����Э�����ݽ���
***************************************************************************/ 
void Protocol(void)
{
	//USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//���ܽ����ж�

	//�ж����ݰ���Ч��

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
		case enSTOP: //ֹͣ
		{
			TIM_SetCompare3(TIM2,0);			  //TIM2�� u16RightMotorValue�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
			TIM_SetCompare4(TIM2,0);
		} break; 					   

		case enRUN: //��ǰ�ٶ� 800
		{
			LeftMotoPWM = 800;
			RightMotoPWM = 800;
		}break;	   

		case enLEFT://��ת 
		{
			LeftMotoPWM = 0;
			RightMotoPWM = 300;

		}break;   
		
		case enRIGHT: //��ת
		{
			LeftMotoPWM = 300;
			RightMotoPWM = 0;
		}break;	
		
		case enBACK: //�����ٶ� -800
		{
			LeftMotoPWM = -400;
			RightMotoPWM = -400;
		}break;
		
		case enTLEFT: //����
		{
			TIM_SetCompare3(TIM2,0);			  //TIM2�� u16RightMotorValue�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
			TIM_SetCompare4(TIM2,400);
		}break;
		case enTRIGHT: //����
		{
			TIM_SetCompare3(TIM2,400);			  //TIM2�� u16RightMotorValue�Աȣ�����ͬ��ת���Σ�����PWMռ�ձ�
			TIM_SetCompare4(TIM2,0);
		}break;
		
		default: TIM2_PWM_CCR_Set(0,0); break; 					   //ֹͣ
	}
	
	LeftMotoPWM+=chaoValue;
	RightMotoPWM+=chaoValue;
    if(LeftMotoPWM<0)										 //������PWM���Ϊ��ʱ PB14��Ϊ�� PB15��Ϊ�� ��PB14 15 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
    {	
	    GPIO_SetBits(GPIOB, GPIO_Pin_15 );				    
        GPIO_ResetBits(GPIOB, GPIO_Pin_14 );
        s16LeftVoltage = (-LeftMotoPWM);
    }
    else 
    {	
        GPIO_SetBits(GPIOB, GPIO_Pin_14 );				    	 //������PWM���Ϊ��ʱ PB14��Ϊ�� PB15��Ϊ�� ��PB14 15 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
        GPIO_ResetBits(GPIOB, GPIO_Pin_15 ); 
        s16LeftVoltage = LeftMotoPWM;
    }

    if(RightMotoPWM<0)
    {															 //���ҵ��PWM���Ϊ��ʱ PB12��Ϊ�� PB13��Ϊ�� ��PB12 13 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
      GPIO_SetBits(GPIOB, GPIO_Pin_12 );				    
      GPIO_ResetBits(GPIOB, GPIO_Pin_13 );
      s16RightVoltage = (-RightMotoPWM);
    }
    else														//���ҵ��PWM���Ϊ��ʱ PB12��Ϊ�� PB13��Ϊ�� ��PB12 13 �ֱ����TB6612fng����оƬ���߼�0 1�ɿ���������ת��ת��
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

