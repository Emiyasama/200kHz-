/*********************************************************************************************
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
#include "oled0561.h"
#include "tim.h"
#include "bsp_adc.h"
#include "usart.h"
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

u16 second1;//发射秒
u8 minutes1;//发射分
u16 second2;//接收秒
u8 minutes2;//接收分
u16 times;
extern vu16 ADC_DMA_IN[2]; //声明外部变量
extern uint32_t t;  //TIM3的计时秒钟
//extern u8 TIM3_flag;//TIM3的开启标志位  //TIM3留着用来验证正弦波传播时间
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

void theta_calculate(int32_t *IBUFOUT)   //拿官方计算幅值的函数改的fft计算相角函数
{
	s16 lx,ly;
	u32 i;
	for(i=0;i<NPT/2;i++)//由于FFT的频域结果是关于奈奎斯特频率对称的，所以只计算一半
	{
		lx = (IBUFOUT[i] << 16)>>16;      //取低十六位，虚部
		ly = (IBUFOUT[i] >> 16);          //取高十六位，实部
		float X = NPT*((float)lx)/32768;
		float Y = NPT*((float)ly)/32768;
		float theta = atan(X/Y);
		//计算相角的公式为theta=atan(虚部/实部);
		OUTPUT_THETA[i] = (u32)(theta);
		//对于theta来说没必要乘回来32768，反正都约掉了
		
	}//这些就是计算振幅IBUFOUT[i] = sqrt(lx*lx+ly*ly)*2/NPT
	 //之所以先除以32768后乘以65536是为了符合浮点数的计算规律
	 //OUTPUT_MAG[0] = OUTPUT_MAG[0]/2;//这个是直流分量，不需要乘以2
}

int main (void){//主程序
	int n=0;//用于for循环存储ADC数值
	int i=0;
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //OLED初始化
	OLED_DISPLAY_8x16_BUFFER(0,"      TEST    "); //显示字符串
	TIM3_Init(99,71);//定时器初始化，定时10us
	OLED_DISPLAY_8x16_BUFFER(2,"juli/um time/us"); //显示字符串
	USART3_Init(115200);  //串口初始化
	Init_AD9850();
	Init_AD9850_2();
  ADCx_Init();
  
	while(1){
      
			AD9850_Write_Serial(0,20000);  //20k
	    AD9850_2_Write_Serial(0,200000);  //200k
		  
		  OLED_DISPLAY_8x16(0,15*8,i+0x30);
		  USART3_printf("i = %d\r\n",i);
		  //TIM3_flag=1;//开启计时
		  TIM_Cmd(TIM3,ENABLE); //使能TIM3
			second1=t;
			
		  //OLED_DISPLAY_8x16_BUFFER(4," time = "); //显示字符串
		  //100us即发20个200k和2个20k
		  delay_us(100);   //10cm的距离需要约400us才会传回来，不慌
		   
			//关闭发波
		  AD9850_Reset_Sreial();
		  AD9850_2_Reset_Sreial();
		

		  //ADC测试部分
//		  ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
//		 
//		  USART3_printf("ADC = %f\r\n",ADC_ConvertedValueLocal[1]);
//			
		  
			
		  while((float)ADC_ConvertedValue[1]/4096*3.3<=0.1){
					
			}
		  //TIM3_flag=0;//停止计时
		  //times=t*10;   //t指有几个10us，time单位为us
			second2=t;   //让t=200us//这里注意我定时器以及改成了1ms一次中断！
			TIM_Cmd(TIM3,DISABLE); //使能TIM3
			
			for(n=0;n<=89;n++){
					ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
				  input[n]= ADC_ConvertedValueLocal[1];
			}
			
			if(second2>=second1){
					times=second2-second1;    //times单位为us
			}
			else{
					times=0xffff+second2-second1;  //溢出了，需要加回来0xffff
			}
			cr4_fft_256_stm32(output,input,NPT);
			theta_calculate(output);
      
			juli= V *(times-OUTPUT_THETA[0]/(2*PI*0.2))/2;//计算出juli,单位um
			//这里时间是times-theta/2pi*（1/200k）,但因为times单位是us，juli单位是um，所以200k变成了0.2
      
			OLED_DISPLAY_8x16_BUFFER(4," time = "); //显示字符串
		  OLED_DISPLAY_8x16(4,9*8,times/10000+0x30);
			OLED_DISPLAY_8x16(4,9*8,times%10000/1000+0x30);
		  OLED_DISPLAY_8x16(4,10*8,times%1000/100+0x30);
			OLED_DISPLAY_8x16(4,11*8,times%100/10+0x30);
			OLED_DISPLAY_8x16(4,12*8,times%10+0x30);

			//距离为int
			OLED_DISPLAY_8x16(6,8*8,juli/1000000+0x30);
			OLED_DISPLAY_8x16(6,9*8,juli%1000000/100000+0x30);
			OLED_DISPLAY_8x16(6,10*8,juli%100000/10000+0x30);
			OLED_DISPLAY_8x16(6,11*8,juli%10000/1000+0x30);
			OLED_DISPLAY_8x16(6,12*8,juli%1000/100+0x30);
			OLED_DISPLAY_8x16(6,13*8,juli%100/10+0x30);
			OLED_DISPLAY_8x16(6,14*8,juli%10+0x30);
      OLED_DISPLAY_8x16_BUFFER(6," juli=");

			i++;
			if(i==10){
					i=0;
			}
			delay_us(100);//等待发射的后几个波路过接收端
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



