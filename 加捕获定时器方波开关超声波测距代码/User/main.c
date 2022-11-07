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

int juli;//距离

u16 second1;//发射秒
extern u16 second2;//接收秒
u16 times;

extern vu16 ADC_DMA_IN[2]; //声明外部变量
extern u16 t;  //TIM3的计时秒钟
extern u8 Capture_State;

int main (void){//主程序
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	I2C_Configuration();//I2C初始化
	//OLED0561_Init(); //OLED初始化
	//OLED_DISPLAY_8x16_BUFFER(0,"   Init Over    "); //显示字符串
	USART3_Init(115200);  //串口初始化
	TIM3_Init(9999,719);//定时器初始化，定时10u秒
	USART3_printf("init down\r\n");
	//Init_AD9850_2();
	//OLED_DISPLAY_8x16_BUFFER(2,"  AD9850 Over   "); //显示字符串
  
	while(1){
			
	    //AD9850_2_Write_Serial(0,200000);  //200k
			TIM_Cmd(TIM3,ENABLE); //使能TIM3
      t=0;//重装载值清零，指经过了多少个周期
		  Capture_State=0;//状态归零，即现在是第0次上升沿，等待第一次上升沿时会进入中断计时停止
		  USART3_printf("start\r\n");
		  //接收部分
		  while(Capture_State==0){  //原ADC接口现连接PA6作为定时器上升沿触发口
					//保持在循环内等待，等待第一次上升沿触发
					USART3_printf("running\r\n");
			}
			USART3_printf("over\r\n");
			TIM_Cmd(TIM3,DISABLE); //关闭TIM3
			times=t*10+second2;//times单位为us
			juli= V *times;//计算出juli，单位为um
      USART3_printf("time = %d us\r\n",times);
			
//		  OLED_DISPLAY_8x16_BUFFER(4," times = "); //显示字符串
//		  OLED_DISPLAY_8x16(4,10*8,times/100+0x30);
//			OLED_DISPLAY_8x16(4,11*8,times%100/10+0x30);
//			OLED_DISPLAY_8x16(4,10*8,times%10+0x30);
//			
//			OLED_DISPLAY_8x16_BUFFER(4,"s(m)="); //显示字符串
//		  OLED_DISPLAY_8x16(4,10*8,juli/100+0x30);
//			OLED_DISPLAY_8x16(4,11*8,juli%100/10+0x30);
//			OLED_DISPLAY_8x16(4,10*8,juli%10+0x30);			
			
			
			//AD9850_2_Reset_Sreial();
			delay_ms(1000);//等待发射的后几个波路过接收端
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



