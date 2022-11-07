
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _eeprom_h_
#define _eeprom_h_

#include "STC12C5A60S2.H"
#include<intrins.h>
/*以下是配置STC单片机的寄存器*/

//sfr ISP_DATA  = 0xC2;
//sfr ISP_ADDRH = 0xC3;
//sfr ISP_ADDRL = 0xC4;
//sfr ISP_CMD   = 0xC5;
//sfr ISP_TRIG  = 0xC6;
//sfr ISP_CONTR = 0xC7;

#define uint  unsigned int
#define uchar unsigned char

#define EEPROM_START_ADDRESS	0x0000

#define ISP_BYTE_READ    1		//ISP字节读
#define ISP_BYTE_PROGRAM 2		//ISP直接编程
#define ISP_SECTOR_ERASE 3		//ISP扇区擦除

//void Delay_1ms(uint i);
void ISP_ERASE(uint Address);
uint ISP_READ(uint Address);
void ISP_PROGRAM(uint Address, uchar Data_temp);

#endif