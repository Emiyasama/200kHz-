#ifndef  __PWM_H
#define  __PWM_H
#include "sys.h"

void TIM3_Init(u16 arr,u16 psc);
void TIM3_NVIC_Init (void);
void TIM2_Init(u16 arr,u16 psc);//新加的
void TIM2_NVIC_Init (void);//新加的定时器

#endif
