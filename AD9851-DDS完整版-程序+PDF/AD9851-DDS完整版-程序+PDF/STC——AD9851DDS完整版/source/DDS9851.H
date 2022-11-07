#ifndef _DDS9851_h_
#define _DDS9851_h_

# include <STC12C5A60S2.H>
# include <stdio.h>
# include <intrins.h>
sbit ad9851_w_clk    =P2^2;  //P2.2口接AD9851的w_clk脚/PIN7
sbit ad9851_fq_up    =P2^1;  //P2.1口接AD9851的fq_up脚/PIN8
sbit ad9851_rest     =P2^0;  //P2.0口接AD9851的rest脚/PIN12
sbit ad9851_bit_data =P0^7;  //P1.7口接AD9851的D7脚/PIN25
#define D_DATA P0
//P1为8位数据口

void ad9851_reset();
void ad9851_reset_serial();
void ad9851_wr_parrel(unsigned char w0,double frequence);
void ad9851_wr_serial(unsigned char w0,double frequence);
////////////////////////
//ad9851_reset();
//ad9851_wr_parrel(0x01,1000010);	//0X00为不开六备频 0X01为开六备频
////////////////////////

#endif
