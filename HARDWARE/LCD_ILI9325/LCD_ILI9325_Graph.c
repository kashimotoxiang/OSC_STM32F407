#include "LCD_ILI9325_Graph.h"

//画点
//x,y:坐标
//BSP_POINT_Color:此点的颜色
inline void BSP_LCD_DrawPoint (u16 x, u16 y) {
	BSP_LCD_SetCursor(x, y); //设置光标位置 
	BSP_LCD_WriteRAM_Prepare (); //开始写入GRAM
	BSP_LCD_WR_DATA (BSP_POINT_COLOR);
}


/****************************两点一线****************************/
//函数功能：画两点一线
//入口参数: x1,y1     直线的起点;
//			    x2,y2     直线的终点
//          Color     直线的颜色
//出口参数: 无
//说明：    该函数是在LCD上画一条直线
/****************************************************************/
void BSP_LCD_DrawLine (u16 x1, u16 y1, u16 x2, u16 y2, u16 Color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
		yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
		curpixel = 0;

	deltax = abs(x2 - x1); /* The difference between the x's */
	deltay = abs(y2 - y1); /* The difference between the y's */
	x = x1; /* Start x off at the first pixel */
	y = y1; /* Start y off at the first pixel */

	if (x2 >= x1) /* The x-values are increasing */ {
		xinc1 = 1;
		xinc2 = 1;
	}
	else /* The x-values are decreasing */ {
		xinc1 = -1;
		xinc2 = -1;
	}
	if (y2 >= y1) /* The y-values are increasing */ {
		yinc1 = 1;
		yinc2 = 1;
	}
	else /* The y-values are decreasing */ {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay) /* There is at least one x-value for every y-value */ {
		xinc1 = 0; /* Don't change the x when numerator >= denominator */
		yinc2 = 0; /* Don't change the y for every iteration */
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax; /* There are more x-values than y-values */
	}
	else /* There is at least one y-value for every x-value */ {
		xinc2 = 0; /* Don't change the x for every iteration */
		yinc1 = 0; /* Don't change the y when numerator >= denominator */
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay; /* There are more y-values than x-values */
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		BSP_LCD_DrawPoint(x, y);//画点 
		num += numadd; /* Increase the numerator by the top of the fraction */
		if (num >= den) /* Check if numerator >= denominator */ {
			num -= den; /* Calculate the new numerator value */
			x += xinc1; /* Change the x as appropriate */
			y += yinc1; /* Change the y as appropriate */
		}
		x += xinc2; /* Change the x as appropriate */
		y += yinc2; /* Change the y as appropriate */
	}
}


/*****************************************************************************
** 函数名称: BSP_LCD_DrawLine
** 功能描述: 在指定位置画一个指定大小的圆
(x,y):中心点 	 r    :半径
*****************************************************************************/
void BSP_Draw_Circle (u16 x0, u16 y0, u8 r, u16 Color) {
	int a = 0, b = 0;
	int di = 0;
	a = 0;
	b = r;
	di = 1 - r; //判断下个点位置的标志
	while (a <= b) {
		BSP_LCD_DrawPoint(x0 - b, y0 - a); //3           
		BSP_LCD_DrawPoint(x0 + b, y0 - a); //0           
		BSP_LCD_DrawPoint(x0 - a, y0 + b); //1       
		BSP_LCD_DrawPoint(x0 - b, y0 - a); //7           
		BSP_LCD_DrawPoint(x0 - a, y0 - b); //2             
		BSP_LCD_DrawPoint(x0 + b, y0 + a); //4               
		BSP_LCD_DrawPoint(x0 + a, y0 - b); //5
		BSP_LCD_DrawPoint(x0 + a, y0 + b); //6 
		BSP_LCD_DrawPoint(x0 - b, y0 + a);
		a++;
		if (di < 0) di += (a << 1) + 3;
		else {
			di += ((a - b) << 1) + 5;
			b--;
		}
	}
}

/*****************************************************************************
** 函数名称: BSP_LCD_Fill
** 功能描述: 在指定位置画一个指定大小的矩形填充

*****************************************************************************/


inline void BSP_LCD_Fill (uint8_t xsta, uint16_t ysta, uint8_t xend, uint16_t yend) {
	uint32_t n;
	//设置窗口										
	BSP_LCD_WriteReg (0x50, xsta); //水平方向GRAM起始地址
	BSP_LCD_WriteReg (0x51, xend); //水平方向GRAM结束地址
	BSP_LCD_WriteReg (0x52, ysta); //垂直方向GRAM起始地址
	BSP_LCD_WriteReg (0x53, yend); //垂直方向GRAM结束地址	
	BSP_LCD_SetCursor(xsta, ysta);//设置光标位置  
	BSP_LCD_WriteRAM_Prepare (); //开始写入GRAM	 	   	   
	n = (u32)(yend - ysta + 1) * (xend - xsta + 1);
	while (n--) {
		BSP_LCD_WR_DATA (BSP_POINT_COLOR);
	}//显示所填充的颜色. 
	BSP_LCD_WriteReg (0x50, 0x0000); //Ë®Æ½·½ÏòGRAMÆðÊ¼µØÖ·
	BSP_LCD_WriteReg (0x51, 0x00EF); //Ë®Æ½·½ÏòGRAM½áÊøµØÖ·
	BSP_LCD_WriteReg (0x52, 0x0000); //´¹Ö±·½ÏòGRAMÆðÊ¼µØÖ·
	BSP_LCD_WriteReg (0x53, 0x013F); //´¹Ö±·½ÏòGRAM½áÊøµØÖ·	    
}

