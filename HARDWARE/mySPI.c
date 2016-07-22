#include "mySPI.h"

/*********************************************************************
*
*       通信协议数组
*
**********************************************************************
*/

SPI_TYPE SPISTARTstr[] = "2354";
/*********************************************************************
*
*       Function
*
**********************************************************************
*/
/***********************************
*函数功能：SPI_1收发（寄存器版本）
*形参说明：
*返回值说明
*创建时间：
***********************************/
void mSPI3_SendString (uint8_t *TxData,uint8_t Size) {
	while(Size--){
			while ((SPI3->SR & 1 << 1) == 0); //等待发送区空 
			SPI3->DR = *TxData; //发送一个byte  	
		  TxData++;
	}
		    
}

/***********************************
*函数功能：SPI_1收发（寄存器版本）
*形参说明：
*返回值说明
*创建时间：
***********************************/
uint8_t mSPI1_ReadByte (uint8_t TxData) {
	return SPI1->DR; //返回收到的数据				    
}

void mSPI1_WriteByte (uint8_t TxData) {
	while ((SPI1->SR & 1 << 1) == 0); //等待发送区空 
	SPI1->DR = TxData; //发送一个byte  
}

/***********************************
*函数功能：SPI_1快速收发（寄存器版本）
*形参说明：
*返回值说明
*创建时间：
***********************************/
uint16_t mSPI1_ReadWriteByte (uint16_t TxData) {
	static int spi_state;
	while ((SPI1->SR & 0x10)); //等待发送区空 
	SPI1->DR = TxData; //发送一个byte  
	spi_state = SPI1->SR & 0x01;
	while (spi_state != 0)
		spi_state = SPI1->SR & 0x01; //等待接收完一个byte  
	return SPI1->DR; //返回收到的数据				    
}

/***********************************
*函数功能：SPI_2快速收发（寄存器版本）
*形参说明：
*返回值说明
*创建时间：
***********************************/
uint8_t mSPI2_ReadWriteByte (uint8_t TxData) {
	static int spi_state;
	while ((SPI2->SR & 0x10)); //等待发送区空 
	SPI2->DR = TxData; //发送一个byte  
	spi_state = SPI2->SR & 0x01;
	while (spi_state != 0)
		spi_state = SPI2->SR & 0x01; //等待接收完一个byte  
	return SPI2->DR; //返回收到的数据				    
}

/***********************************
*函数功能：SPI_3快速收发（寄存器版本）
*形参说明：
*返回值说明
*创建时间：
***********************************/
uint8_t mSPI3_ReadWriteByte (uint8_t TxData) {
	while ((SPI3->SR & 1 << 1) == 0); //等待发送区空 
	SPI3->DR = TxData; //发送一个byte  
	while ((SPI3->SR & 1 << 0) == 0); //等待接收完一个byte  
	return SPI3->DR; //返回收到的数据				    
}

