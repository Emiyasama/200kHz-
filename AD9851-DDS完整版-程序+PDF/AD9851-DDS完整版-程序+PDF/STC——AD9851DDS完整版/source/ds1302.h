
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _ds1302_h_
#define _ds1302_h_
#include "STC12C5A60S2.H"
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define u32 unsigned long int

sbit sdinn = P0^4;
sbit sclkk = P0^3;
sbit  cee     = P0^5;

#define time_24mode	0x00
#define time_12mode	0x01
#define AM			0X00
#define PM			0X01

#define second_w	0x80
#define second_r	0x81
#define minute_w	0x82
#define minute_r	0x83
#define hour_w		0x84
#define hour_r		0x85
#define day_w		0x86
#define day_r		0x87
#define month_w		0x88
#define month_r		0x89
#define year_w		0x8c
#define year_r		0x8d
#define week_w		0x8a
#define week_r		0x8b
#define protect_w	0xc0
#define protect_r	0xc1
#define clock_base_w 0xc2
#define clock_base_r 0xc3
#define clock_state_w 0xcc
#define clock_state_r 0xcd
#define test_w 0xce
#define test_r 0xcf	
#define D_protect	0x69
typedef struct{
	uint year;
	uint month;
	uint day;
	uint hour;
	uint minute;
	uint second;
	uint week;
} Time_info;

//void System_Init();
void delay(unsigned long int i);
void DS1302_Writebyte(uchar dat);
void time_write2DS1302(Time_info Time);
void time_read_from_DS1302(Time_info *Time);
uchar DS1302_Readbyte();
void DS1302Write(uchar cmd, uchar dat);
uchar DS1302Read(uchar cmd);
void Init_DS1302(void);
void display_date(unsigned char X,unsigned char Y,Time_info Read_T);
void writeDate2DS1302(Time_info Time);
void time_HMS2DS1302(Time_info Time);
//void Alarm_respond();
//void sendtime2computer();
void datecopy(Time_info *Write_Time, Time_info *Read_Time,uchar Write_date[]);
char* printf_YMD2String(char *str, int year, int month, int day);
char* printf_HMS2String(char *str, int hour, int minute, int second);
char* printf_alarm2String(char *str, int hour, int minute, u8 dam, u8 state);
#endif

