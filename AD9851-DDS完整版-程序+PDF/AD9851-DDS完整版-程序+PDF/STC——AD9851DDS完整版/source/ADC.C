#include "STC12C5A60S2.H"
#include "ADC.H"

volatile u8 ADC_Value;

void AD_Init()
{
	uint i=0;
	P1ASF=0x10;//仅P1^0作为模拟输入。
	ADC_RES=0;
	ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START;
	for(i=0;i<10;i++);//延时，打开总中断，AD中断。
	EADC=1;
}

void adc_isr() interrupt 5 using 1
{
      ADC_CONTR &= !ADC_FLAG;           //清楚AD的标志位。
	  ADC_Value=ADC_RES;
	  ADC_CONTR = ADC_POWER | ADC_SPEEDHH | ADC_START;	//重新将配置写入AD配置寄存器。
}
