//*****************************************************************************
//
// #include  
//
//*****************************************************************************
#include "myUART.h"

//*****************************************************************************
//
// extern 
//
//*****************************************************************************
extern UART_HandleTypeDef huart3;

//*****************************************************************************
//
// variable
//
//*****************************************************************************
#define TX_TRANSMIT_SIZE 640
char TxBuffer[TX_TRANSMIT_SIZE] = {0};
char RxBuffer = 0;

/*-------------------------------------------------------*
*
*	UART_DMATX
*
*-------------------------------------------------------*/
void UART_DMATX (volatile uint16_t* sourceTxBuffer, uint16_t Tx_Begin, uint16_t Tx_End) {
	static uint16_t Tx_i = 0, So_i = 0, Tx_Size;
	Tx_Size = (Tx_End - Tx_Begin) * 2;
	//Tx_Size=Tx_End-Tx_Begin;//制定起始终止
	Tx_Size = Tx_Size < TX_TRANSMIT_SIZE ? Tx_Size : TX_TRANSMIT_SIZE;//取源大小和制定大小中的最小值
	for (Tx_i = 0 , So_i = 0; Tx_i < Tx_Size; So_i++) {
		TxBuffer[Tx_i++] = sourceTxBuffer[So_i] >> 8;
		TxBuffer[Tx_i++] = sourceTxBuffer[So_i] & 0xff;
	}
	/*  ---------------------------------------------------------*/
	if (HAL_UART_Transmit_DMA(&huart3, (uint8_t*)TxBuffer, Tx_Size) != HAL_OK) {
		/* Transfer error in transmission process */
		Error_Handler();
	}
}

/*-------------------------------------------------------*
*
*	UART_SendRecieve
*
*-------------------------------------------------------*/
uint8_t UART_SendReceive (uint8_t address) {

	USART3->DR = address;
	while ((USART3->SR & 0X40) == 0);//发送
	while ((USART3->SR & 0X20) == 0);//接受
	RxBuffer = USART3->DR;
	return RxBuffer;
}

