
/**********************************************************
                       ¿µÍþ¿Æ¼¼
¸ü¶àµç×ÓÐèÇó£¬Çëµ½ÌÔ±¦µê£¬¿µÍþµç×Ó½ß³ÏÎªÄú·þÎñ ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "STC12C5A60S2.H"
#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8 unsigned char 
#define  u32 unsigned long int 
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define OLED_MODE 0
#define Nomal (u8)1
#define InNomal (u8)0

//sbit OLED_CS=P1^4; //Æ¬Ñ¡
//sbit OLED_RST =P1^2;//¸´Î»
//sbit OLED_DC =P1^3;//Êý¾Ý/ÃüÁî¿ØÖÆ
//sbit OLED_SCL=P1^0;//Ê±ÖÓ D0£¨SCLK£
//sbit OLED_SDIN=P1^1;//D1£¨MOSI£© Êý¾Ý

sbit OLED_CS=P2^7; //Æ¬Ñ¡
sbit OLED_RST =P1^0;//¸´Î»
sbit OLED_DC =P1^1;//Êý¾Ý/ÃüÁî¿ØÖÆ
sbit OLED_SCL=P1^3;//Ê±ÖÓ D0£¨SCLK£
sbit OLED_SDIN=P1^2;//D1£¨MOSI£© Êý¾Ý

//sbit LCD_SCL=P1^3; //Ê±ÖÓ D0£¨SCLK£©
//sbit LCD_SDA=P1^2; //D1£¨MOSI£© Êý¾Ý
//sbit LCD_RST=P1^0; //¸´Î» 
//sbit LCD_DC =P1^1; //Êý¾Ý/ÃüÁî¿ØÖÆ


#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;





//OLEDÄ£Ê½ÉèÖÃ
//0:4Ïß´®ÐÐÄ£Ê½
//1:²¢ÐÐ8080Ä£Ê½

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

void delay_ms(unsigned int ms);


 		     

//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(u8 dat,u8 cmd);	    
//void OLED_Display_On(void);
//void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
//void OLED_Show_expect_damp(u8 x, u8 y);
//void OLED_ShowmodeNum(u8 x,u8 y,u32 num,u8 len,u8 size2,u8 show_mode);
void OLED_ShowmodeChar(u8 x,u8 y,u8 chr,u8 show_mode);
void LCD_Show_CEStr(u16 x0,u16 y0, u8 *pp);
void LCD_Show_modeCEStr(u16 x0,u16 y0, u8 *pp, u8 mode);
void draw_cell(u8 x, u8 y, u8 remain_len, u8 all_len);
//void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
#endif  
	 



