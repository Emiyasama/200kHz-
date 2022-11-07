/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	光敏和电位器ADC读取程序
编写人：	伍明慧	
编写时间：	2022年8月5日
硬件支持：	STM32F103C8   外部晶振8MHz RCC函数设置主频72MHz　  

修改日志：　　
1-	
	
							
说明：
 # 本模板加载了STM32F103内部的RCC时钟设置，并加入了利用滴答定时器的延时函数。
 # 可根据自己的需要增加或删减。

*********************************************************************************************/
#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "touch_key.h"
#include "relay.h"
#include "oled0561.h"
#include "tim.h"
#include "bsp_adc.h"
#include "key.h"
#include "usart.h"
#include "led.h"
#include "AD9850.h"
#include "AD9850_2.h"
#include <math.h>
#include "table_fft.h"
#include "stm32_dsp.h"
//#include "bsp_tpad.h"
#define PI 3.14159265
#define V 344
#define NPT 90 //采点次数=90

float fashe[90];//发射
float jieshou[90];//接受
int juli;//距离【mm】
int i;

u8 second1;//发射秒
u8 minutes1;//发射分
u8 second2;//接收秒
u8 minutes2;//接收分
u16 times;
extern vu16 ADC_DMA_IN[2]; //声明外部变量
extern u16 t;  //TIM3的计时秒钟
extern u8 TIM3_flag;//TIM3的开启标志位  //TIM3留着用来验证正弦波传播时间
extern u8 Capture_State;
extern u8 flag;
char p[10];
extern uint16_t TIM5_GetTime(void);
int32_t input[90];
int32_t output[90];
int32_t OUTPUT_THETA[90];

// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// 局部变量，用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[NOFCHANEL];


int main (void){//主程序
	//int n=0;//用于for循环存储ADC数值
	int i=0;
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //OLED初始化
	OLED_DISPLAY_8x16_BUFFER(0,"      TEST    "); //显示字符串
	TIM3_Init(39,17);//定时器初始化，定时10us
	OLED_DISPLAY_8x16_BUFFER(2,"juli/cmm time/us"); //显示字符串
	USART3_Init(115200);  //串口初始化
	Init_AD9850();
	Init_AD9850_2();
  ADCx_Init();
  
	while(1){
      
			AD9850_Write_Serial(0,20000);  //20k
	    AD9850_2_Write_Serial(0,200000);  //200k
		  TIM3_flag=1;//开启计时
		  
		  //100us即发20个200k和2个20k
		  delay_us(100);   //5cm的距离需要约200us才会传回来，不慌
		
			//关闭发波
		  AD9850_Reset_Sreial();
		  AD9850_2_Reset_Sreial();
		

		  //ADC测试部分
		  //ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3-1;
		 
		  //USART3_printf("ADC = %f\r\n",ADC_ConvertedValueLocal[1]);
			
		  OLED_DISPLAY_8x16(0,15*8,i+0x30);
			
		  while((float)ADC_ConvertedValue[1]/4096*3.3<=0.05){
					
			}
			OLED_DISPLAY_8x16_BUFFER(4," time = "); //显示字符串
		  TIM3_flag=0;//停止计时
		  //times=t*10;   //t指有几个10us，time单位为us
			times=t*10;   //让t=200us//这里注意我定时器以及改成了10us一次中断！
			juli= V *times;//计算出juli
      
		  
			OLED_DISPLAY_8x16(4,9*8,times/1000+0x30);
		  OLED_DISPLAY_8x16(4,10*8,times%1000/100+0x30);
			OLED_DISPLAY_8x16(4,11*8,times%100/10+0x30);
			OLED_DISPLAY_8x16(4,12*8,times%10+0x30);

			
			//距离为int
			OLED_DISPLAY_8x16(6,6*8,juli/1000000+0x30);
			OLED_DISPLAY_8x16(6,7*8,juli%1000000/100000+0x30);
			OLED_DISPLAY_8x16(6,8*8,juli%100000/10000+0x30);
			OLED_DISPLAY_8x16(6,9*8,juli%10000/1000+0x30);
			OLED_DISPLAY_8x16(6,10*8,juli%1000/100+0x30);
			OLED_DISPLAY_8x16(6,11*8,juli%100/10+0x30);
			OLED_DISPLAY_8x16(6,12*8,juli%10+0x30);
      OLED_DISPLAY_8x16_BUFFER(6," juli=");

			i++;
			if(i==10){
					i=0;
			}
			delay_us(100);//等待发射的后几个波路过接收端
			//OLED_DISPLAY_8x16_BUFFER(0,"H"); //显示字符串
			//OLED_DISPLAY_8x16(6,15*8,8+0x30);
	}
	
}	
	


/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
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



