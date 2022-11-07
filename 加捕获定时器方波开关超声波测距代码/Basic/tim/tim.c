

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
#include "tim.h"
#include "oled0561.h"
//定时器时间计算公式Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
//例如：1秒定时，重装载值=9999，预分频系数=7199



void TIM3_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef  TIM2_ICInitStructure;
	  
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
    
	  TIM3_NVIC_Init (); //开启TIM3中断向量
	  
		
    //初始化TIM2输入捕获参数
	  
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 IC1映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);	

	
		//初始化IO口，模式为输入
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA6 清除之前设置  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6 输入  
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//PA0 下拉	

    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
    TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_CC1, ENABLE);//允许更新中断，允许CC1TE捕获中断    
    TIM_Cmd(TIM3,DISABLE); //使能TIM3
}


uint16_t t=0; //计时的秒
u16 second2;//停止时间
u8 Capture_State=0;//捕获状态

void TIM3_IRQHandler(void){ //TIM3中断处理函数
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){	
		//判断是否为定时中断
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				t++;//每0.1s进入一次进行递增
			  TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		}
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){
		//判断是否为捕获中断，按照设定，此处为上升沿触发
				if(Capture_State==0){
				//即是第一次捕获高电平时
						Capture_State|=1;//让状态为变为1，即不再是第一次捕获到高电平
					  //second2 = t;  //输出times，单位是us
						second2=TIM_GetCounter(TIM3);
				}
		    //如果不是第一次捕获高电平就不处理它
				TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);
		}
		
}
