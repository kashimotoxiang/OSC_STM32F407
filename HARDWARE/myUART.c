//*****************************************************************************
//
// #include  
//
//*****************************************************************************
#include "myUART.h"

//*****************************************************************************
//
// variable
//
//*****************************************************************************
char RxBuffer = 0;

/*-------------------------------------------------------*
*
*	UART_SendRecieve
*
*-------------------------------------------------------*/
inline uint8_t UART_SendReceive8bit (uint8_t address) {
	USART3->DR = address;
	while ((USART3->SR & 0X40) == 0);//发送
	while ((USART3->SR & 0X20) == 0) {
	}//接受
	RxBuffer = USART3->DR;
	return RxBuffer;
}

/*-------------------------------------------------------*
*
*	UART_Send8Bit
*
*-------------------------------------------------------*/
inline void UART_Send8Bit (uint8_t address) {
	USART3->DR = address;
	while ((USART3->SR & 0X40) == 0);//发送
}

/*-------------------------------------------------------*
*
*	UART_Send16Bit
*
*-------------------------------------------------------*/
inline void UART_Send16Bit (uint16_t address) {
	USART3->DR = (uint8_t)((address >> 8) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送
	USART3->DR = (uint8_t)((address) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送
}

/*-------------------------------------------------------*
*
*	UART_Send32Bit
*
*-------------------------------------------------------*/
inline void UART_Send32Bit (uint32_t address) {
	USART3->DR = (uint8_t) ((address >> 24) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送
	USART3->DR = (uint8_t) ((address >> 16) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送	
	USART3->DR = (uint8_t) ((address >> 8) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送
	USART3->DR = (uint8_t) ((address) & 0xFF);
	while ((USART3->SR & 0X40) == 0);//发送
}

