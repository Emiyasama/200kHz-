#ifndef _ADC_H_
#define	_ADC_H_

#define uchar unsigned char
#define uint unsigned int

/*Define ADC operation const for ADC_CONTR*/
#define   ADC_POWER     0x80               //ADC电源控制位
#define   ADC_FLAG      0x10               //ADC完成标志
#define   ADC_START   	0x08                //ADC开始标志位。
//AD速度设置
#define   ADC_SPEEDLL   0x00               //540 clocks
#define   ADC_SPEEDL    0x20               //360 clocks
#define   ADC_SPEEDH    0x40               //180 clocks
#define   ADC_SPEEDHH   0x60               //90 clocks


void AD_Init();

#endif 
