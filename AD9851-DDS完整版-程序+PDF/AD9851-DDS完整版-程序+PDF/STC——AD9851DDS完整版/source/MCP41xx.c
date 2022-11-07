
/**********************************************************
                       �����Ƽ�
������������뵽�Ա��꣬�������ӽ߳�Ϊ������ ^_^
https://kvdz.taobao.com/
**********************************************************/

#include "MCP41xx.h"
#include "STC12C5A60S2.H"


void mcp_delay(uint n)
{
	n=n*110;
	while(n--);
}

void MCP41xx_writedata(uchar dat1)		//�������ֵ�λ��
{
	
  uchar i,command=0x11;
	
	MCP41xx_SPI_CS_H();//cs=1; 
	MCP41xx_SPI_CLK_L();//sck=1;
	MCP41xx_SPI_CS_L();//cs=0;

 	 mcp_delay(10);

	for(i=0;i<8;i++)          //д����
	{ 
		if(command & 0x80)
		{
			MCP41xx_SPI_DAT_H();//si=1;	
		}
		else
		{
			MCP41xx_SPI_DAT_L();//si=0;
		}
		mcp_delay(10);
		MCP41xx_SPI_CLK_L();//sck=1;//sck=1;
		mcp_delay(10);
		MCP41xx_SPI_CLK_H();//sck=0;sck=0;
		mcp_delay(10); 

		command=command<<1;
	}

     
	for(i=0;i<8;i++)          //д����
	{ 
		
		if(dat1 & 0x80)
		{
			MCP41xx_SPI_DAT_H();//si=1;	
		}
		else
		{
			MCP41xx_SPI_DAT_L();//si=0;
		}
		mcp_delay(10);
		MCP41xx_SPI_CLK_L();//sck=1;//sck=1;
		mcp_delay(10);
		MCP41xx_SPI_CLK_H();//sck=0;sck=0;
		mcp_delay(10);
		dat1=dat1<<1;
	}

	MCP41xx_SPI_CS_H();//cs=1; //cs=1;

	mcp_delay(10);//_nop_();
}
