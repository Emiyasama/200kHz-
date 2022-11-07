//#include "AD9850.h"

//void SentFreq(void)
//{
//	unsigned char i;
//	unsigned int temp;
//	AD9850_FQUD_CLR;	//FQ_QD_AD9850=0;
//	for(i=0;i<32;i++) //串口数据 输入频率控制字
//	{
//		AD9850_WCLK_CLR; //CLK_AD9850= 0;
//		temp=( ( freq >> i ) & 1 );
//		DataIn_AD9850 = temp;
//		AD9850_WCLK_SET; //CLK_AD9850 = 1;
//	}
//	for(i=0;i<8;i++) 
//	//phase-b4 ph-b3 ph-b2 ph-b1 ph-b0 
//	{
//		AD9850_WCLK_CLR;//CLK_AD9850 = 0;
//		temp=( ( Control_AD9850 >> i ) & 1 );
//		DataIn_AD9850 = temp;
//		AD9850_WCLK_SET; //CLK_AD9850 = 1;
//	}
//	AD9850_WCLK_CLR;//CLK_AD9850 = 0;
//	AD9850_FQUD_SET;//FQ_QD_AD9850= 1;
//	AD9850_FQUD_CLR;//FQ_QD_AD9850 = 0;
//}

//void Set_Freq(unsigned long int Freqency)
//{
//	freq= (unsigned long int)(34.36*Freqency);
//	// SYSCLK = 125 MHz 2^32/125000000=34.36
//	SentFreq();
//}



#include "stm32f10x.h"
#include "AD9850.h"

void AD9850_Delay(unsigned int z)
{
    for(;z>0;z--)
    {;}
}

/*------------------------????-------------------------*/
/*---------------------------------------------------------*/
/*------------------------????-------------------------*/
#ifdef MODE_PARALLEL   

void AD9850_Reset_Parallel(void)
{
    AD9850_WCLK_CLR ;
    AD9850_FQUD_CLR ;
    //RST??
    AD9850_RST_CLR ;
    AD9850_RST_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_RST_CLR ;
}

void AD9850_Write_Parallel(unsigned char W0,unsigned long freq)
{
    unsigned char wdata ;
    unsigned long  y=(unsigned long)(4294967295/AD9850_SYSTEM_COLCK) ;
    y *= freq ;
    wdata=W0 ;  //?w0
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>24 ;  //?w1
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>16 ;  //?w2
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>8 ;  //?w3
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>0 ;  //?w4
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    AD9850_FQUD_SET ;  //????
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}
#endif /* MODE_PARALLEL */




void AD9850_Reset_Sreial(void)
{
    AD9850_WCLK_CLR ;
    AD9850_FQUD_CLR ;
    //RST??
    AD9850_RST_CLR ;
    AD9850_RST_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_RST_CLR ;
    //WCLK??
    AD9850_WCLK_CLR ;
    AD9850_WCLK_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_WCLK_CLR ;
    //FQUD??
    AD9850_FQUD_CLR ;
    AD9850_FQUD_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}
/********************************************
????:AD9850_Write_Serial
?    ?:???????AD9850???
?    ?:W0 - W0?????
          freq - ???
???  :?
*********************************************/
void AD9850_Write_Serial(unsigned char W0,unsigned long freq)
{
    unsigned char i,wdata ;
    unsigned long  y ;
    y = (unsigned long )268435456.0/AD9850_SYSTEM_COLCK*freq ; 
    //268435456.0=2^28	
    //y = (unsigned long )AD9850_SYSTEM_COLCK*freq ; 
    wdata = y>>0 ;   //?w4
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>8 ;  //?w3
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>16 ;  //?w2
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>24 ;  //?w1
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = W0 ;  //?w0
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    
    AD9850_FQUD_SET ;  //????
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}

/*------------------------????-------------------------*/
/*---------------------------------------------------------*/
/********************************************
????:Init_AD9834
?    ?:???AD9834
?    ?:?
???  :?
*********************************************/
void Init_AD9850(void)
{

    GPIO_InitTypeDef GPIO_InitStructure ;
    GPIO_InitStructure.GPIO_Pin = AD9850_WCLK | AD9850_FQUD | AD9850_RST | AD9850_DATA ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(AD9850_CONTROL_PORT ,&GPIO_InitStructure) ;
    
    AD9850_Reset_Sreial() ;

}
