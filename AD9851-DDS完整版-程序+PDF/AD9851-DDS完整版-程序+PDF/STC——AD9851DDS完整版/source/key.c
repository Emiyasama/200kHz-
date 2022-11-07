#include "key.h"
#include "oled.h"
#include "DDS9851.H"
#define Fosc 1000000
//#define Fosc 12000000
#define f_inter 100 //
//#define KEYS ((P2|0XF8)&(((P3&0X80)>>4)|0XF7))
#define KEYS ((P3>>2)|0XF0)
u32 SystemTime;
u32 KEY_Time;
u32 key_Trgtime;
u8 KEY_Trg;
u8 KEY_Cont=0;
u8 Trg_state;
u8 Trg_state = NO;
u8 Keycode;
extern u8 Sweeper_state;

unsigned char KeyRead(void)
{
    unsigned char ReadData = KEYS^0xff;   // 1
    KEY_Trg = ReadData & (ReadData ^ KEY_Cont);      // 2
    KEY_Cont = ReadData;

	if(!Trg_state) 
		key_Trgtime = SystemTime + T_10mS;
	if(KEY_Trg)
		Trg_state = YES;
	if(Trg_state)
	{
		if(SystemTime > key_Trgtime)  //??
		{
			switch(KEY_Cont)
			{
//				case 1:{Keycode = K_L_S; break;}
//				case 2:{Keycode = K_R_S; break;}
//				case 4:{Keycode = K_U_S; break;}
//				case 8:{Keycode = K_D_S; break;}
//				case 12:{Keycode = K_UD_S; break;}
//				case 3:{Keycode = K_LR_S; break;}
//				default:{Keycode = K_NO; break;}
				case 1:{Keycode = K_L_S; break;}
				case 2:{Keycode = K_D_S; break;}
				case 4:{Keycode = K_R_S; break;}
				case 8:{Keycode = K_U_S; break;}
				case 10:{Keycode = K_UD_S; break;}
				case 5:{Keycode = K_LR_S; break;}
				default:{Keycode = K_NO; break;}
			}	
		}
	}
	if(KEY_Time >= T_1S)	  	//
	{ 
		KEY_Time = T_1S-T_Cont;
		switch(KEY_Cont)
		{
			case 1:{Keycode = K_L_L; break;}
			case 2:{Keycode = K_D_L; break;}
			case 4:{Keycode = K_R_L; break;}
			case 8:{Keycode = K_U_L; break;}
			case 10:{Keycode = K_UD_L; break;}
			case 5:{Keycode = K_LR_L; break;} 
			default:{Keycode = K_NO; break;} 
		}
	}
	return Keycode;
}
void KEY_EXIT(void)
{
	if(Keycode != K_NO)
	{
		Trg_state = NO;
		Keycode = K_NO;
	}	
}

void KEY_TIM_Init(void)  
{
//	TMOD &= 0XF0;
//	TMOD |= 0X01;
//	TL0 = (65536-Fosc/f_inter)%256;
//	TH0 = (65536-Fosc/f_inter)/256;

	TMOD &= 0XF0;
	TMOD |= 0X02;
	TL0 = 256-100;
	TH0 = 256-100;

	ET0 = 1;
	EA = 1;
	TR0 = 1;
}
void Key_Init(void)
{
	KEY_TIM_Init();
}
static u32 sweeper_fre = 0;
/*************为按键提供系统时间***************/
static u16 smallbase = 0;
void Timer0_inter(void) interrupt 1
{
//	TL0 = (65536-Fosc/f_inter)%256;
//	TH0 = (65536-Fosc/f_inter)/256;
	if(Sweeper_state == 1)
	{
		ad9851_wr_parrel(0x01,sweeper_fre);
		sweeper_fre += 5;
		if(sweeper_fre >= 80000000)	 
		{
			sweeper_fre = 0;
			LCD_Show_CEStr(2,2,"扫频结束");
		}
	}
	smallbase++;
	if(smallbase == 100)
	{
		smallbase = 0;
		SystemTime++;
		if(KEY_Cont)
			KEY_Time++;
		else
			KEY_Time = 0;
	}
}
