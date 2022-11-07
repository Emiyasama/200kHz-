
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "STC12C5A60S2.H"
#include "oled.h"
#include "key.h"
#include "DHT11.H"
#include "USART.H"
#include "stdio.h"
#include "DS1302.H"
#include "FunManage.h"
#include "eeprom.h"
#include "DDS9851.H"
#include "MCP41xx.h"
#include "ADC.H"

extern Task_Table code tasks_table[Max_tasks];
u8 task_id = 0;
void (*Active_fun)(HardWare *har);
HardWare hard;
u8 old_id = 0, new_id = 0;

extern uint AD_Point;
extern u8 ADC_Value;
extern u8 fresh_screen;
extern u32 Frequency;
extern u32 PWM;
extern u8 Fre_buffer[9];
extern u8 PWM_buffer[9];
u8 i;
void main()
{
	OLED_Init();
	Key_Init();
	UsartInit();
	Init_DS1302();
	AD_Init();
	ad9851_reset();
	ad9851_wr_parrel(0x01,Frequency);	//0X00为不开六备频 0X01为开六备频
	MCP41xx_writedata(0);
	Data_prepare();
	LCD_Show_CEStr(30,0,"康威电子");
	LCD_Show_CEStr(36,2,"DDS9851");
	LCD_Show_CEStr(16,4,"便携式频率源");
	LCD_Show_CEStr(52,6,"初始化.");
	delay_ms(300);
	LCD_Show_CEStr(52,6,"初始化..");
	LCD_Show_modeCEStr(52,6,"初始化..", InNomal);
	while(1)
	{
		old_id = new_id;
		hard.KeyValue = KeyRead();
		if(hard.KeyValue == K_R_S)
		{
			task_id++;
			task_id %= 2;
		}
		if(hard.KeyValue == K_R_L)
			task_id = 2;
		Active_fun = tasks_table[task_id].function;
		new_id = task_id;
		if(old_id != new_id) fresh_screen = 1;
		(*Active_fun)(&hard);		
		KEY_EXIT();
		power_control();
	}
}
