
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _FunManage_h_
#define _FunManage_h_

#include "STC12C5A60S2.H"
#include "key.h"
#include "oled.h"
#include "DS1302.H"
#include "DHT11.H"

#define Task_ID u8
#define Max_tasks 3

extern u32 SystemTime;
sbit JDQ = P0^2;
typedef struct{
u8 Screen_permission;
u8 KeyBoard_permission;
u8 KeyValue;
} HardWare;

typedef struct{
Task_ID task_id;
void (*function)(HardWare *har);
}Task_Table;

void Set_Fre(HardWare *har);
void Set_PWM(HardWare *har);
//void SetTime(HardWare *har);
void Sweeper(HardWare *har);
void SetHumidiTime(HardWare *har);
void power_control(void);
u32 Frebuf2intANDdisbuff(u8 Fre_buffer[9], u8 dis[]);
u32 PWMbuf2intANDdisbuff(u8 pwm_buffer[9], u8 dis[]);
void Data_prepare();
extern Task_Table code tasks_table[Max_tasks];

#endif
