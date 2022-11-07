#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
/********************ADC1����ͨ�������ţ�����**************************/
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA 
#define    ADC_PORT                      GPIOA

// ע��
// 1-PC0 �ڰԵ�����ӵ��Ƿ�������Ĭ�ϱ�����
// 2-PC0 ��ָ��������ӵ���SPI FLASH�� Ƭѡ��Ĭ�ϱ�����
// ���� PC0 �� ADC ת��ͨ����ʱ�򣬽�����ܻ������

// ת��ͨ������
#define    NOFCHANEL										2

//#define    ADC_PIN1                      GPIO_Pin_1
//#define    ADC_CHANNEL1                  ADC_Channel_1

//#define    ADC_PIN2                      GPIO_Pin_2
//#define    ADC_CHANNEL2                  ADC_Channel_2

//#define    ADC_PIN3                      GPIO_Pin_3
//#define    ADC_CHANNEL3                  ADC_Channel_3

#define    ADC_PIN4                      GPIO_Pin_4
#define    ADC_CHANNEL4                  ADC_Channel_4

#define    ADC_PIN5                      GPIO_Pin_5
#define    ADC_CHANNEL5                  ADC_Channel_5

//#define    ADC_PIN6                      GPIO_Pin_6
//#define    ADC_CHANNEL6                  ADC_Channel_6


// ADC1 ��Ӧ DMA1ͨ��1��ADC3��ӦDMA2ͨ��5��ADC2û��DMA����
#define    ADC_x                         ADC1
#define    ADC_DMA_CHANNEL               DMA1_Channel1
#define    ADC_DMA_CLK                   RCC_AHBPeriph_DMA1


/**************************��������********************************/
void               ADCx_Init                               (void);


#endif /* __ADC_H */

