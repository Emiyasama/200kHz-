

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1- 创建。


*/
#include "delay.h"
#include "led.h" //因在中断处理函数中用到LED驱动
#include "key.h"
#include "tim.h"
#include "oled0561.h"
#include "touch_key.h"
//定时器时间计算公式Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
//例如：1秒定时，重装载值=9999，预分频系数=7199

void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
    TIM3_NVIC_Init (); //开启TIM3中断向量
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能TIM3中断    
    TIM_Cmd(TIM3,DISABLE); //使能TIM3
}

void TIM3_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Init(u16 arr,u16 psc){  //TIM2 初始化 arr重装载值 psc预分频系数
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能TIM2
    TIM2_NVIC_Init (); //开启TIM2中断向量
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStrue); //TIM2初始化设置
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能TIM2中断    
    TIM_Cmd(TIM2,ENABLE); //使能TIM2
}


//新加的

void TIM2_NVIC_Init (void){ //开启TIM2中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



u16 t=0; //计时的秒
u16 m=0; //计时的分钟
void TIM3_IRQHandler(void){ //TIM3中断处理函数
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){	//判断是否是TIM3中断
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

        //此处写入用户自己的处理程序
				t++;        //每1s自增1
				if(t==60){ 
						t=0;    //进位归零
					  m++;    //进位，表分钟
				}
				
}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
