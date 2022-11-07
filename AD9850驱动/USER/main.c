#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "AD9850.h"
//ALIENTEK战舰STM32开发板实验4
//串口实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 int main(void)
 {		
 	u8 t;
	u8 len;	
	u16 times=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
 	LED_Init();			     //LED端口初始化
	Init_AD9850();
	AD9850_Reset_Sreial();
	AD9850_Write_Serial(0,200000);
	 //delay_ms(2000);
	 
 	while(1)
	{
		
		
		
		
	}	 
 }

