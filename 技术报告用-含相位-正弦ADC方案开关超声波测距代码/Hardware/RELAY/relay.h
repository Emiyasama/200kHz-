#ifndef __RELAY_H
#define __RELAY_H	 
#include "sys.h"


#define RELAYPORT	GPIOA	//定义IO接口
#define RELAY1	GPIO_Pin_10	//定义IO接口  //这里伍明慧把PA14改成了PA10!!!!
#define RELAY2	GPIO_Pin_11	//定义IO接口  //这里伍明慧把PA13改成了PA11!!!!



void RELAY_Init(void);//继电器初始化
void RELAY_1(u8 c);//继电器控制1
void RELAY_2(u8 c);//继电器控制2
		 				    
#endif
