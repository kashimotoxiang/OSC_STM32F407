#include "LCD_MD050SD_Touch.h"
#include "mxconstants.h"
#define ESP_delay_ns_ 8

#define ESP_READ_TIMES 6 //????
#define ESP_LOST_VAL 1	  //???

u16 ESP_vx = 4421, ESP_vy = 6078;
u16 ESP_chx = 247, ESP_chy = 3388;
T_Data_Struct ESP_tp_pixad, ESP_TS; //???????AD?,?????????

//------------------------------------------------------------------------------------------//
void ESP_TS_Init (void) //SPI??
{
	ESP_T_CS_High ;
	ESP_T_CLK_High ;
	ESP_T_DIN_High ;
	ESP_T_CLK_High ;
}

//------------------------------------------------------------------------------------------//
void ESP_WriteCharTo7843 (unsigned char num) //SPI???
{
	static unsigned char count = 0;
	count = 0;
	ESP_T_CLK_Low ;
	for (count = 0; count < 8; count++) {
		if (num & 0x80)
		ESP_T_DIN_High ;
		else
		ESP_T_DIN_Low ;
		num <<= 1;
		ESP_T_CLK_Low ;
		delay_25ns(ESP_delay_ns_); //?????
		ESP_T_CLK_High ;
		delay_25ns(ESP_delay_ns_);
	}
}

//------------------------------------------------------------------------------------------//
u16 ESP_ReadFromCharFrom7843 (void) //SPI ???
{
	static u8 count = 0;
	static u16 Num = 0;
	count = 0;
	Num = 0;
	for (count = 0; count < 12; count++) {
		Num <<= 1;
		ESP_T_CLK_High ;
		delay_25ns(ESP_delay_ns_); //?????
		ESP_T_CLK_Low ;
		delay_25ns(ESP_delay_ns_);
		if (((ESP_T_DOUT_GPIO_Port->IDR) & ESP_T_DOUT_Pin)) {
			Num |= 1;
		}
	}

	return (Num);
}

//------------------------------------------------------------------------------------------//
//?7846/7843/XPT2046/UH7843/UH7846??adc?	  0x90=y_cur   0xd0-x_cur
u16 ESP_ADS_Read_AD (unsigned char CMD) {
	static u16 l;
	ESP_T_CS_Low ;
	ESP_WriteCharTo7843(CMD); //???????????x_cur?? ????????
	ESP_T_CLK_High ;
	delay_25ns(ESP_delay_ns_);
	ESP_T_CLK_Low ;
	delay_25ns(ESP_delay_ns_);
	l = ESP_ReadFromCharFrom7843();
	return l;
}

//------------------------------------------------------------------------------------------//
//???????
//????ESP_READ_TIMES???,?????????,
//?????????ESP_LOST_VAL??,????
u16 ESP_ADS_Read_XY (u8 xy) {
	static u16 i, j;
	static u16 buf[ESP_READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	sum = 0;
	for (i = 0; i < ESP_READ_TIMES; i++) {
		buf[i] = ESP_ADS_Read_AD(xy);
	}
	for (i = 0; i < ESP_READ_TIMES - 1; i++) //??
	{
		for (j = i + 1; j < ESP_READ_TIMES; j++) {
			if (buf[i] > buf[j])//????
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = ESP_LOST_VAL; i < ESP_READ_TIMES - ESP_LOST_VAL; i++)sum += buf[i];
	temp = sum / (ESP_READ_TIMES - 2 * ESP_LOST_VAL);
	return temp;
}

//------------------------------------------------------------------------------------------//
//????????
//???????100.
u8 ESP_Read_ADS (u16* x_cur, u16* y_cur) {
	static u16 xtemp, ytemp;
	xtemp = ESP_ADS_Read_XY(ESP_T_CMD_RDX);
	ytemp = ESP_ADS_Read_XY(ESP_T_CMD_RDY);
	if (xtemp < 100 || ytemp < 100)return 0;//????
	*x_cur = xtemp;
	*y_cur = ytemp;
	return 1;//????
}

//------------------------------------------------------------------------------------------//
//2???ADS7846,????2????AD?,???????????
//50,????,???????,??????.
//???????????
#define ERR_RANGE 20 //????
u8 ESP_Read_ADS2 (u16* x_cur, u16* y_cur) {
	static u16 x1, y1;
	static u16 x2, y2;
	static u8 flag;
	flag = ESP_Read_ADS(&x1, &y1);
	if (flag == 0)return (0);
	flag = ESP_Read_ADS(&x2, &y2);
	if (flag == 0)return (0);
	if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE))//???????+-ERR_RANGE?
		&& ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
		*x_cur = (x1 + x2) >> 1;
		*y_cur = (y1 + y2) >> 1;
		return 1;
	}
	else return 0;
}

//------------------------------------------------------------------------------------------//
//?LCD???????
//??????
//??????
void ESP_Drow_Touch_Point (u16 x_cur, u16 y_cur) {
	ESP_LCD_DrawLine(x_cur - 12, y_cur, x_cur + 13, y_cur);//??
	ESP_LCD_DrawLine(x_cur, y_cur - 12, x_cur, y_cur + 13);//??
	ESP_LCD_DrawPoint(x_cur + 1, y_cur + 1);
	ESP_LCD_DrawPoint(x_cur - 1, y_cur + 1);
	ESP_LCD_DrawPoint(x_cur + 1, y_cur - 1);
	ESP_LCD_DrawPoint(x_cur - 1, y_cur - 1);
	//	Draw_Circle(x_cur,y_cur,6);//????
}


/*------------------------触摸屏配置-------------------------------*/
u16 ESP_TP_Read_XOY (u8 xy) {
	static u16 i, j;
	static u16 buf[ESP_READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	for (i = 0; i < ESP_READ_TIMES; i++)buf[i] = ESP_ADS_Read_XY(xy);
	for (i = 0; i < ESP_READ_TIMES - 1; i++) {
		for (j = i + 1; j < ESP_READ_TIMES; j++) {
			if (buf[i] > buf[j]) {
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = ESP_LOST_VAL; i < ESP_READ_TIMES - ESP_LOST_VAL; i++)sum += buf[i];
	temp = sum / (ESP_READ_TIMES - 2 * ESP_LOST_VAL);
	return temp;
}


/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:返回0触摸失败，返回1触摸成功
*/
u8 ESP_Convert_Pos (void) {
	static u8 state;
	state = 0;
	//if (!ESP_T_IRQ_Dect) {//检测硬件触摸
	if (ESP_Read_ADS2(&ESP_tp_pixad.xc, &ESP_tp_pixad.yc)) {//判断触摸数据是否正常
		state = 1;
		ESP_TS.xc = ESP_tp_pixad.xc > ESP_chx ? ((u32)ESP_tp_pixad.xc - (u32)ESP_chx) * 1000 / ESP_vx : ((u32)ESP_chx - (u32)ESP_tp_pixad.xc) * 1000 / ESP_vx;
		ESP_TS.yc = ESP_tp_pixad.yc > ESP_chy ? ((u32)ESP_tp_pixad.yc - (u32)ESP_chy) * 1000 / ESP_vy : ((u32)ESP_chy - (u32)ESP_tp_pixad.yc) * 1000 / ESP_vy;
		return state;
	}
	//}
	ESP_TS.xc = 0;
	ESP_TS.yc = 0;
	return state;
}

/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:单点触摸时touchDetected为0 1
*/
void ESP_Pointer_Update (void) {
	static GUI_PID_STATE TS_State = {0, 0, 0, eLAYER_ESP};
	static uint16_t xDiff, yDiff;//最终装载的点
	/*-------------------------------------------------------*/
	ESP_TS.TouchDetected = ESP_Convert_Pos();//判断是否有触摸//获取当前触摸数据 tp.xc & tp.yc

	/*不在屏幕范围内-------------------------------------------------------*/
	if ((ESP_TS.xc >= LCD_GetXSize()) || (ESP_TS.yc >= LCD_GetYSize())) {
		ESP_TS.xc = 0;
		ESP_TS.yc = 0;
		ESP_TS.TouchDetected = 0;
	}

	/*触摸滤波防抖-------------------------------------------------------*/
	xDiff = (TS_State.x > ESP_TS.xc) ? (TS_State.x - ESP_TS.xc) : (ESP_TS.xc - TS_State.x);
	yDiff = (TS_State.y > ESP_TS.yc) ? (TS_State.y - ESP_TS.yc) : (ESP_TS.yc - TS_State.y);

	/*装载触摸数据-------------------------------------------------------*/
	if ((TS_State.Pressed != ESP_TS.TouchDetected) || (xDiff > 30) || (yDiff > 30)) {
		TS_State.Pressed = ESP_TS.TouchDetected;//按下与没有按下
		if (ESP_TS.TouchDetected) {
			TS_State.x = ESP_TS.xc;
			TS_State.y = ESP_TS.yc;
			GUI_TOUCH_StoreStateEx(&TS_State);
		}
		else {
			GUI_TOUCH_StoreStateEx(&TS_State);
			TS_State.x = 0;
			TS_State.y = 0;
		}

	}
}

