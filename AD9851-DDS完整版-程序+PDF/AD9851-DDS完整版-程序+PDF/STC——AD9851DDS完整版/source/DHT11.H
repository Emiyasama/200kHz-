#ifndef _DHT11_H_
#define _DHT11_H_
#include "STC12C5A60S2.H"
#include "oled.h"
extern u32 SystemTime;

sbit DATA_DHT = P3^6;

u16 Get_Tem_Dam(u8 *dam_integer, u8 *tem_integer);

#endif