#ifndef  __PWM_H
#define  __PWM_H
#include "sys.h"

//#define KEYPORT	    GPIOA	//����IO�ӿ���
//#define KEY1	      GPIO_Pin_6	//����IO�ӿ�

#define 	getPORT 	GPIOA
#define 	getPin  	GPIO_Pin_0   //�趨PA0Ϊ���������ն����벶���

void TIM3_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM3_NVIC_Init (void);
void TIM2_NVIC_Init (void);
//void KEY_Init(void);//��ʼ��

#endif
