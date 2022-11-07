
/*
《修改日志》
1- 创建。
2- 配置TIM3为计时器,TIM2为输入捕获器

*/

#include "led.h" //因在中断处理函数中用到LED驱动

#include "tim.h"

//定时器时间计算公式Tout = ((重装载值+1)*(预分频系数+1))/时钟频率;
//例如：1秒定时，重装载值=9999，预分频系数=7199

//void KEY_Init(void){ //微动开关的接口初始化
//	GPIO_InitTypeDef  GPIO_InitStructure; //定义GPIO的初始化枚举结构	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
//    GPIO_InitStructure.GPIO_Pin = KEY1 ; //选择PA6端口                        
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //选择IO接口工作方式 //下拉输入       
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
//	GPIO_Init(KEYPORT,&GPIO_InitStructure);			
//}


void TIM3_Init(u16 arr,u16 psc){  //TIM3 初始化 arr重装载值 psc预分频系数
		TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//使能TIM3
		//KEY_Init();//输入口按键初始化
    TIM3_NVIC_Init (); //开启TIM3中断向量
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3初始化设置
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能TIM3中断    
    TIM_Cmd(TIM3,DISABLE); //使能TIM3
}

void TIM2_Init(u16 arr,u16 psc){
		TIM_ICInitTypeDef TIM2_ICInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能 TIM2 时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能 GPIOA 时钟
		TIM2_NVIC_Init (); //开启TIM2中断向量
	
		GPIO_InitStructure.GPIO_Pin = getPin; //PA0(getPin) 清除之前设置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入
		GPIO_Init(getPORT, &GPIO_InitStructure);
		GPIO_ResetBits(getPORT,getPin); //PA0(getPin) 下拉

		//初始化定时器 2 TIM2
		TIM_TimeBaseStructure.TIM_Period = arr; //设定计数器自动重装值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //预分频器
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //初始化 TIMx 的时间基数单位

		//初始化 TIM2 输入捕获参数
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择输入端 IC1 映射到 TI1 上
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
		TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);

		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);//使能TIM2中断   
		TIM_Cmd(TIM2,DISABLE); //使能TIM2
}


void TIM3_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_NVIC_Init (void){ //开启TIM3中断向量
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;	//设置抢占和子优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u16 t;
u8 Capture_State;
void TIM3_IRQHandler(void){ //TIM3中断处理函数
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){	//判断是否是TIM3中断
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        //此处写入用户自己的处理程序
				t++;
				//Capture_State = GPIO_ReadInputDataBit(KEYPORT,KEY1); //判断PA6输入引脚值,初始化后应一直保持为0
			
    }
}

float time_us;
float times;
void TIM2_IRQHandler(void){
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//捕获 1 发生捕获事件
		{
				time_us=TIM_GetCounter (TIM3);
				times=t*100+time_us;//times单位为us
		}
		else{
				Capture_State=1;//为0时才开始计时
		}
}

