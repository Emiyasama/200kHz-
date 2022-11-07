#ifndef  __PWM_H
#define  __PWM_H
#include "sys.h"

//#define KEYPORT	    GPIOA	//定义IO接口组
//#define KEY1	      GPIO_Pin_6	//定义IO接口

#define 	getPORT 	GPIOA
#define 	getPin  	GPIO_Pin_0   //设定PA0为超声波接收端输入捕获口

void TIM3_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);
void TIM3_NVIC_Init (void);
void TIM2_NVIC_Init (void);
//void KEY_Init(void);//初始化

#endif
