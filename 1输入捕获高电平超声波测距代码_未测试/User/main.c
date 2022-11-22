#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "tim.h"
#include "usart.h"
#include "led.h"
#include "AD9850_2.h"  //ʹ�õ��ǰ汾2
#include <math.h>
#include "lm75a.h"

extern u8 TIM2CH1_CAPTURE_STA;  //���벶��״̬
extern u16  TIM2CH1_CAPTURE_VAL; //���벶��ֵ
float Ultra_v;//�����������ٶ�
float Temp;//��ǰ�¶�
float Ultra_s;//��ǰ����ľ���
int OLED_s;//������OLED����ʾ�ľ������ݣ���λΪum
/**
�����б�
1��������ر�DDS��200kHz���������Ҳ�
2��������Ʋ��Σ���ʼ����ʱ���1����������ʱ���0
				�������޸�led.c�����������ѡΪLED1���Ǹ����ţ�PB0����ѡ���������
				
3�����벶��ߵ�ƽʱ�䣺������ʱ��
				�������޸�tim.c
4��I2C���¶ȴ������¶ȣ�Ultra_v = 331.5 + 0.6 * Temp;
5�����㴫�����룺Ultra_s = 0.5 * Ultra_v * Temp;
6��OLED��ʾ�����I2C_SCL = PB6; I2C_SDA = PB7;
**/

int main(){
	
	u32 time=0;//���벶����м����
	u8 buffer[3];//��ȡ���¶�����
	
	//0��������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���� NVIC �жϷ���2(2λ��ռ���ȼ���2λ��Ӧ���ȼ�)
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	I2C_Configuration();//I2C��ʼ��
	OLED0561_Init(); //OLED��ʼ��
	USART3_Init(115200);  //���ڳ�ʼ����PB11��-TXD��PB10--RXD
	Init_AD9850_2();//DDS��ʼ��
	TIM2_Cap_Init(0XFFFF,72-1);  //�� 1Mhz ��Ƶ�ʼ��������벶��
	
	while(1){
		
			while(GPIO_ReadInputDataBit(getPORT,getPin)==1){}//��һ�㱣���������벶�����Żع�0ʱ�����¿�ʼ����
				
			TIM2CH1_CAPTURE_STA=0;  //������һ�β���	
			//1������DDS����
			AD9850_2_Write_Serial(0,200000);//����200k����
			//2��������Ʋ���
			GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //PB0�����źſ��ƣ�����ʱ��1
			
			//3�����벶��ߵ�ƽʱ��
			if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
			{
					time=TIM2CH1_CAPTURE_STA&0X3F;
					time*=65536; //���ʱ���ܺ�
					time+=TIM2CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
					USART3_printf("highValTime = %d us\r\n",time);
					//TIM2CH1_CAPTURE_STA=0;  //������һ�β���
			}
			
			//6��OLED��ʾʱ��
			OLED_DISPLAY_8x16_BUFFER(4,"time ="); //��ʾʱ���ַ�
			OLED_DISPLAY_8x16(2,7*8,time/1000000+0x30);  //��λ
			OLED_DISPLAY_8x16(2,8*8,time/100000+0x30);  
			OLED_DISPLAY_8x16(2,9*8,time/10000+0x30);
			OLED_DISPLAY_8x16(2,10*8,time/1000+0x30);    //msλ
			OLED_DISPLAY_8x16(2,11*8,time/100+0x30);
			OLED_DISPLAY_8x16(2,12*8,time/10+0x30);
			OLED_DISPLAY_8x16(2,13*8,time%10+0x30);      //usλ
			OLED_DISPLAY_8x16(2,15*8,'u');               //��λus
			OLED_DISPLAY_8x16(2,16*8,'s');
			
			//4���¶ȴ���������
			LM75A_GetTemp(buffer); //��ȡLM75A���¶�����
			Temp = buffer[1] + (buffer[2]/10)*0.1 + (buffer[2]%10)*0.01; //����+С�����һλ+С������λ
			if(buffer[0]==1){//Ϊ0ʱ�¶�ֵΪ����Ϊ1ʱ�¶�ֵΪ��
						Temp = -Temp;//��ֵΪ����ǵðѸ��żӽ�Temp��
			}
			Ultra_v = 331.5 + 0.6*Temp; //��λ����ÿ��
			USART3_printf("Ultra_v = %f m/s \r\n",Ultra_v);
			
			OLED_DISPLAY_8x16_BUFFER(4,"   T ="); //��ʾʱ���ַ�
			if(buffer[0]==1)OLED_DISPLAY_8x16(4,7*8,'-');   //�����1��Ϊ1���Ǹ��¶�
			OLED_DISPLAY_8x16(4,8*8,buffer[1]/10+0x30);     //��ʾ�¶�ֵ��������
			OLED_DISPLAY_8x16(4,9*8,buffer[1]%10+0x30);
			OLED_DISPLAY_8x16(4,10*8,'.');                  //��ʾС����
			OLED_DISPLAY_8x16(4,11*8,buffer[2]/10+0x30);    //��ʾС������
			OLED_DISPLAY_8x16(4,12*8,buffer[2]%10+0x30);
			OLED_DISPLAY_8x16(4,13*8,'C');                  //��ʾ��λ
			
			
			//5�����㴫������
			Ultra_s = 0.5 * Ultra_v * (time*0.000001); //���� = �ٶ�(m/s)*ʱ��(us��s)/2
			USART3_printf("Ultra_s = %f m\r\n",Ultra_s);
			USART3_printf("Ultra_s = %f cm\r\n",Ultra_s*100);
			
			//6��OLED��ʾ����
			OLED_s =(Ultra_s*1000000);
			OLED_DISPLAY_8x16_BUFFER(4,"   s ="); //��ʾʱ���ַ�
			OLED_DISPLAY_8x16(2,7*8,OLED_s/1000000+0x30);  //��λ
			OLED_DISPLAY_8x16(2,8*8,OLED_s/100000+0x30);  
			OLED_DISPLAY_8x16(2,9*8,OLED_s/10000+0x30);
			OLED_DISPLAY_8x16(2,10*8,OLED_s/1000+0x30);    //mmλ
			OLED_DISPLAY_8x16(2,11*8,OLED_s/100+0x30);
			OLED_DISPLAY_8x16(2,12*8,OLED_s/10+0x30);
			OLED_DISPLAY_8x16(2,13*8,OLED_s%10+0x30);      //umλ
			OLED_DISPLAY_8x16(2,15*8,'u');               //��λum
			OLED_DISPLAY_8x16(2,16*8,'m');
			
			AD9850_2_Reset_Sreial();//1���رղ���
			GPIO_WriteBit(LEDPORT,LED1,(BitAction)(0)); //2��PB0�����źſ��ƣ��ز�ʱ��0
			delay_ms(1000);//�ڶ��㱣�����ȴ����ι�ȥ
	}
}


































//float juli;//����
////float times;
//extern float time_us;
//int stop_int=0;

//extern vu16 ADC_DMA_IN[2]; //�����ⲿ����
//extern u16 t;  //TIM3�ļ�ʱ����
//extern u8 Capture_State; //��ʼֵΪ1
//extern float times;

//int main (void){//������
//	delay_ms(500); //�ϵ�ʱ�ȴ�������������
//	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
//	I2C_Configuration();//I2C��ʼ��
//	//OLED0561_Init(); //OLED��ʼ��
//	USART3_Init(115200);  //���ڳ�ʼ����PB11��-TXD��PB10--RXD
//	USART3_printf(" hello!!!!!!\r\n");
//	TIM3_Init(99,71);//��ʱ����ʼ����(1/72M*psc)�����һ��arrֵ������(arr+1)�趨ֵʱ�����ж�
//	//(999,71)��������ÿ1ms����һ���ж�
//	USART3_printf(" tim3 init over\r\n");
//	TIM2_Init(99,71);//���벶��ʱ��
//	
//	//Init_AD9850_2();
//	USART3_printf(" tim2 init over\r\n");
//	
//	while(1){
//			int getpin;
//			//��ʼ����Capture_StateΪ1�����ж����������Ϊ0ʱ�ŻῪʼ����
////			TIM_Cmd(TIM2,ENABLE); //ʹ��TIM2���ж����ж������Ƿ�Ϊ0
//			TIM_Cmd(TIM3,ENABLE); //ʹ��TIM3
//			while(Capture_State==1 && GPIO_ReadInputDataBit(getPORT,getPin)==1){  //�ȴ����ն˱��0�ſ�ʼ׼����һ�η���
//					//USART3_printf("waiting for 0\r\n");
//			}
//			getpin = GPIO_ReadInputDataBit(getPORT,getPin);//�鿴��ʱ����Ϊʲô������ѭ��
//			Capture_State=0; //����ѭ����Ψһ������PA0Ϊ0����ʱ��״̬�ֶ���0
//			USART3_printf("getpin = %d��State = %d,start timing!\r\n",getpin,Capture_State);
//      //AD9850_2_Write_Serial(0,200000);  //����200k
//			TIM_Cmd(TIM2,ENABLE); //ʹ��TIM2���ж����ж������Ƿ�Ϊ0
//			t=0;//��װ��ֵ���㣬ָ�����˶��ٸ�����
//			
//		  //���ղ���
//		  while(Capture_State==0 && GPIO_ReadInputDataBit(getPORT,getPin)==0){  //�ȴ�����˱�Ϊ1 //���벶��ʱ�Ὣ״̬��1����ѭ��
//					//USART3_printf("waiting for 1\r\n");
//			}
//			getpin = GPIO_ReadInputDataBit(getPORT,getPin);//�鿴��ʱ����Ϊʲô������ѭ��
//			//time_us=TIM_GetCounter (TIM3);
//			USART3_printf("over\r\n");
//			USART3_printf("getpin = %d,State = %d,timing over!\r\n",getpin,Capture_State);
//			
//			//times=t*100+time_us;//times��λΪus
//			//time_us=TIM_GetCounter (TIM2);
//			//USART3_printf("time_us=%d\r\n",time_us);
//			//times=t*100+time_us;//times��λΪus
//			times=t*100;
//			Capture_State=1;  //Ϊ1��ֹͣ��ʱ
//			USART3_printf("StateNow=%d\r\n",Capture_State);
//			USART3_printf("time = %f us\r\n",times);
//			juli= V *times/10000;//�����juli����λΪum
//			USART3_printf("juli = %f m\r\n",juli);
//			
//			/****************��������ͣ����********************/
//			stop_int++;
//			if(stop_int==10){
//					stop_int=0;
//					delay_ms(5000);
//				  USART3_printf("-----------already 10 results---------\r\n");
//			}
//			/****************��������ͣ����*****************/
//			TIM_Cmd(TIM2,DISABLE); //ʹ��TIM2���ж����ж������Ƿ�Ϊ0
//			TIM_Cmd(TIM3,DISABLE); //ʹ��TIM3
//			
//			//AD9850_2_Reset_Sreial();
//			//delay_ms(1000);//�ȴ����ι�ȥ
//	}
//	
//}	
	



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



