#pragma once

//*****************************************************************************
//
// #include 
//
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//
// declartion
//
//*****************************************************************************
uint8_t UART_SendReceive8bit (uint8_t address);
void UART_Send8Bit (uint8_t address);
void UART_Send16Bit (uint16_t address);
void UART_Send32Bit (uint32_t address);

