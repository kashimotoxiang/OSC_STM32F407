#pragma once
#include "main.h"
/*********************************************************************
*
*       extern  
*
**********************************************************************
*/
/*FPGA通信协议控制字-------------------------------------------------------*/
extern SPI_TYPE SPISTARTstr[];


#define FPGA_COM_SPISTART uint8_t mSPI3_SendString (SPISTARTstr,4);
/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/
void mSPI3_SendString (uint8_t *TxData,uint8_t Size);
/*-------------------------------------------------------*/
uint8_t mSPI1_ReadByte (uint8_t TxData);
void mSPI1_WriteByte (uint8_t TxData);
uint16_t mSPI1_ReadWriteByte (uint16_t TxData);
uint8_t mSPI2_ReadWriteByte (uint8_t TxData);
uint8_t mSPI3_ReadWriteByte (uint8_t TxData);

