
/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "STC12C5A60S2.H"
#include<intrins.h>
#include "eeprom.h"

/*����������STC��Ƭ���ļĴ���*/

//sfr ISP_DATA  = 0xC2;
//sfr ISP_ADDRH = 0xC3;
//sfr ISP_ADDRL = 0xC4;
//sfr ISP_CMD   = 0xC5;
//sfr ISP_TRIG  = 0xC6;
//sfr ISP_CONTR = 0xC7;

#define uint  unsigned int
#define uchar unsigned char

#define ISP_BYTE_READ    1		//ISP�ֽڶ�
#define ISP_BYTE_PROGRAM 2		//ISPֱ�ӱ��
#define ISP_SECTOR_ERASE 3		//ISP��������

/********************************************************************
* ���� : Delay()
* ���� : ��ʱ,��ʱʱ��Ϊ 10ms * del
* ���� : del
* ��� : ��
***********************************************************************/
//void Delay_1ms(uint i)
//{
//	uchar x,j;
//	for(j=0;j<i;j++)
//	for(x=0;x<=148;x++);	
//}

/********************************************************************
* ���� : ISP_ERASE()
* ���� : EEPROM����������
* ���� : Address�����������ĵ�ַ)
* ��� : ��
***********************************************************************/
void ISP_ERASE(uint Address)
{
	Address += EEPROM_START_ADDRESS;
	ISP_ADDRL = Address;
	ISP_ADDRH = Address >> 8;
	ISP_CONTR = 0x83;				
	ISP_CMD = ISP_SECTOR_ERASE;	
	ISP_TRIG = 0x5a;	//PDF�ֲ���˵���� ISP_TRIG ��д�� 0x5a, Ȼ��д�� 0xa5
	ISP_TRIG = 0xa5;
	_nop_();
	_nop_();	
}

/********************************************************************
* ���� : ISP_READ()
* ���� : ��EEPROM��Ӧ�ĵ�ַ�ж���ֵ
* ���� : Address(��Ҫ��ȡEEPROM�ĵ�ַ��
* ��� : Read_Data����EEPROM������ֵ��
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
* ���� : ISP_PROGRAM()
* ���� : ʵ��EEPROM���ֽڱ��
* ���� : AddressΪ��̵ĵ�ַ��Data_tempΪ��Ҫ��̵�ֵ
* ��� : ��
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
