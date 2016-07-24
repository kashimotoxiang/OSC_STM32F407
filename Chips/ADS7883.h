#include "ChipsDefine.h"
/*
#define   ADS7883_CS(a)	if (a)	\
				 GPIO_SetBits(GPIOC,GPIO_Pin_7);\
				 else		\
				 GPIO_ResetBits(GPIOC,GPIO_Pin_7)					
*/

#define   ADS7883_CS_1 		ADS7883_CS_GPIO_Port->ODR|=ADS7883_CS_Pin
#define   ADS7883_CS_0		ADS7883_CS_GPIO_Port->ODR&=~ADS7883_CS_Pin			//  CS

#define   ADS7883_SCLK_1	ADS7883_SCLK_GPIO_Port->ODR|=ADS7883_SCLK_Pin
#define   ADS7883_SCLK_0	ADS7883_SCLK_GPIO_Port->ODR&=~ADS7883_SCLK_Pin

#define   ADS7883_DOUT		((ADS7883_DOUT_GPIO_Port->IDR) & ADS7883_CS_Pin)		//  CS

//void ADS7883_Init(void);
uint16_t ADS7883_Read ();

