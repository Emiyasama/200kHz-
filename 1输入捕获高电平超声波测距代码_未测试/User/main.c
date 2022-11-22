#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "tim.h"
#include "usart.h"
#include "led.h"
#include "AD9850_2.h"  //使用的是版本2
#include <math.h>
#include "lm75a.h"

extern u8 TIM2CH1_CAPTURE_STA;  //输入捕获状态
extern u16  TIM2CH1_CAPTURE_VAL; //输入捕获值
float Ultra_v;//超声波传播速度
float Temp;//当前温度
float Ultra_s;//当前测出的距离
int OLED_s;//用于在OLED上显示的距离数据，单位为um
/**
功能列表：
1、发出与关闭DDS：200kHz方波与正弦波
2、输出控制波形：开始发波时输出1，结束发波时输出0
				方案：修改led.c，将输出引脚选为LED1的那个引脚（PB0），选择推挽输出
				
3、输入捕获高电平时间：即传播时间
				方案：修改tim.c
4、I2C测温度传感器温度：Ultra_v = 331.5 + 0.6 * Temp;
5、计算传播距离：Ultra_s = 0.5 * Ultra_v * Temp;
6、OLED显示结果：I2C_SCL = PB6; I2C_SDA = PB7;
**/

int main(){
	
	u32 time=0;//输入捕获的中间变量
	u8 buffer[3];//读取的温度数据
	
	//0、基本初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置 NVIC 中断分组2(2位抢占优先级，2位响应优先级)
	delay_ms(500); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	I2C_Configuration();//I2C初始化
	OLED0561_Init(); //OLED初始化
	USART3_Init(115200);  //串口初始化：PB11—-TXD，PB10--RXD
	Init_AD9850_2();//DDS初始化
	TIM2_Cap_Init(0XFFFF,72-1);  //以 1Mhz 的频率计数（输入捕获）
	
	while(1){
		
			while(GPIO_ReadInputDataBit(getPORT,getPin)==1){}//做一层保护，当输入捕获引脚回归0时才重新开始发波
				
			TIM2CH1_CAPTURE_STA=0;  //开启下一次捕获	
			//1、发出DDS波形
			AD9850_2_Write_Serial(0,200000);//发出200k波形
			//2、输出控制波形
			GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //PB0引脚信号控制，发波时置1
			
			//3、输入捕获高电平时间
			if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
			{
					time=TIM2CH1_CAPTURE_STA&0X3F;
					time*=65536; //溢出时间总和
					time+=TIM2CH1_CAPTURE_VAL; //得到总的高电平时间
					USART3_printf("highValTime = %d us\r\n",time);
					//TIM2CH1_CAPTURE_STA=0;  //开启下一次捕获
			}
			
			//6、OLED显示时间
			OLED_DISPLAY_8x16_BUFFER(4,"time ="); //显示时间字符
			OLED_DISPLAY_8x16(2,7*8,time/1000000+0x30);  //秒位
			OLED_DISPLAY_8x16(2,8*8,time/100000+0x30);  
			OLED_DISPLAY_8x16(2,9*8,time/10000+0x30);
			OLED_DISPLAY_8x16(2,10*8,time/1000+0x30);    //ms位
			OLED_DISPLAY_8x16(2,11*8,time/100+0x30);
			OLED_DISPLAY_8x16(2,12*8,time/10+0x30);
			OLED_DISPLAY_8x16(2,13*8,time%10+0x30);      //us位
			OLED_DISPLAY_8x16(2,15*8,'u');               //单位us
			OLED_DISPLAY_8x16(2,16*8,'s');
			
			//4、温度传感器测温
			LM75A_GetTemp(buffer); //读取LM75A的温度数据
			Temp = buffer[1] + (buffer[2]/10)*0.1 + (buffer[2]%10)*0.01; //整数+小数点后一位+小数点后二位
			if(buffer[0]==1){//为0时温度值为正，为1时温度值为负
						Temp = -Temp;//若值为负则记得把负号加进Temp里
			}
			Ultra_v = 331.5 + 0.6*Temp; //单位是米每秒
			USART3_printf("Ultra_v = %f m/s \r\n",Ultra_v);
			
			OLED_DISPLAY_8x16_BUFFER(4,"   T ="); //显示时间字符
			if(buffer[0]==1)OLED_DISPLAY_8x16(4,7*8,'-');   //如果第1组为1即是负温度
			OLED_DISPLAY_8x16(4,8*8,buffer[1]/10+0x30);     //显示温度值整数部分
			OLED_DISPLAY_8x16(4,9*8,buffer[1]%10+0x30);
			OLED_DISPLAY_8x16(4,10*8,'.');                  //显示小数点
			OLED_DISPLAY_8x16(4,11*8,buffer[2]/10+0x30);    //显示小数部分
			OLED_DISPLAY_8x16(4,12*8,buffer[2]%10+0x30);
			OLED_DISPLAY_8x16(4,13*8,'C');                  //显示单位
			
			
			//5、计算传播距离
			Ultra_s = 0.5 * Ultra_v * (time*0.000001); //距离 = 速度(m/s)*时间(us变s)/2
			USART3_printf("Ultra_s = %f m\r\n",Ultra_s);
			USART3_printf("Ultra_s = %f cm\r\n",Ultra_s*100);
			
			//6、OLED显示距离
			OLED_s =(Ultra_s*1000000);
			OLED_DISPLAY_8x16_BUFFER(4,"   s ="); //显示时间字符
			OLED_DISPLAY_8x16(2,7*8,OLED_s/1000000+0x30);  //秒位
			OLED_DISPLAY_8x16(2,8*8,OLED_s/100000+0x30);  
			OLED_DISPLAY_8x16(2,9*8,OLED_s/10000+0x30);
			OLED_DISPLAY_8x16(2,10*8,OLED_s/1000+0x30);    //mm位
			OLED_DISPLAY_8x16(2,11*8,OLED_s/100+0x30);
			OLED_DISPLAY_8x16(2,12*8,OLED_s/10+0x30);
			OLED_DISPLAY_8x16(2,13*8,OLED_s%10+0x30);      //um位
			OLED_DISPLAY_8x16(2,15*8,'u');               //单位um
			OLED_DISPLAY_8x16(2,16*8,'m');
			
			AD9850_2_Reset_Sreial();//1、关闭波形
			GPIO_WriteBit(LEDPORT,LED1,(BitAction)(0)); //2、PB0引脚信号控制，关波时置0
			delay_ms(1000);//第二层保护，等待波形过去
	}
}


































//float juli;//距离
////float times;
//extern float time_us;
//int stop_int=0;

//extern vu16 ADC_DMA_IN[2]; //声明外部变量
//extern u16 t;  //TIM3的计时秒钟
//extern u8 Capture_State; //初始值为1
//extern float times;

//int main (void){//主程序
//	delay_ms(500); //上电时等待其他器件就绪
//	RCC_Configuration(); //系统时钟初始化 
//	I2C_Configuration();//I2C初始化
//	//OLED0561_Init(); //OLED初始化
//	USART3_Init(115200);  //串口初始化：PB11—-TXD，PB10--RXD
//	USART3_printf(" hello!!!!!!\r\n");
//	TIM3_Init(99,71);//定时器初始化，(1/72M*psc)秒计数一次arr值，到达(arr+1)设定值时产生中断
//	//(999,71)————每1ms进入一次中断
//	USART3_printf(" tim3 init over\r\n");
//	TIM2_Init(99,71);//输入捕获定时器
//	
//	//Init_AD9850_2();
//	USART3_printf(" tim2 init over\r\n");
//	
//	while(1){
//			int getpin;
//			//初始化的Capture_State为1，当中断里读到引脚为0时才会开始发射
////			TIM_Cmd(TIM2,ENABLE); //使能TIM2，中断里判断引脚是否为0
//			TIM_Cmd(TIM3,ENABLE); //使能TIM3
//			while(Capture_State==1 && GPIO_ReadInputDataBit(getPORT,getPin)==1){  //等待接收端变回0才开始准备下一次发射
//					//USART3_printf("waiting for 0\r\n");
//			}
//			getpin = GPIO_ReadInputDataBit(getPORT,getPin);//查看此时是因为什么而跳出循环
//			Capture_State=0; //跳出循环的唯一方法是PA0为0，此时把状态手动归0
//			USART3_printf("getpin = %d，State = %d,start timing!\r\n",getpin,Capture_State);
//      //AD9850_2_Write_Serial(0,200000);  //发射200k
//			TIM_Cmd(TIM2,ENABLE); //使能TIM2，中断里判断引脚是否为0
//			t=0;//重装载值清零，指经过了多少个周期
//			
//		  //接收部分
//		  while(Capture_State==0 && GPIO_ReadInputDataBit(getPORT,getPin)==0){  //等待输入端变为1 //输入捕获时会将状态置1跳出循环
//					//USART3_printf("waiting for 1\r\n");
//			}
//			getpin = GPIO_ReadInputDataBit(getPORT,getPin);//查看此时是因为什么而跳出循环
//			//time_us=TIM_GetCounter (TIM3);
//			USART3_printf("over\r\n");
//			USART3_printf("getpin = %d,State = %d,timing over!\r\n",getpin,Capture_State);
//			
//			//times=t*100+time_us;//times单位为us
//			//time_us=TIM_GetCounter (TIM2);
//			//USART3_printf("time_us=%d\r\n",time_us);
//			//times=t*100+time_us;//times单位为us
//			times=t*100;
//			Capture_State=1;  //为1后停止计时
//			USART3_printf("StateNow=%d\r\n",Capture_State);
//			USART3_printf("time = %f us\r\n",times);
//			juli= V *times/10000;//计算出juli，单位为um
//			USART3_printf("juli = %f m\r\n",juli);
//			
//			/****************测试用暂停代码********************/
//			stop_int++;
//			if(stop_int==10){
//					stop_int=0;
//					delay_ms(5000);
//				  USART3_printf("-----------already 10 results---------\r\n");
//			}
//			/****************测试用暂停代码*****************/
//			TIM_Cmd(TIM2,DISABLE); //使能TIM2，中断里判断引脚是否为0
//			TIM_Cmd(TIM3,DISABLE); //使能TIM3
//			
//			//AD9850_2_Reset_Sreial();
//			//delay_ms(1000);//等待波形过去
//	}
//	
//}	
	



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



