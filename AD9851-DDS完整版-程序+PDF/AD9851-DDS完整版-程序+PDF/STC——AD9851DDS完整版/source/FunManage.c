
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "FunManage.h"
#include "stdio.h"
#include "eeprom.h"
#include "string.h"
#include "MCP41xx.h"
#include "DDS9851.H"

Task_Table code tasks_table[Max_tasks] = {
{0, (*Set_Fre)},
{1, (*Set_PWM)},
{2, (*Sweeper)},
};

xdata Time_info Read_Time, Write_Time;
xdata uchar Write_date_buff[15];
xdata u8 fresh = 1;
xdata u32 fresh_time;
xdata u8 power_fresh = 1;
xdata u32 power_fresh_time;
xdata char dis_str[30];
u8 fresh_screen = 1;

extern u8 ADC_Value;
u32 Frequency = 1000;
u8 Fre_buffer[9];
u8 Fre_index = 0;
u32 PWM = 30;
u8 PWM_buffer[9] = {0,0,0,0,0,0,0,0,0};
u8 PWM_index = 0;
u8 disbuf[20];
#define Saveflag 0X55
void Set_Fre(HardWare *har)
{
	u8 i;
	if(fresh_screen == 1)
	{
		fresh_screen = 0;
		OLED_Clear();
		LCD_Show_CEStr(2,2,"频率:");
		if(Fre_index < 3)
			LCD_Show_CEStr(42,2,"(M Hz )");
		else if(Fre_index < 6)
			LCD_Show_CEStr(42,2,"(k Hz )");
		else
			LCD_Show_CEStr(42,2,"( Hz ) ");
		Fre_buffer[0] = Frequency/100000000;
		Fre_buffer[1] = Frequency%100000000/10000000;
		Fre_buffer[2] = Frequency%10000000/1000000;
		Fre_buffer[3] = Frequency%1000000/100000;
		Fre_buffer[4] = Frequency%100000/10000;
		Fre_buffer[5] = Frequency%10000/1000;
		Fre_buffer[6] = Frequency%1000/100;
		Fre_buffer[7] = Frequency%100/10;
		Fre_buffer[8] = Frequency%10;
	}
	switch(har->KeyValue)
	{
		case K_U_S: Fre_buffer[Fre_index]++;break;
		case K_D_S: Fre_buffer[Fre_index]--;break;
		case K_U_L: Fre_buffer[Fre_index]++;break;
		case K_D_L: Fre_buffer[Fre_index]--;break;
		case K_L_S: Fre_index++;break;
		case K_L_L: Fre_index++;break;
		default: break;
	}
	Fre_index %= 9;
	if(Fre_buffer[Fre_index] == 255)
		Fre_buffer[Fre_index] = 9;
	Fre_buffer[Fre_index] %= 10;
	Frequency = Frebuf2intANDdisbuff(Fre_buffer, disbuf);	
	if(Fre_index<3)			disbuf[Fre_index]   += 128;
	else if(Fre_index<6)	disbuf[Fre_index+1] += 128;
	else					disbuf[Fre_index+2] += 128;
	OLED_ShowString(18,4,disbuf);
	if(har->KeyValue != K_NO)
	{
		if(Fre_index < 3)
			LCD_Show_CEStr(42,2,"(M Hz )");
		else if(Fre_index < 6)
			LCD_Show_CEStr(42,2,"(k Hz )");
		else
			LCD_Show_CEStr(42,2,"( Hz ) ");
		ad9851_wr_parrel(0x01,Frequency);
		ISP_ERASE(0);
		for(i = 0; i < 9; i++)
			ISP_PROGRAM(i,Fre_buffer[i]);
		for(i = 0; i < 3; i++)
			ISP_PROGRAM(i+9,PWM_buffer[i]);	
		ISP_PROGRAM(12,Saveflag);
	}	
}
u8 num = 0;
void Set_PWM(HardWare *har)
{
	u8 i;
	if(fresh_screen == 1)
	{
		fresh_screen = 0;
		OLED_Clear();
		LCD_Show_CEStr(2,2,"占空比:");
		PWM_buffer[0] = PWM/100;
		PWM_buffer[1] = PWM%100/10;
		PWM_buffer[2] = PWM%10;
	}
	switch(har->KeyValue)
	{
		case K_U_S: PWM_buffer[PWM_index]++;break;
		case K_D_S: PWM_buffer[PWM_index]--;break;
		case K_U_L: PWM_buffer[PWM_index]++;break;
		case K_D_L: PWM_buffer[PWM_index]--;break;
		case K_L_S: PWM_index++;break;
		case K_L_L: PWM_index++;break;
		default: break;
	}
	if(PWM_index == 3)
		PWM_index = 1;
	if(PWM_index == 0)
		PWM_index = 2;
	if(PWM_buffer[1] > 5) PWM_buffer[1] = 0;
	if(PWM_buffer[1] == 255) PWM_buffer[1] = 5;
	if(PWM_buffer[1] == 5)
	{
		if(PWM_buffer[2] > 5) PWM_buffer[2] = 0;
	}
	if(PWM_buffer[2] > 9) PWM_buffer[2] = 0;
	if(PWM_buffer[2] == 255) PWM_buffer[2] = 9;

//	PWM_index %= 3;
//	if(PWM_buffer[PWM_index] == 255)
//		PWM_buffer[PWM_index] = 9;
//	PWM_buffer[PWM_index] %= 10;
//	PWM_buffer[0] %= 2;
//	if(PWM_buffer[0] == 1)
//	{
//		PWM_buffer[1] = 0;
//		PWM_buffer[2] = 0;
//	}
	PWM = PWMbuf2intANDdisbuff(PWM_buffer, disbuf);	
	disbuf[PWM_index] += 128;
	OLED_ShowString(18,4,disbuf);
	if(har->KeyValue != K_NO)
	{
		MCP41xx_writedata((u8)((float)PWM/100*255));
		MCP41xx_writedata(PWM+200);
		ISP_ERASE(0);
		for(i = 0; i < 9; i++)
			ISP_PROGRAM(i,Fre_buffer[i]);
		for(i = 0; i < 3; i++)
			ISP_PROGRAM(i+9,PWM_buffer[i]);
		ISP_PROGRAM(12,Saveflag);
			
	}	
}
u8 Sweeper_state = 0;
void Sweeper(HardWare *har)
{
	HardWare *HARD = har;		//防编译出错，无实际意义
	if(fresh_screen == 1)
	{
		fresh_screen = 0;
		OLED_Clear();
		LCD_Show_CEStr(2,2,"开始扫频");
		Sweeper_state = 1;
	}	
}
u16 ADC_Real;
void power_control(void)
{
	if(power_fresh == 1)
	{
		power_fresh_time = SystemTime + 100;
		power_fresh = 0;
	}
	if((SystemTime >= power_fresh_time)&&(SystemTime<(((u32)0-100))))
	{
		power_fresh = 1;
//		time_read_from_DS1302(&Read_Time);			
//		sprintf(dis_str,"%02d:%02d:%02d", Read_Time.hour, Read_Time.minute, Read_Time.second);
//		OLED_ShowString(0,0, dis_str);
//		draw_cell(92,0,(u8)((double)ADC_Value/255*25), 25);
		OLED_ShowNum(0,6,2,1,16);	
		OLED_ShowString(0,6, "    ");
	}
}
u32 Frebuf2intANDdisbuff(u8 Fre_buffer[9], u8 dis[])
{
	u32 Fre = 0;
	Fre += Fre_buffer[0]*100000000;
	Fre += Fre_buffer[1]*10000000;
	Fre += Fre_buffer[2]*1000000;
	Fre += Fre_buffer[3]*100000;
	Fre += Fre_buffer[4]*10000;
	Fre += Fre_buffer[5]*1000;
	Fre += Fre_buffer[6]*100;
	Fre += Fre_buffer[7]*10;
	Fre += Fre_buffer[8];
	dis[0]=Fre_buffer[0]+'0';
	dis[1]=Fre_buffer[1]+'0';
	dis[2]=Fre_buffer[2]+'0';
	dis[3]=',';
	dis[4]=Fre_buffer[3]+'0';
	dis[5]=Fre_buffer[4]+'0';
	dis[6]=Fre_buffer[5]+'0';
	dis[7]=',';
	dis[8]=Fre_buffer[6]+'0';
	dis[9]=Fre_buffer[7]+'0';
	dis[10]=Fre_buffer[8]+'0';
	dis[11]=0;
	return Fre;
}
u32 PWMbuf2intANDdisbuff(u8 pwm_buffer[3], u8 dis[])
{
	u32 pwm = 0;

	pwm += pwm_buffer[0]*100;
	pwm += pwm_buffer[1]*10;
	pwm += pwm_buffer[2];
	dis[0]=pwm_buffer[0]+'0';
	dis[1]=pwm_buffer[1]+'0';
	dis[2]=pwm_buffer[2]+'0';
	dis[3]=0;
	return pwm;
}
void Data_prepare()
{
	u8 i;
	if(ISP_READ(12) != Saveflag)
	{
		for(i = 0; i < 9; i++)
			Fre_buffer[i] = 0;
		for(i = 0; i < 3; i++)
			PWM_buffer[i] = 0;
		Frequency = 0;
		PWM = 0;	
	}
	else
	{
		for(i = 0; i < 9; i++)
			Fre_buffer[i] = ISP_READ(i);
		for(i = 0; i < 3; i++)
			PWM_buffer[i] = ISP_READ(i+9);
		Frequency = Frebuf2intANDdisbuff(Fre_buffer, disbuf);
		PWM = PWMbuf2intANDdisbuff(PWM_buffer, disbuf);
	}	
}