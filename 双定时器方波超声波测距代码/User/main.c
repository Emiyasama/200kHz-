#include "stm32f10x.h" //STM32ͷ�ļ�
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

float juli;//����
//float times;
//float time_us;
int stop_int=0;

extern vu16 ADC_DMA_IN[2]; //�����ⲿ����
extern u16 t;  //TIM3�ļ�ʱ����
extern u8 Capture_State;
extern float times;

int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	I2C_Configuration();//I2C��ʼ��
	//OLED0561_Init(); //OLED��ʼ��
	USART3_Init(115200);  //���ڳ�ʼ����PB11��-TXD��PB10--RXD
	
	TIM3_Init(99,71);//��ʱ����ʼ����(1/72M*psc)�����һ��arrֵ������(arr+1)�趨ֵʱ�����ж�
	//(999,71)��������ÿ1ms����һ���ж�
	TIM2_Init(99,71);//���벶��ʱ��
	
	//Init_AD9850_2();
	USART3_printf(" init over\r\n");
	
	while(1){
			//��ʼ����Capture_StateΪ1�����ж����������Ϊ0ʱ�ŻῪʼ����
			TIM_Cmd(TIM2,ENABLE); //ʹ��TIM2���ж����ж������Ƿ�Ϊ0
			while(Capture_State==1){  //�ȴ����ն˱��0�ſ�ʼ׼����һ�η���
					//USART3_printf("waiting for 0\r\n");
			}
		
			USART3_printf("Capture_State = %d,start timing!\r\n",Capture_State);
      //AD9850_2_Write_Serial(0,200000);  //����200k
			TIM_Cmd(TIM3,ENABLE); //ʹ��TIM3
			t=0;//��װ��ֵ���㣬ָ�����˶��ٸ�����
			
		  //���ղ���
		  while(Capture_State==0){  //�ȴ�����˱�Ϊ1
					//USART3_printf("waiting for 1\r\n");
			}
			//time_us=TIM_GetCounter (TIM3);
			USART3_printf("Capture_State = %d,timing over!\r\n",Capture_State);
			USART3_printf("over\r\n");
			
			//times=t*100+time_us;//times��λΪus
			USART3_printf("time = %f us\r\n",times);
			juli= V *times/10000;//�����juli����λΪum
			USART3_printf("juli = %f m\r\n",juli);
			
			/****************��������ͣ����********************/
			stop_int++;
			if(stop_int==50){
					stop_int=0;
					delay_ms(5000);
				  USART3_printf("-----------already 10 results---------\r\n");
			}
			/****************��������ͣ����*****************/
			
			
			//AD9850_2_Reset_Sreial();
			//delay_ms(1000);//�ȴ����ι�ȥ
	}
	
}	
	



/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/



