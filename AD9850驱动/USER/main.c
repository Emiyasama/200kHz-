#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "AD9850.h"
//ALIENTEKս��STM32������ʵ��4
//����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 int main(void)
 {		
 	u8 t;
	u8 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
 	LED_Init();			     //LED�˿ڳ�ʼ��
	Init_AD9850();
	AD9850_Reset_Sreial();
	AD9850_Write_Serial(0,200000);
	 //delay_ms(2000);
	 
 	while(1)
	{
		
		
		
		
	}	 
 }

