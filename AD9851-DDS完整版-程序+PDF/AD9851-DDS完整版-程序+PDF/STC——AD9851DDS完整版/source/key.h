#ifndef __key_H
#define __key_H
#include "STC12C5A60S2.H"

#define YES	  1
#define NO	  0

#define T_10mS	((u32)1)
#define T_1S	((u32)100)
#define T_330mS	((u32)33)
#define T_200mS	((u32)20)

#define T_Cont	T_200mS

#define K_L_S		0X01       
#define K_U_S		0X02        
#define K_R_S		0X03
#define K_D_S		0X04

#define K_L_L		0X05       
#define K_U_L		0X06        
#define K_R_L		0X07
#define K_D_L		0X08
#define K_UD_S		0X09
#define K_LR_S		0X10
#define K_UD_L		0X11
#define K_LR_L		0X12

#define K_NO		0X00

extern u32 SystemTime;
extern u32 KEY_Time;
extern u32 key_Trgtime;
extern u8 KEY_Trg;
extern u8 KEY_Cont;
extern u8 Trg_state;


#define KeyIn0 PEin(2)
#define KeyIn1 PEin(3)
#define KeyIn2 PEin(4)
//sbit KEY_UP  = P2^0;
//sbit KEY_DOWN  = P2^1;
//sbit KEY_LEFT  = P2^2;
//sbit KEY_RIGHT = P2^3;
unsigned char KeyRead(void);
void KEY_EXIT(void);
void KEY_TIM_Init(void);
void Key_Init(void);


#endif
