
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "DS1302.H"
#include "stdio.h"
xdata uchar trans_buf;
extern xdata uchar Date_Index;
extern xdata uchar Time_Index;
extern xdata uchar Write_date_buff[15];
//extern xdata uchar Alarm_buff[7];
extern xdata uchar Alarm_arrive;
extern xdata uchar arriveminute;
extern xdata uchar Alarm_state;
extern xdata Time_info Read_Time;
extern xdata Time_info Write_Time;

xdata uint hour,minute,second;
xdata uchar old_second=250;

void delay(unsigned long int i)
{
	while(i--);
}
//void System_Init()
//{	
//	Init_DS1302();
//}
void time_write2DS1302(Time_info Time)
{
	DS1302Write(0x8e,0x00);//写保护关
	trans_buf = ((Time.year%100/10)<<4)+((Time.year%10)&0X0F);            //写入年 
	DS1302Write(year_w, trans_buf);	
	trans_buf = (((Time.month%100/10)<<4)&0x10) + ((Time.month%10)&0X0F);	 //写入月
	DS1302Write(month_w, trans_buf);	  
	trans_buf = (((Time.day%100/10)<<4)&0x30) + ((Time.day%10)&0X0F);        //写入日
	DS1302Write(day_w, trans_buf);		 								
	trans_buf = (((Time.hour%100/10)<<4)&0x30)+((Time.hour%10)&0X0F);		 //写入时
	DS1302Write(hour_w, trans_buf);
	trans_buf = (((Time.minute%100/10)<<4)&0x70) + ((Time.minute%10)&0X0F);	 //写入分
	DS1302Write(minute_w, trans_buf);
	trans_buf = (((Time.second%100/10)<<4)&0x70) + ((Time.second%10)&0X0F);	 //写入秒
	DS1302Write(second_w, (trans_buf&0X7F));
	trans_buf = Time.week%7;                                       	 //写入星期
	DS1302Write(week_w, trans_buf);
	DS1302Write(0x8e,0x80); //开写保护
}
void writeDate2DS1302(Time_info Time)
{
	DS1302Write(0x8e,0x00);//写保护关
	trans_buf = ((Time.year%100/10)<<4)+((Time.year%10)&0X0F);            //写入年 
	DS1302Write(year_w, trans_buf);	
	trans_buf = (((Time.month%100/10)<<4)&0x10) + ((Time.month%10)&0X0F);	 //写入月
	DS1302Write(month_w, trans_buf);	  
	trans_buf = (((Time.day%100/10)<<4)&0x30) + ((Time.day%10)&0X0F);        //写入日
	DS1302Write(day_w, trans_buf);
   	Time.week = (int)(Time.year/100)/4-(int)2*(Time.year/100)+(int)(Time.year%100)+(int)(Time.year%100)/4+(int)26*(Time.month+1)/10+(int)Time.day-1;
	Time.week %= 7;
	if(Time.week == 0) Time.week = 7;
	trans_buf = Time.week;                                       	 //写入星期
	DS1302Write(week_w, trans_buf);		 								
	DS1302Write(0x8e,0x80); //开写保护
}
void time_HMS2DS1302(Time_info Time)
{
	DS1302Write(0x8e,0x00);//写保护关	 								
	trans_buf = (((Time.hour%100/10)<<4)&0x30)+((Time.hour%10)&0X0F);		 //写入时
	DS1302Write(hour_w, trans_buf);
	trans_buf = (((Time.minute%100/10)<<4)&0x70) + ((Time.minute%10)&0X0F);	 //写入分
	DS1302Write(minute_w, trans_buf);
	trans_buf = (((Time.second%100/10)<<4)&0x70) + ((Time.second%10)&0X0F);	 //写入秒
	DS1302Write(second_w, (trans_buf&0X7F));
	DS1302Write(0x8e,0x80); //开写保护
}
void time_read_from_DS1302(Time_info *Time)
{
	Time->year   = DS1302Read(year_r);
	Time->month  = DS1302Read(month_r);
	Time->day    = DS1302Read(day_r);
	Time->hour   = DS1302Read(hour_r);
	Time->minute = DS1302Read(minute_r);
	Time->second = DS1302Read(second_r);
	Time->week   = DS1302Read(week_r);
	
	Time->year   = 2000+((Time->year>>4)*10)+(Time->year&0x0f);
	Time->month  = ((Time->month>>4)*10) + (Time->month&0x0f);
	Time->day    = (Time->day>>4)*10 + (Time->day&0x0f);
	Time->hour   = (Time->hour>>4)*10 + (Time->hour&0x0f);
	Time->minute = ((Time->minute>>4)*10) + (Time->minute&0x0f);
	Time->second = (Time->second>>4)*10 + (Time->second&0x0f);
}
void DS1302_Writebyte(uchar dat)
{
	uchar i;
	delay(30);
	for(i = 0; i < 8; i++)
	{
		sclkk = 0;
		sdinn = dat & 0x01;
		delay(9);
		sclkk = 1;
		delay(9);
		dat >>= 1;
	}
}
uchar DS1302_Readbyte()
{
	uchar i, dat = 0;
	delay(9);
	for(i = 0; i < 8; i++)
	{
		dat >>=1;
		sclkk = 1;
		delay(10);
		sclkk = 0;
		delay(6);
		if(sdinn)
			dat  |= 0x80;
	}
	sdinn = 0;
	return dat;
}
void DS1302Write(uchar cmd, uchar dat)
{
	cee = 0;
	sclkk = 0;
	cee = 1;
	DS1302_Writebyte(cmd);
	DS1302_Writebyte(dat);
	sclkk = 1;
	delay(6);
	cee = 0;
}
uchar DS1302Read(uchar cmd)
{
	uchar dat = 0;
	cee = 0;
	sclkk = 0;
	cee = 1;
	DS1302_Writebyte(cmd);
	dat = DS1302_Readbyte();
	sclkk = 1;
	cee = 0;
	return dat;
}
void Init_DS1302(void)
{
	xdata char temp;
	Time_info time;
	uchar init_state;
	DS1302Write(0x8e,0x00);//写保护关
	temp = DS1302Read(second_r);
	DS1302Write(second_w, temp&0X7F);
	DS1302Write(0x90,0XA7); //充电
	init_state = DS1302Read(protect_r);
	DS1302Write(0x8e,0x80); //开写保护
	if(init_state != D_protect)
	{	
		time.year = 2014;
		time.month = 12;
		time.day = 5;
		time.hour = 18;
		time.minute = 57;
		time.second = 50;	 //日期换算为星期	[c/4]-2c+y+[y/4]+[26(m+1)/10]+d-1
		time.week = (time.year/100)/4-2*(time.year/100)+(time.year%100)+(time.year%100)/4+26*(time.month+1)/10+time.day-1;
		time.week %= 7;
		if(time.week == 0) time.week = 7;
		time_write2DS1302(time);
		DS1302Write(0x8e,0x00);//写保护关
		temp = DS1302Read(second_r);
		DS1302Write(second_w, temp&0X7F);
		DS1302Write(protect_w,D_protect); //初始化一次标示
		DS1302Write(0x8e,0x80); //开写保护
	}
//	Alarm_buff[0] = DS1302Read(clock_base_r+0);
//	Alarm_buff[1] = DS1302Read(clock_base_r+2);
//	Alarm_buff[2] = DS1302Read(clock_base_r+4);
//	Alarm_buff[3] = DS1302Read(clock_base_r+6);
//	Alarm_buff[4] = DS1302Read(clock_base_r+8);
//	Alarm_buff[5] = DS1302Read(clock_base_r+10);
//	Alarm_state   = DS1302Read(clock_state_r);
}
//void Alarm_respond()
//{
//	hour = (uint)Alarm_buff[0]*10+Alarm_buff[1];
//	minute = (uint)Alarm_buff[2]*10+Alarm_buff[3];
//    second = (uint)Alarm_buff[4]*10+Alarm_buff[5];
//		
//	if(Alarm_state==1)
//	{
//		time_read_from_DS1302(&Read_Time);
//		if((old_second < Read_Time.second)||(Read_Time.second==0))
//		{
//			if(arriveminute!=Read_Time.minute) Alarm_arrive = 0;
//			if((Read_Time.hour == hour)&&(Read_Time.minute == minute)&&(Read_Time.second == second))
//			{
//				Alarm_arrive = 1;
//				arriveminute = Read_Time.minute;
//			}
//		}
//		old_second = Read_Time.second;
//	}	
//}
//void sendtime2computer()
//{
//	time_read_from_DS1302(&Read_Time);
//}
void datecopy(Time_info *Write_Time, Time_info *Read_Time,uchar Write_date[])
{
	Write_Time->year = Read_Time->year;
	Write_Time->month = Read_Time->month;
	Write_Time->day = Read_Time->day;
	Write_Time->hour = Read_Time->hour;
	Write_Time->minute = Read_Time->minute;
	Write_Time->second = Read_Time->second;
	Write_Time->week = Read_Time->week;
	
	Write_date[0] = Write_Time->year%100/10;
	Write_date[1] = Write_Time->year%10;
	Write_date[2] = Write_Time->month/10;
	Write_date[3] = Write_Time->month%10;
	Write_date[4] = Write_Time->day/10;
	Write_date[5] = Write_Time->day%10;

	Write_date[6] = Write_Time->hour/10;
	Write_date[7] = Write_Time->hour%10;
	Write_date[8] = Write_Time->minute/10;
	Write_date[9] = Write_Time->minute%10;
	Write_date[10] = Write_Time->second/10;
	Write_date[11] = Write_Time->second%10;

	Write_date[12] = Write_Time->week;
	Write_date[13] = 0;
}
char* printf_YMD2String(char *str, int year, int month, int day)
{
	*(str+0)  = year%10000/1000+'0';
	*(str+1)  = year%1000/100+'0';
	*(str+2)  = year%100/10+'0';
	*(str+3)  = year%10+'0';
	*(str+4)  = '/';
	*(str+5)  = month%100/10 + '0';
	*(str+6)  = month%10 + '0';
	*(str+7)  = '/';
	*(str+8)  = day%100/10 + '0';
	*(str+9)  = day%10 + '0';
	*(str+10) = 0;	
	return str;
}
char* printf_HMS2String(char *str, int hour, int minute, int second)
{
	*(str+0)  = hour%100/10+'0';
	*(str+1)  = hour%10+'0';
	*(str+2)  = ':';
	*(str+3)  = minute%100/10 + '0';
	*(str+4)  = minute%10 + '0';
	*(str+5)  = ':';
	*(str+6)  = second%100/10 + '0';
	*(str+7)  = second%10 + '0';
	*(str+8)  = 0;	
	return str;
}
char* printf_alarm2String(char *str, int hour, int minute, u8 dam, u8 state)
{
	*(str+0)  = hour%100/10+'0';
	*(str+1)  = hour%10+'0';
	*(str+2)  = ':';
	*(str+3)  = minute%100/10 + '0';
	*(str+4)  = minute%10 + '0';
	*(str+5)  = ' ';
	*(str+6)  = dam%100/10 + '0';
	*(str+7)  = dam%10 + '0';
	*(str+8)  = ' ';
	*(str+9)  = 'S';
	*(str+10)  = ':';
	if(state == 1)
	{
		*(str+11)  = 'O';
		*(str+12)  = 'N';
		*(str+13)  = ' ';
	}
	else
	{
		*(str+11)  = 'O';
		*(str+12)  = 'F';
		*(str+13)  = 'F';	
	}
	*(str+14)  = 0;	
	return str;
}


