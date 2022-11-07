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

int juli;//����

u16 second1;//������
extern u16 second2;//������
u16 times;

extern vu16 ADC_DMA_IN[2]; //�����ⲿ����
extern u16 t;  //TIM3�ļ�ʱ����
extern u8 Capture_State;

int main (void){//������
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	I2C_Configuration();//I2C��ʼ��
	//OLED0561_Init(); //OLED��ʼ��
	//OLED_DISPLAY_8x16_BUFFER(0,"   Init Over    "); //��ʾ�ַ���
	USART3_Init(115200);  //���ڳ�ʼ��
	TIM3_Init(9999,719);//��ʱ����ʼ������ʱ10u��
	USART3_printf("init down\r\n");
	//Init_AD9850_2();
	//OLED_DISPLAY_8x16_BUFFER(2,"  AD9850 Over   "); //��ʾ�ַ���
  
	while(1){
			
	    //AD9850_2_Write_Serial(0,200000);  //200k
			TIM_Cmd(TIM3,ENABLE); //ʹ��TIM3
      t=0;//��װ��ֵ���㣬ָ�����˶��ٸ�����
		  Capture_State=0;//״̬���㣬�������ǵ�0�������أ��ȴ���һ��������ʱ������жϼ�ʱֹͣ
		  USART3_printf("start\r\n");
		  //���ղ���
		  while(Capture_State==0){  //ԭADC�ӿ�������PA6��Ϊ��ʱ�������ش�����
					//������ѭ���ڵȴ����ȴ���һ�������ش���
					USART3_printf("running\r\n");
			}
			USART3_printf("over\r\n");
			TIM_Cmd(TIM3,DISABLE); //�ر�TIM3
			times=t*10+second2;//times��λΪus
			juli= V *times;//�����juli����λΪum
      USART3_printf("time = %d us\r\n",times);
			
//		  OLED_DISPLAY_8x16_BUFFER(4," times = "); //��ʾ�ַ���
//		  OLED_DISPLAY_8x16(4,10*8,times/100+0x30);
//			OLED_DISPLAY_8x16(4,11*8,times%100/10+0x30);
//			OLED_DISPLAY_8x16(4,10*8,times%10+0x30);
//			
//			OLED_DISPLAY_8x16_BUFFER(4,"s(m)="); //��ʾ�ַ���
//		  OLED_DISPLAY_8x16(4,10*8,juli/100+0x30);
//			OLED_DISPLAY_8x16(4,11*8,juli%100/10+0x30);
//			OLED_DISPLAY_8x16(4,10*8,juli%10+0x30);			
			
			
			//AD9850_2_Reset_Sreial();
			delay_ms(1000);//�ȴ�����ĺ󼸸���·�����ն�
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



