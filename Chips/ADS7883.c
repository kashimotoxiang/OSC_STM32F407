/******************** (C) COPYRIGHT 2013 ***************************
 * 文件名  ：ADS7883.c
 * 描述    ：ADS7883 应用函数库         
 * 实验平台：
 * 硬件连接：-----------------
 *          |   PE5 - FSYNC    |
 *          |   PE6 - SCLK     |
 *          |   PE4 - DATA     |
 *           ----------------- 
 * 库版本  ：ST3.5.0   
**********************************************************************************/
#include "ADS7883.h"
/*
配置ADS7883用到的I/O口
*/
	/*
void ADS7883_Init(void)
{		

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE, ENABLE); 
															   
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;			 					//	 SDA3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15 ;			 	    //	 ADC-CS,SCK3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_SetBits(GPIOE, GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15);	 
}
  */

uint16_t ADS7883_Read()
{
	unsigned char i;
	uint16_t tmp_in=0;

	ADS7883_CS_0;
	/*
	ADS7883_SCLK_0;
	ADS7883_SCLK_1;
	ADS7883_SCLK_0;
	ADS7883_SCLK_1;	  */
	ADS7883_SCLK_0;	   
	for(i=0;i<13;i++)
	{
		ADS7883_SCLK_1;
		tmp_in<<=1;
		if(ADS7883_DOUT)
		{
			tmp_in|=1;
		}					
		ADS7883_SCLK_0;
	}		  
	ADS7883_SCLK_1;
	ADS7883_CS_1;
	return tmp_in;
}

