#include "USART.H"
#include "STC12C5A60S2.H"
#include "stdio.h"

#define Fosc 12000000
#define Bound 9600
u8 busy = 0;
u8 Rx_temp;
void UsartInit(void)
{
	TMOD &= 0X0F;
	TMOD |= 0X20;
//	TH1 = 256 - Fosc/12/32/Bound;
//	TL1 = 256 - Fosc/12/32/Bound;
	TH1 = 254;
	TL1 = 254;
	SCON = 0X50;
	PCON &= 0X7F;
	ES = 1;
	EA = 1;
	TI = 0;
	RI = 0;
	TR1 = 1;
}
void SendChar(unsigned char ch)
{
	while(busy);									// 等待串口处于可发送状态
	SBUF = ch;										// 向SBUF中写入字符让串口发送
	busy = 1;										// 置位忙标志位
}
char putchar (char ch)		                                                                                                                                            
{
	SendChar(ch);
	return ch;
}
void Usart_inter(void) interrupt 4
{
	if(TI)
	{
		TI = 0;
		busy = 0;
	}
	if(RI == 1)
	{
		Rx_temp = SBUF;
		RI = 0;	
	}	
}

