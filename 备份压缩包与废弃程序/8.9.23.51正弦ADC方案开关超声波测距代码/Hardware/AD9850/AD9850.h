//#ifndef __AD9850_H
//#define __AD9850_H
//#include "sys.h"

//#define AD9850_CONTROL_PORT  GPIOA
//#define FQ_QD_AD9850    GPIO_Pin_0
//#define CLK_AD9850      GPIO_Pin_1
//#define DataIn_AD9850   GPIO_Pin_2
//#define RST_AD9850      GPIO_Pin_3

//#define AD9850_WCLK_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,CLK_AD9850,Bit_SET)
//#define AD9850_WCLK_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,CLK_AD9850,Bit_RESET)
//#define AD9850_FQUD_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,FQ_QD_AD9850,Bit_SET)
//#define AD9850_FQUD_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,FQ_QD_AD9850,Bit_RESET)
//#define AD9850_RST_SET     GPIO_WriteBit(AD9850_CONTROL_PORT,RST_AD9850,Bit_SET)
//#define AD9850_RST_CLR     GPIO_WriteBit(AD9850_CONTROL_PORT,RST_AD9850,Bit_RESET)

//#define AD9850_DATA_Write_1     GPIO_WriteBit(AD9850_CONTROL_PORT,DataIn_AD9850,Bit_SET)
//#define AD9850_DATA_Write_0     GPIO_WriteBit(AD9850_CONTROL_PORT,DataIn_AD9850,Bit_RESET)

//unsigned long int freq=0;
//unsigned char Control_AD9850=0X00;
//void SentFreq(void);
//void Set_Freq(unsigned long int Freqency);

//#endif







#ifndef __AD9850_H
#define __AD9850_H

#define MODE_SEIAL   //????
#define AD9850_SYSTEM_COLCK     7600000

#define AD9850_CONTROL_PORT  GPIOA
#define AD9850_FQUD   GPIO_Pin_0
#define AD9850_WCLK   GPIO_Pin_1
#define AD9850_RST    GPIO_Pin_2
#define AD9850_DATA   GPIO_Pin_3 
#ifdef MODE_PARALLEL  //???????0~7
#define AD9850_DATA_PORT  GPIOC
#endif
#ifdef MODE_SEIAL   //???????  
#define AD9850_DATA_Write_1     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_SET)
#define AD9850_DATA_Write_0     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_RESET)
#endif 

#define AD9850_WCLK_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_SET)
#define AD9850_WCLK_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_RESET)
#define AD9850_FQUD_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_SET)
#define AD9850_FQUD_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_RESET)
#define AD9850_RST_SET     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_SET)
#define AD9850_RST_CLR     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_RESET)
/* ???????? */
#ifdef MODE_SEIAL    
extern void AD9850_Reset_Sreial(void) ;
extern void AD9850_Write_Serial(unsigned char W0,unsigned long freq) ;
#endif

#ifdef MODE_PARALLEL
extern void AD9850_Reset_Parallel(void) ;
extern void AD9850_Write_Parallel(unsigned char W0,unsigned long freq) ;
#endif
extern void Init_AD9850(void) ;
#endif /* AD8950_H */

