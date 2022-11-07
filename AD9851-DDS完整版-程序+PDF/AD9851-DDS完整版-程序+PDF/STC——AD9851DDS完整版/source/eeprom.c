
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "STC12C5A60S2.H"
#include<intrins.h>
#include "eeprom.h"

/*以下是配置STC单片机的寄存器*/

//sfr ISP_DATA  = 0xC2;
//sfr ISP_ADDRH = 0xC3;
//sfr ISP_ADDRL = 0xC4;
//sfr ISP_CMD   = 0xC5;
//sfr ISP_TRIG  = 0xC6;
//sfr ISP_CONTR = 0xC7;

#define uint  unsigned int
#define uchar unsigned char

#define ISP_BYTE_READ    1		//ISP字节读
#define ISP_BYTE_PROGRAM 2		//ISP直接编程
#define ISP_SECTOR_ERASE 3		//ISP扇区擦除

/********************************************************************
* 名称 : Delay()
* 功能 : 延时,延时时间为 10ms * del
* 输入 : del
* 输出 : 无
***********************************************************************/
//void Delay_1ms(uint i)
//{
//	uchar x,j;
//	for(j=0;j<i;j++)
//	for(x=0;x<=148;x++);	
//}

/********************************************************************
* 名称 : ISP_ERASE()
* 功能 : EEPROM的扇区擦除
* 输入 : Address（擦除扇区的地址)
* 输出 : 无
***********************************************************************/
void ISP_ERASE(uint Address)
{
	Address += EEPROM_START_ADDRESS;
	ISP_ADDRL = Address;
	ISP_ADDRH = Address >> 8;
	ISP_CONTR = 0x83;				
	ISP_CMD = ISP_SECTOR_ERASE;	
	ISP_TRIG = 0x5a;	//PDF手册中说先像 ISP_TRIG 中写入 0x5a, 然后写入 0xa5
	ISP_TRIG = 0xa5;
	_nop_();
	_nop_();	
}

/********************************************************************
* 名称 : ISP_READ()
* 功能 : 从EEPROM相应的地址中读出值
* 输入 : Address(想要读取EEPROM的地址）
* 输出 : Read_Data（从EEPROM读出的值）
*********************************************************************/
uint ISP_READ(uint Address)
{
	uchar Read_Data;
	uchar EA_State = EA;
	ISP_ADDRL = Address;
	ISP_ADDRH = Address >> 8;
	EA = 0;	
	ISP_CONTR = 0x83;
	ISP_CMD = ISP_BYTE_READ;
	ISP_TRIG = 0x5a;
	ISP_TRIG = 0xa5;
	_nop_();
	_nop_();
	Read_Data = ISP_DATA;
	EA = EA_State;
	return Read_Data;	
}

/********************************************************************
* 名称 : ISP_PROGRAM()
* 功能 : 实现EEPROM的字节编程
* 输入 : Address为编程的地址，Data_temp为想要编程的值
* 输出 : 无
***********************************************************************/
void ISP_PROGRAM(uint Address, uchar Data_temp)
{
	ISP_DATA = Data_temp;
	ISP_ADDRL = Address;
	ISP_ADDRH = Address >> 8;
	ISP_CONTR = 0x83;
	ISP_CMD = ISP_BYTE_PROGRAM;
	ISP_TRIG = 0x5a;
	ISP_TRIG = 0xa5;
	_nop_();
	_nop_(); 	
}
