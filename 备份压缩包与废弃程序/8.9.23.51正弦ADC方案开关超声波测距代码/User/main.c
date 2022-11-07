/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	�����͵�λ��ADC��ȡ����
��д�ˣ�	������	
��дʱ�䣺	2022��8��5��
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
#include "bsp_adc.h"
#include "key.h"
#include "usart.h"
#include "led.h"
#include "AD9850.h"
#include "AD9850_2.h"
#include <math.h>
#include "table_fft.h"
#include "stm32_dsp.h"
//#include "bsp_tpad.h"
#define PI 3.14159265
#define V 344
#define NPT 90 //�ɵ����=90

float fashe[90];//����
float jieshou[90];//����
int juli;//���롾mm��
int i;

u8 second1;//������
u8 minutes1;//�����
u8 second2;//������
u8 minutes2;//���շ�
u16 times;
extern vu16 ADC_DMA_IN[2]; //�����ⲿ����
extern u16 t;  //TIM3�ļ�ʱ����
extern u8 TIM3_flag;//TIM3�Ŀ�����־λ  //TIM3����������֤���Ҳ�����ʱ��
extern u8 Capture_State;
extern u8 flag;
char p[10];
extern uint16_t TIM5_GetTime(void);
int32_t input[90];
int32_t output[90];
int32_t OUTPUT_THETA[90];

// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[NOFCHANEL];

// �ֲ����������ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[NOFCHANEL];


int main (void){//������
	//int n=0;//����forѭ���洢ADC��ֵ
	int i=0;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_8x16_BUFFER(0,"      TEST    "); //��ʾ�ַ���
	TIM3_Init(39,17);//��ʱ����ʼ������ʱ10us
	OLED_DISPLAY_8x16_BUFFER(2,"juli/cmm time/us"); //��ʾ�ַ���
	USART3_Init(115200);  //���ڳ�ʼ��
	Init_AD9850();
	Init_AD9850_2();
  ADCx_Init();
  
	while(1){
      
			AD9850_Write_Serial(0,20000);  //20k
	    AD9850_2_Write_Serial(0,200000);  //200k
		  TIM3_flag=1;//������ʱ
		  
		  //100us����20��200k��2��20k
		  delay_us(100);   //5cm�ľ�����ҪԼ200us�Żᴫ����������
		
			//�رշ���
		  AD9850_Reset_Sreial();
		  AD9850_2_Reset_Sreial();
		

		  //ADC���Բ���
		  //ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3-1;
		 
		  //USART3_printf("ADC = %f\r\n",ADC_ConvertedValueLocal[1]);
			
		  OLED_DISPLAY_8x16(0,15*8,i+0x30);
			
		  while((float)ADC_ConvertedValue[1]/4096*3.3<=0.05){
					
			}
			OLED_DISPLAY_8x16_BUFFER(4," time = "); //��ʾ�ַ���
		  TIM3_flag=0;//ֹͣ��ʱ
		  //times=t*10;   //tָ�м���10us��time��λΪus
			times=t*10;   //��t=200us//����ע���Ҷ�ʱ���Լ��ĳ���10usһ���жϣ�
			juli= V *times;//�����juli
      
		  
			OLED_DISPLAY_8x16(4,9*8,times/1000+0x30);
		  OLED_DISPLAY_8x16(4,10*8,times%1000/100+0x30);
			OLED_DISPLAY_8x16(4,11*8,times%100/10+0x30);
			OLED_DISPLAY_8x16(4,12*8,times%10+0x30);

			
			//����Ϊint
			OLED_DISPLAY_8x16(6,6*8,juli/1000000+0x30);
			OLED_DISPLAY_8x16(6,7*8,juli%1000000/100000+0x30);
			OLED_DISPLAY_8x16(6,8*8,juli%100000/10000+0x30);
			OLED_DISPLAY_8x16(6,9*8,juli%10000/1000+0x30);
			OLED_DISPLAY_8x16(6,10*8,juli%1000/100+0x30);
			OLED_DISPLAY_8x16(6,11*8,juli%100/10+0x30);
			OLED_DISPLAY_8x16(6,12*8,juli%10+0x30);
      OLED_DISPLAY_8x16_BUFFER(6," juli=");

			i++;
			if(i==10){
					i=0;
			}
			delay_us(100);//�ȴ�����ĺ󼸸���·�����ն�
			//OLED_DISPLAY_8x16_BUFFER(0,"H"); //��ʾ�ַ���
			//OLED_DISPLAY_8x16(6,15*8,8+0x30);
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



