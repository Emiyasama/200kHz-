#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "tim.h"
#include "usart.h"
#include "led.h"
#include "AD9850_2.h"
#include <math.h>
#include "table_fft.h"
#include "stm32_dsp.h"
#define V 344

float juli;//距离
//float times;
//float time_us;
int stop_int=0;

extern vu16 ADC_DMA_IN[2]; //声明外部变量
extern u16 t;  //TIM3的计时秒钟
extern u8 Capture_State;
extern float times;

int main (void){//主程序
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	I2C_Configuration();//I2C初始化
	//OLED0561_Init(); //OLED初始化
	USART3_Init(115200);  //串口初始化：PB11—-TXD，PB10--RXD
	
	TIM3_Init(99,71);//定时器初始化，(1/72M*psc)秒计数一次arr值，到达(arr+1)设定值时产生中断
	//(999,71)————每1ms进入一次中断
	TIM2_Init(99,71);//输入捕获定时器
	
	//Init_AD9850_2();
	USART3_printf(" init over\r\n");
	
	while(1){
			//初始化的Capture_State为1，当中断里读到引脚为0时才会开始发射
			TIM_Cmd(TIM2,ENABLE); //使能TIM2，中断里判断引脚是否为0
			while(Capture_State==1){  //等待接收端变回0才开始准备下一次发射
					//USART3_printf("waiting for 0\r\n");
			}
		
			USART3_printf("Capture_State = %d,start timing!\r\n",Capture_State);
      //AD9850_2_Write_Serial(0,200000);  //发射200k
			TIM_Cmd(TIM3,ENABLE); //使能TIM3
			t=0;//重装载值清零，指经过了多少个周期
			
		  //接收部分
		  while(Capture_State==0){  //等待输入端变为1
					//USART3_printf("waiting for 1\r\n");
			}
			//time_us=TIM_GetCounter (TIM3);
			USART3_printf("Capture_State = %d,timing over!\r\n",Capture_State);
			USART3_printf("over\r\n");
			
			//times=t*100+time_us;//times单位为us
			USART3_printf("time = %f us\r\n",times);
			juli= V *times/10000;//计算出juli，单位为um
			USART3_printf("juli = %f m\r\n",juli);
			
			/****************测试用暂停代码********************/
			stop_int++;
			if(stop_int==50){
					stop_int=0;
					delay_ms(5000);
				  USART3_printf("-----------already 10 results---------\r\n");
			}
			/****************测试用暂停代码*****************/
			
			
			//AD9850_2_Reset_Sreial();
			//delay_ms(1000);//等待波形过去
	}
	
}	
	



/*

【变量定义】
u32     a; //定义32位无符号变量a
u16     a; //定义16位无符号变量a
u8     a; //定义8位无符号变量a
vu32     a; //定义易变的32位无符号变量a
vu16     a; //定义易变的 16位无符号变量a
vu8     a; //定义易变的 8位无符号变量a
uc32     a; //定义只读的32位无符号变量a
uc16     a; //定义只读 的16位无符号变量a
uc8     a; //定义只读 的8位无符号变量a

#define ONE  1   //宏定义

delay_us(1); //延时1微秒
delay_ms(1); //延时1毫秒
delay_s(1); //延时1秒

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED控制

*/



