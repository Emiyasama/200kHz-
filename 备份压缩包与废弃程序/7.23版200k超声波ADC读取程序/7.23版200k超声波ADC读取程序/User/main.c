/*********************************************************************************************
模板制作：  杜洋工作室/洋桃电子
程序名：	光敏和电位器ADC读取程序
编写人：	杜洋	
编写时间：	2018年4月6日
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
#include "adc.h"
#include "key.h"
#include "usart.h"
#include "led.h"
#include <math.h>
#define PI 3.14159265
#define CAIYANG 285.714
//Khz
u8 fashe[256];//发射
u8 jieshou[256];//接受
u8 juli;//距离
u8 D[256];
u8 C[32];
u8 S[32];
u8 K1[8];
u8 K2[8];
u8 v[8];
u8 h[8];
u8 maxn;
u8 i;
u16 n; //这里伍明慧做了修改：因为u8能存储的最大值就是256，所以u8的采集点n永远小于256
u16 adcx;
u8 second1;//发射秒
u8 minutes1;//发射分
u8 second2;//接收秒
u8 minutes2;//接收分
extern vu16 ADC_DMA_IN[2]; //声明外部变量
extern u16 t;  //TIM3的计时秒钟
extern u16 m;  //TIM3的计时分钟

int main (void){//主程序
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	LED_Init();
	RELAY_Init();//继电器初始化
	KEY_Init();//KEY
	ADC_Configuration(); //ADC初始化设置
	I2C_Configuration();//I2C初始化
	TIM3_Init(9999,7199);//定时器初始化，定时1秒
	TIM2_Init(9999,7199);//计时功能，TIM2_INit(a,b),时间=（a+1）*（b+1）/72微秒（ms）
	
	//上电产生发射波形
	RELAY_1(1);      //电源和发射电路间的开关闭合
	delay_ms(2000); //等待波形起振完毕
	
	while(1){
		
		//发送3个周期的波
		RELAY_2(1);   //发射电路和探头间的开关闭合
		TIM_Cmd(TIM3,DISABLE); //使能TIM3，开始计时
		for(n=0;n<45;n++){
				fashe[n]=ADC_DMA_IN[1];        //发射波采集，暂不知此命令用时，慎用！！
		}
		delay_us(25);//因为 f = 200kHz，5个周期 = 25us
		RELAY_2(0);
		
		
		
		//采集接收波
		delay_us(440);//放置余波影响
		while(ADC_DMA_IN[1]>1){ //当接收波采集值
				second2 = t;
			  minutes2 = m;
			  for(n=0;n<45;n++){
						jieshou[n]=ADC_DMA_IN[1];
				}
		}
		
		//ADC数据显示在触摸屏上
        //计时器TIM3记录了传播时间，为minutes2分钟second2秒
		
		
		//计算相位（发射波and接收波）

		for(i=0;i<9;i++){
		  for(n=0;n<5;n++){
				D[n]=jieshou[9*n+i];
				//TODO:计算包含得阈值
				//if(V[i]>vmax){vmax=v[i];h=i;}
			}
		}
		for(n=0;n <32;){
			S[n]=sin(2*PI/CAIYANG*n*2*PI*200);//200Khz
			C[n]=cos(2*PI/CAIYANG*n*2*PI*200);
			
		  for(i=0;i<8;i++){
				D[n]=jieshou[8*n+i];//有逻辑问题【？？？？】
				//TODO:计算包含得阈值
				
				//if(V[i]>vmax){vmax=v[i];h=i;}
			}
			K1[n/8]+= D[n]*S[n];
			K2[n/8]+= D[n]*C[n];
			n+=8;
		}
		maxn=v[0];
		for(i=0;i<8;i++){
		  K1[i]=2*K1[i]/32;
		  K2[i]=2*K2[i]/32;
			v[i]=sqrt(D[0]*D[0])/D[0]*sqrt(K1[i]*K1[i]+K2[i]*K2[i]);
			h[i]=atan(K2[i]/K1[i])+PI/2;
			if(v[i]>maxn){
				maxn=v[i];
				n=i;
			}
		}
		//可知h[i]
		//TODO:计算相位
		//计算相角
		
			
		//计算距离

		
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



