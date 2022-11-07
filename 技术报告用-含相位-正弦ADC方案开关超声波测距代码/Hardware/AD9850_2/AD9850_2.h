#ifndef __AD9850_2_H
#define __AD9850_2_H
/*????????,??????*/
#define MODE_SEIAL   //????
//#define MODE_PARALLEL  //????
#define AD9850_SYSTEM_COLCK     7600000
/* AD9850 ????????? */
#define AD9850_CONTROL_PORT  GPIOA
#define AD9850_2_FQUD   GPIO_Pin_8
#define AD9850_2_WCLK   GPIO_Pin_9
#define AD9850_2_RST    GPIO_Pin_10
#ifdef MODE_PARALLEL  //???????0~7
#define AD9850_DATA_PORT  GPIOC
#endif
#ifdef MODE_SEIAL   //???????   
#define AD9850_2_DATA   GPIO_Pin_11  
#define AD9850_2_DATA_Write_1     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_DATA,Bit_SET)
#define AD9850_2_DATA_Write_0     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_DATA,Bit_RESET)
#endif 
#define AD9850_2_WCLK_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_WCLK,Bit_SET)
#define AD9850_2_WCLK_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_WCLK,Bit_RESET)
#define AD9850_2_FQUD_SET    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_FQUD,Bit_SET)
#define AD9850_2_FQUD_CLR    GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_FQUD,Bit_RESET)
#define AD9850_2_RST_SET     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_RST,Bit_SET)
#define AD9850_2_RST_CLR     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_2_RST,Bit_RESET)
/* ???????? */
#ifdef MODE_SEIAL    
extern void AD9850_2_Reset_Sreial(void) ;
extern void AD9850_2_Write_Serial(unsigned char W0,unsigned long freq) ;
#endif
/* ???????? */
#ifdef MODE_PARALLEL
extern void AD9850_Reset_Parallel(void) ;
extern void AD9850_Write_Parallel(unsigned char W0,unsigned long freq) ;
#endif
extern void Init_AD9850_2(void) ;
#endif /* AD8950_2_H */

