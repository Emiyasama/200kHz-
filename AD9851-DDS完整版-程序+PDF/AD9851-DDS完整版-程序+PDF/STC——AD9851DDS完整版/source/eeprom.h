
/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef _eeprom_h_
#define _eeprom_h_

#include "STC12C5A60S2.H"
#include<intrins.h>
/*����������STC��Ƭ���ļĴ���*/

//sfr ISP_DATA  = 0xC2;
//sfr ISP_ADDRH = 0xC3;
//sfr ISP_ADDRL = 0xC4;
//sfr ISP_CMD   = 0xC5;
//sfr ISP_TRIG  = 0xC6;
//sfr ISP_CONTR = 0xC7;

#define uint  unsigned int
#define uchar unsigned char

#define EEPROM_START_ADDRESS	0x0000

#define ISP_BYTE_READ    1		//ISP�ֽڶ�
#define ISP_BYTE_PROGRAM 2		//ISPֱ�ӱ��
#define ISP_SECTOR_ERASE 3		//ISP��������

//void Delay_1ms(uint i);
void ISP_ERASE(uint Address);
uint ISP_READ(uint Address);
void ISP_PROGRAM(uint Address, uchar Data_temp);

#endif