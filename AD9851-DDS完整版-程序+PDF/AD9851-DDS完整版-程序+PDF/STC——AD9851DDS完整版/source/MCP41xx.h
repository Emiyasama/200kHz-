
/**********************************************************
                       康威科技
更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://kvdz.taobao.com/
**********************************************************/

#ifndef __MCP41xx_h
#define __MCP41xx_h

#include "STC12C5A60S2.H"
#define uint unsigned int
#define uchar unsigned char

#define DATA_CD 0x11
			
sbit P_MCP_CS	 = P2^3;	
sbit P_MCP_CLK	 = P2^4;
sbit P_MCP_DAT	 = P2^5;

#define MCP41xx_SPI_CLK_H()     P_MCP_CLK=1	//GPIOB->BSRR = MCP41xx_SPI_CLK_BIT;		   
#define MCP41xx_SPI_CLK_L()     P_MCP_CLK=0	//GPIOB->BRR =  MCP41xx_SPI_CLK_BIT;

#define MCP41xx_SPI_DAT_H()     P_MCP_DAT=1	//GPIOB->BSRR = MCP41xx_SPI_DAT_BIT;
#define MCP41xx_SPI_DAT_L()     P_MCP_DAT=0	//GPIOB->BRR =  MCP41xx_SPI_DAT_BIT;

#define MCP41xx_SPI_CS_H()      P_MCP_CS=1	//GPIOB->BSRR = MCP41xx_SPI_CS_BIT;
#define MCP41xx_SPI_CS_L()      P_MCP_CS=0	//GPIOB->BRR =  MCP41xx_SPI_CS_BIT;

void mcp_delay(uint n);
void MCP41xx_writedata(uchar dat1);


#endif 
