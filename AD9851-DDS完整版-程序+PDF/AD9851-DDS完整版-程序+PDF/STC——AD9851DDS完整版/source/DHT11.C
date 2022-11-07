
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "DHT11.H"
#include "stdio.h"
#define over_time 2000
u16 Get_Tem_Dam(u8 *dam_integer, u8 *tem_integer)
{
	xdata u8 dam_decimal; 
	xdata u8 tem_decimal;
	xdata u8 i, j, DATA;
	xdata u16 time;
	xdata u8 validate, temp;
	xdata u16 Tem_Dam = 0;
	xdata u32 Tem_time;
	static xdata u8 access = 1;
	xdata u16 breaktime = 0;
	DATA_DHT = 0;
	if(access == 1)
	{
		Tem_time = SystemTime;
		access = 0;
	}	
	if(SystemTime >= (Tem_time+3))
	{
		access = 1;
		DATA_DHT = 1;

		while(DATA_DHT == 1)
		{
			breaktime++;
			if(breaktime > over_time) 
				return 0;	  
		}
		breaktime = 0;
		while(DATA_DHT == 0)
		{
			breaktime++;
			if(breaktime > over_time) 
				return 0;
		}
		while(DATA_DHT == 1)
		{
			breaktime++;
			if(breaktime > over_time)
				return 0;
		}
		for(i = 0; i < 5; i++)
		{
			DATA = 0;
			TR0 = 0; ET0 = 0;
			for(j = 0; j < 8; j++)
			{
				DATA = DATA<<1;
				TH0 = 0; TL0 = 0;
				breaktime = 0;
				while(DATA_DHT == 0)
				{
					breaktime++;
					if(breaktime > over_time)
					{
						ET0 = 1; TR0 = 1; 
						return 0;
					}
				}
				breaktime = 0;
				TR0 = 1;
				while(DATA_DHT == 1)
				{
					breaktime++;
					if(breaktime > over_time)
					{
						ET0 = 1; TR0 = 1;
						return 0;
					}
				}
				TR0 = 0;
				time = 256*TH0 + TL0;
				if((time>2)&&(time < 40))
					DATA |= 0;
				else if((time>50)&&(time < 100))
					DATA |= 1;
			}
			if(i == 0) 		*dam_integer = DATA;
			else if(i == 1) dam_decimal = DATA;
			else if(i == 2) *tem_integer = DATA;
			else if(i == 3) tem_decimal = DATA;
			else if(i == 4) validate = DATA;
		}
		DATA_DHT = 1;
		temp = *dam_integer + dam_decimal + *tem_integer + tem_decimal;
		if(validate != temp) 
		{
			ET0 = 1; TR0 = 1;
			return 0;
		}
		else 
		{
			Tem_Dam = ((u16)*tem_integer<<8) + ((u16)*dam_integer);
		}
		ET0 = 1;
		TR0 = 1;	
	}
	return Tem_Dam;
}
