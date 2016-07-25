#pragma once

#include "main.h"
#include "mxconstants.h"

#define Triangle_Wave     0x2002
#define Sine_Wave		  0x2028
/* AD9834����Ƶ�� */
#define AD9834_SYSTEM_COLCK     50000000UL
/* AD9834 �������� */
#define AD9834_FSYNC_SET   AD9834_FSYNC_GPIO_Port->ODR |=  AD9834_FSYNC_Pin
#define AD9834_FSYNC_CLR   AD9834_FSYNC_GPIO_Port->ODR &=~ AD9834_FSYNC_Pin
#define AD9834_SCLK_SET    AD9834_SCLK_GPIO_Port->ODR |=  AD9834_SCLK_Pin
#define AD9834_SCLK_CLR    AD9834_SCLK_GPIO_Port->ODR &=~ AD9834_SCLK_Pin
#define AD9834_SDATA_SET   AD9834_SDATA_GPIO_Port->ODR |=  AD9834_SDATA_Pin
#define AD9834_SDATA_CLR   AD9834_SDATA_GPIO_Port->ODR &=~ AD9834_SDATA_Pin
#define AD9834_RESET_SET   AD9834_RESET_GPIO_Port->ODR |= AD9834_RESET_Pin
#define AD9834_RESET_CLR   AD9834_RESET_GPIO_Port->ODR &=~AD9834_RESET_Pin

#define FREQ_0      0
#define FREQ_1      1 

/* AD9834�������� */
void AD9834_Write_16Bits (unsigned int data); //дһ���ֵ�AD9834
void AD9834_Select_Wave (unsigned int initdata); //ѡ���������
void AD9834_Set_Freq (unsigned char freq_number, unsigned long freq); //ѡ������Ĵ��������Ƶ��ֵ
void AD9834_Triangular_Wave ();

