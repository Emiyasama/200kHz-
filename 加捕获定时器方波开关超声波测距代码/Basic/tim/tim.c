

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1- ������


*/
#include "delay.h"
#include "led.h" //�����жϴ��������õ�LED����
#include "tim.h"
#include "oled0561.h"
//��ʱ��ʱ����㹫ʽTout = ((��װ��ֵ+1)*(Ԥ��Ƶϵ��+1))/ʱ��Ƶ��;
//���磺1�붨ʱ����װ��ֵ=9999��Ԥ��Ƶϵ��=7199



void TIM3_NVIC_Init (void){ //����TIM3�ж�����
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;	//������ռ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void TIM3_Init(u16 arr,u16 psc){  //TIM3 ��ʼ�� arr��װ��ֵ pscԤ��Ƶϵ��
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef  TIM2_ICInitStructure;
	  
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//ʹ��TIM3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
    
	  TIM3_NVIC_Init (); //����TIM3�ж�����
	  
		
    //��ʼ��TIM2���벶�����
	  
		TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);	

	
		//��ʼ��IO�ڣ�ģʽΪ����
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  //PA6 ���֮ǰ����  
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6 ����  
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);	//PA0 ����	

    TIM_TimeBaseInitStrue.TIM_Period=arr; //�����Զ���װ��ֵ
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ��
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //�������������
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӵķ�Ƶ���ӣ�����һ������ʱ���ã�һ����ΪTIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue); //TIM3��ʼ������
    TIM_ITConfig(TIM3, TIM_IT_Update|TIM_IT_CC1, ENABLE);//��������жϣ�����CC1TE�����ж�    
    TIM_Cmd(TIM3,DISABLE); //ʹ��TIM3
}


uint16_t t=0; //��ʱ����
u16 second2;//ֹͣʱ��
u8 Capture_State=0;//����״̬

void TIM3_IRQHandler(void){ //TIM3�жϴ�����
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){	
		//�ж��Ƿ�Ϊ��ʱ�ж�
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
				t++;//ÿ0.1s����һ�ν��е���
			  TIM_ClearITPendingBit(TIM3,TIM_FLAG_Update);
		}
		if(TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET){
		//�ж��Ƿ�Ϊ�����жϣ������趨���˴�Ϊ�����ش���
				if(Capture_State==0){
				//���ǵ�һ�β���ߵ�ƽʱ
						Capture_State|=1;//��״̬Ϊ��Ϊ1���������ǵ�һ�β��񵽸ߵ�ƽ
					  //second2 = t;  //���times����λ��us
						second2=TIM_GetCounter(TIM3);
				}
		    //������ǵ�һ�β���ߵ�ƽ�Ͳ�������
				TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);
		}
		
}
