#ifndef __CS_H
#define __CS_H 	
#include "sys.h"
#define uint unsigned int
#define TRIG_Send PBout(8)
#define ECHO_Reci PBin(9)
#define distance Senor_Using()
void CH_SR04_Init(void);  //������ģ��������ó�ʼ��
float Senor_Using(void);  //��ຯ��������ֵ��Ϊ����
void NVIC_Config(void);   //�ж�����


#endif

