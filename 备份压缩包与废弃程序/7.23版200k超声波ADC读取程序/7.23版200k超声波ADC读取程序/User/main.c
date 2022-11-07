/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	�����͵�λ��ADC��ȡ����
��д�ˣ�	����	
��дʱ�䣺	2018��4��6��
Ӳ��֧�֣�	STM32F103C8   �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
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
u8 fashe[256];//����
u8 jieshou[256];//����
u8 juli;//����
u8 D[256];
u8 C[32];
u8 S[32];
u8 K1[8];
u8 K2[8];
u8 v[8];
u8 h[8];
u8 maxn;
u8 i;
u16 n; //���������������޸ģ���Ϊu8�ܴ洢�����ֵ����256������u8�Ĳɼ���n��ԶС��256
u16 adcx;
u8 second1;//������
u8 minutes1;//�����
u8 second2;//������
u8 minutes2;//���շ�
extern vu16 ADC_DMA_IN[2]; //�����ⲿ����
extern u16 t;  //TIM3�ļ�ʱ����
extern u16 m;  //TIM3�ļ�ʱ����

int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	LED_Init();
	RELAY_Init();//�̵�����ʼ��
	KEY_Init();//KEY
	ADC_Configuration(); //ADC��ʼ������
	I2C_Configuration();//I2C��ʼ��
	TIM3_Init(9999,7199);//��ʱ����ʼ������ʱ1��
	TIM2_Init(9999,7199);//��ʱ���ܣ�TIM2_INit(a,b),ʱ��=��a+1��*��b+1��/72΢�루ms��
	
	//�ϵ�������䲨��
	RELAY_1(1);      //��Դ�ͷ����·��Ŀ��رպ�
	delay_ms(2000); //�ȴ������������
	
	while(1){
		
		//����3�����ڵĲ�
		RELAY_2(1);   //�����·��̽ͷ��Ŀ��رպ�
		TIM_Cmd(TIM3,DISABLE); //ʹ��TIM3����ʼ��ʱ
		for(n=0;n<45;n++){
				fashe[n]=ADC_DMA_IN[1];        //���䲨�ɼ����ݲ�֪��������ʱ�����ã���
		}
		delay_us(25);//��Ϊ f = 200kHz��5������ = 25us
		RELAY_2(0);
		
		
		
		//�ɼ����ղ�
		delay_us(440);//�����ನӰ��
		while(ADC_DMA_IN[1]>1){ //�����ղ��ɼ�ֵ
				second2 = t;
			  minutes2 = m;
			  for(n=0;n<45;n++){
						jieshou[n]=ADC_DMA_IN[1];
				}
		}
		
		//ADC������ʾ�ڴ�������
        //��ʱ��TIM3��¼�˴���ʱ�䣬Ϊminutes2����second2��
		
		
		//������λ�����䲨and���ղ���

		for(i=0;i<9;i++){
		  for(n=0;n<5;n++){
				D[n]=jieshou[9*n+i];
				//TODO:�����������ֵ
				//if(V[i]>vmax){vmax=v[i];h=i;}
			}
		}
		for(n=0;n <32;){
			S[n]=sin(2*PI/CAIYANG*n*2*PI*200);//200Khz
			C[n]=cos(2*PI/CAIYANG*n*2*PI*200);
			
		  for(i=0;i<8;i++){
				D[n]=jieshou[8*n+i];//���߼����⡾����������
				//TODO:�����������ֵ
				
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
		//��֪h[i]
		//TODO:������λ
		//�������
		
			
		//�������

		
	}
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
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



