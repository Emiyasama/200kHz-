#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"


#define RELAYPORT	GPIOA	//����IO�ӿ�
#define RELAY1	GPIO_Pin_10	//����IO�ӿ�  //���������۰�PA14�ĳ���PA10!!!!
#define RELAY2	GPIO_Pin_11	//����IO�ӿ�  //���������۰�PA13�ĳ���PA11!!!!



void RELAY_Init(void);//�̵�����ʼ��
void RELAY_1(u8 c);//�̵�������1
void RELAY_2(u8 c);//�̵�������2
		 				    
#endif
