#pragma once

//*****************************************************************************
//
// #include 
//
//*****************************************************************************
#include "main.h"
#include "LCD_ILI9325.h"

//*****************************************************************************
//
// declartion
//
//*****************************************************************************
void BSP_LCD_DrawPoint (u16 x, u16 y);
void BSP_LCD_DrawLine (u16 x1, u16 y1, u16 x2, u16 y2, u16 Color);
void BSP_Draw_Circle (u16 x0, u16 y0, u8 r, u16 Color);
void BSP_LCD_Fill (uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend);

