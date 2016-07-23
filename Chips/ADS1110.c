#include "ADS1110.h"
#include <LCD_ILI9325/LCD_ILI9325.h>

unsigned char ADS1110_ShiftIn8Bit();
void ADS1110_ShiftOut8Bit(unsigned char data);
void ADS1110_start();
void ADS1110_stop();
void ADS1110_get_ack();
void ADS1110_ack();

uint8_t ADS1110_Rrgister=0x8C;

///*
//配置ADS1110用到的I/O口
//*/
//void ADS1110_GPIO_Config(void)
//{		
//	/*定义一个GPIO_InitTypeDef类型的结构体*/
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	/*开启GPIOC的外设时钟*/
//	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE); 
//															   
//   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			 		//	 SCK
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOA, GPIO_Pin_8);
//
//   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			 		//	 SDA
//  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
//  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//  	GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC, GPIO_Pin_9);
//}

void ADS1110_Init(void)
{
	//ADS1110_GPIO_Config ();
	ADS1110_DIN_1;
	ADS1110_SCLK_1;
	ADS1110_Write(0x8C);
}

/*
ADS1110 FSYNC 拉低后，一个时钟信号一 bit ， 写8位数据
*/

void ADS1110_start()
{
	ADS1110_SCLK_1;
	ADS1110_DIN_1;
	ADS1110_DIN_0;
	delay_us(6);
	ADS1110_SCLK_0;
	delay_us(12);
}

void ADS1110_stop()
{
	ADS1110_DIN_0;
	ADS1110_SCLK_1;
	delay_us(6);
	ADS1110_DIN_1;
	delay_us(6);
}

void ADS1110_get_ack()
{
 	ADS1110_SCLK_0;		
	ADS1110_DIN_1;
	delay_us(12);
	ADS1110_SCLK_1;
 	delay_us(6);
 	ADS1110_SCLK_0;		//ack
	delay_us(12);

}
void ADS1110_ack()
{
// 	ADS1110_SCLK_0;  //ack
//	delay_us(5);
	ADS1110_DIN_0;
	delay_us(6);
	ADS1110_SCLK_1;
	delay_us(6);
	ADS1110_SCLK_0;
	delay_us(12);
	ADS1110_DIN_1;

}

unsigned char ADS1110_ShiftIn8Bit()
{
	char i=0;
	unsigned char tmp_in=0;
	for(i=0;i<8;i++)
	{
		ADS1110_SCLK_1;
		delay_us(6);
		tmp_in<<=1;
		if(ADS1110_DOUT)
		{
			tmp_in|=1;
		}
		ADS1110_SCLK_0;
		delay_us(12);	
	}
	return tmp_in;	
}
void ADS1110_ShiftOut8Bit(unsigned char data)
{
	char i=0;
	for(i=0;i<8;i++)
	{
		ADS1110_SCLK_0;
		//delay_us(5);
		if(data&0x80)
		{
			ADS1110_DIN_1;
		}
		else
		{
			ADS1110_DIN_0;
		}
		data<<=1;
		delay_us(12);
		ADS1110_SCLK_1;
		delay_us(6);	
	}
}

void ADS1110_Write(unsigned short data)
{	
	ADS1110_start();
	ADS1110_ShiftOut8Bit(ADS1110_ADDRESS);
	ADS1110_get_ack();
	ADS1110_ShiftOut8Bit(data);
	ADS1110_get_ack();
	ADS1110_stop();
}
 
int  ADS1110_Read()
{
	int16_t tmp_in=0;
	ADS1110_start();
	ADS1110_ShiftOut8Bit(ADS1110_ADDRESS|0x01);
	ADS1110_get_ack();
	tmp_in=ADS1110_ShiftIn8Bit();
	tmp_in<<=8;
	ADS1110_ack();
	tmp_in|=ADS1110_ShiftIn8Bit();
	ADS1110_ack();	  
	ADS1110_ShiftIn8Bit();
	ADS1110_ack();	   
	ADS1110_stop();	 

	return tmp_in;	
		 
}

