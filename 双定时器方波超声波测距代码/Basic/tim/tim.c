
/*
���޸���־��
1- ������
2- ����TIM3Ϊ��ʱ��,TIM2Ϊ���벶����

*/

#include "led.h" //�����жϴ��������õ�LED����

#include "tim.h"

//��ʱ��ʱ����㹫ʽTout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
//���磺1�붨ʱ����װ��ֵ=9999��Ԥ��Ƶϵ��=7199

//void KEY_Init(void){ //΢�����صĽӿڳ�ʼ��
//	GPIO_InitTypeDef  GPIO_InitStructure; //����GPIO�ĳ�ʼ��ö�ٽṹ	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
//    GPIO_InitStructure.GPIO_Pin = KEY1 ; //ѡ��PA6�˿�                        
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //ѡ��IO�ӿڹ�����ʽ //��������       
////    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO�ӿ��ٶȣ�2/10/50MHz��    
//	GPIO_Init(KEYPORT,&GPIO_InitStructure);			
//}


void TIM3_Init(u16 arr,u16 psc){  //TIM3 ��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
		TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3
		//KEY_Init();//����ڰ�����ʼ��
    TIM3_NVIC_Init (); //����TIM3�ж�����
	      
    TIM_TimeBaseInitStrue.TIM_Period=arr; //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3��ʼ������
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ��TIM3�ж�    
    TIM_Cmd(TIM3,DISABLE); //ʹ��TIM3
}

void TIM2_Init(u16 arr,u16 psc){
		TIM_ICInitTypeDef TIM2_ICInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʹ�� TIM2 ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ�� GPIOA ʱ��
		TIM2_NVIC_Init (); //����TIM2�ж�����
	
		GPIO_InitStructure.GPIO_Pin = getPin; //PA0(getPin) ���֮ǰ����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����
		GPIO_Init(getPORT, &GPIO_InitStructure);
		GPIO_ResetBits(getPORT,getPin); //PA0(getPin) ����

		//��ʼ����ʱ�� 2 TIM2
		TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //Ԥ��Ƶ��
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //��ʼ�� TIMx ��ʱ�������λ

		//��ʼ�� TIM2 ���벶�����
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //ѡ������� IC1 ӳ�䵽 TI1 ��
		TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //�����ز���
		TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽 TI1 ��
		TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //���������Ƶ,����Ƶ
		TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
		TIM_ICInit(TIM2, &TIM2_ICInitStructure);

		TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);//ʹ��TIM2�ж�   
		TIM_Cmd(TIM2,DISABLE); //ʹ��TIM2
}


void TIM3_NVIC_Init (void){ //����TIM3�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//������ռ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_NVIC_Init (void){ //����TIM3�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x2;	//������ռ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u16 t;
u8 Capture_State;
void TIM3_IRQHandler(void){ //TIM3�жϴ�����
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){	//�ж��Ƿ���TIM3�ж�
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        //�˴�д���û��Լ��Ĵ������
				t++;
				//Capture_State = GPIO_ReadInputDataBit(KEYPORT,KEY1); //�ж�PA6��������ֵ,��ʼ����Ӧһֱ����Ϊ0
			
    }
}

float time_us;
float times;
void TIM2_IRQHandler(void){
		if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//���� 1 ���������¼�
		{
				time_us=TIM_GetCounter (TIM3);
				times=t*100+time_us;//times��λΪus
		}
		else{
				Capture_State=1;//Ϊ0ʱ�ſ�ʼ��ʱ
		}
}

