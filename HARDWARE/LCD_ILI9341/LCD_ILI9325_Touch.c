#include "LCD_ILI9325_Touch.h"
#include "mxconstants.h"
#define BSP_delay_ns_ 8

#define BSP_READ_TIMES 6 //????
#define BSP_LOST_VAL 1	  //???

u16 BSP_vx = 4421, BSP_vy = 6078;
u16 BSP_chx = 247, BSP_chy = 3388;
T_Data_Struct BSP_tp_pixad, BSP_TS; //???????AD?,?????????
//------------------------------------------------------------------------------------------//
inline u8 BSP_tpstate (void) {
	return BSP_T_IRQ_Dect;
}

//------------------------------------------------------------------------------------------//
void BSP_TS_Init (void) //SPI??
{
	BSP_T_CS_High ;
	BSP_T_CLK_High ;
	BSP_T_DIN_High ;
	BSP_T_CLK_High ;
}

//------------------------------------------------------------------------------------------//
void BSP_WriteCharTo7843 (unsigned char num) //SPI???
{
	static unsigned char count = 0;
	count = 0;
	BSP_T_CLK_Low ;
	for (count = 0; count < 8; count++) {
		if (num & 0x80)
		BSP_T_DIN_High ;
		else
		BSP_T_DIN_Low ;
		num <<= 1;
		BSP_T_CLK_Low ;
		delay_25ns(BSP_delay_ns_); //?????
		BSP_T_CLK_High ;
		delay_25ns(BSP_delay_ns_);
	}
}

//------------------------------------------------------------------------------------------//
u16 BSP_ReadFromCharFrom7843 (void) //SPI ???
{
	static u8 count = 0;
	static u16 Num = 0;
	count = 0;
	Num = 0;
	for (count = 0; count < 12; count++) {
		Num <<= 1;
		BSP_T_CLK_High ;
		delay_25ns(BSP_delay_ns_); //?????
		BSP_T_CLK_Low ;
		delay_25ns(BSP_delay_ns_);
		if (((BSP_T_DOUT_GPIO_Port->IDR) & BSP_T_DOUT_Pin)) {
			Num |= 1;
		}
	}

	return (Num);
}

//------------------------------------------------------------------------------------------//
//?7846/7843/XPT2046/UH7843/UH7846??adc?	  0x90=y_cur   0xd0-x_cur
u16 BSP_ADS_Read_AD (unsigned char CMD) {
	static u16 l;
	BSP_T_CS_Low ;
	BSP_WriteCharTo7843(CMD); //???????????x_cur?? ????????
	BSP_T_CLK_High ;
	delay_25ns(BSP_delay_ns_);
	BSP_T_CLK_Low ;
	delay_25ns(BSP_delay_ns_);
	l = BSP_ReadFromCharFrom7843();
	return l;
}

//------------------------------------------------------------------------------------------//
//???????
//????BSP_READ_TIMES???,?????????,
//?????????BSP_LOST_VAL??,????
u16 BSP_ADS_Read_XY (u8 xy) {
	static u16 i, j;
	static u16 buf[BSP_READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	sum = 0;
	for (i = 0; i < BSP_READ_TIMES; i++) {
		buf[i] = BSP_ADS_Read_AD(xy);
	}
	for (i = 0; i < BSP_READ_TIMES - 1; i++) //??
	{
		for (j = i + 1; j < BSP_READ_TIMES; j++) {
			if (buf[i] > buf[j])//????
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = BSP_LOST_VAL; i < BSP_READ_TIMES - BSP_LOST_VAL; i++)sum += buf[i];
	temp = sum / (BSP_READ_TIMES - 2 * BSP_LOST_VAL);
	return temp;
}

//------------------------------------------------------------------------------------------//
//????????
//???????100.
u8 BSP_Read_ADS (u16* x_cur, u16* y_cur) {
	static u16 xtemp, ytemp;
	xtemp = BSP_ADS_Read_XY(BSP_T_CMD_RDX);
	ytemp = BSP_ADS_Read_XY(BSP_T_CMD_RDY);
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

u8 BSP_Read_ADS2 (u16* x_cur, u16* y_cur) {
	static u16 x1, y1;
	static u16 x2, y2;
	static u8 flag;
	flag = BSP_Read_ADS(&x1, &y1);
	if (flag == 0)return (0);
	flag = BSP_Read_ADS(&x2, &y2);
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
//????????,???????
u8 BSP_Read_TP_Once (void) {
	static u8 re = 0;
	static u16 x1, y1;
	static u16 x2, y2;
	re = 0;
	while (re == 0) {
		while (!BSP_Read_ADS2(&x1, &y1));
		delay_ms(10);
		while (!BSP_Read_ADS2(&x2, &y2));
		if ((x2 <= x1 && x1 < x2 + 3) || (x1 <= x2 && x2 < x1 + 3)) {
			BSP_tp_pixad.xc = (x1 + x2) / 2;
			BSP_tp_pixad.yc = (y1 + y2) / 2;
			re = 1;
		}
	}
	return re;
}


/*------------------------触摸屏配置（SPI2）-------------------------------*/
u16 BSP_TP_Read_XOY (u8 xy) {
	static u16 i, j;
	static u16 buf[BSP_READ_TIMES ];
	static u16 sum = 0;
	static u16 temp;
	for (i = 0; i < BSP_READ_TIMES; i++)buf[i] = BSP_ADS_Read_XY(xy);
	for (i = 0; i < BSP_READ_TIMES - 1; i++) {
		for (j = i + 1; j < BSP_READ_TIMES; j++) {
			if (buf[i] > buf[j]) {
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = BSP_LOST_VAL; i < BSP_READ_TIMES - BSP_LOST_VAL; i++)sum += buf[i];
	temp = sum / (BSP_READ_TIMES - 2 * BSP_LOST_VAL);
	return temp;
}


/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:返回0触摸失败，返回1触摸成功
*/
u8 BSP_Convert_Pos (void) {
	static u8 state;
	state = 0;
	if (!BSP_T_IRQ_Dect) {//检测硬件触摸
		if (BSP_Read_ADS2(&BSP_tp_pixad.xc, &BSP_tp_pixad.yc)) {//判断触摸数据是否正常
			state = 1;
			BSP_TS.xc = BSP_tp_pixad.xc > BSP_chx ? ((u32)BSP_tp_pixad.xc - (u32)BSP_chx) * 1000 / BSP_vx : ((u32)BSP_chx - (u32)BSP_tp_pixad.xc) * 1000 / BSP_vx;
			BSP_TS.yc = BSP_tp_pixad.yc > BSP_chy ? ((u32)BSP_tp_pixad.yc - (u32)BSP_chy) * 1000 / BSP_vy : ((u32)BSP_chy - (u32)BSP_tp_pixad.yc) * 1000 / BSP_vy;
			return state;
		}
	}
	BSP_TS.xc = 0;
	BSP_TS.yc = 0;
	return state;
}

/*-------------------------------------------------------*/
/**
* @brief:
* @param:
* @note:单点触摸时touchDetected为0 1
*/
void BSP_Pointer_Update (void) {
	static GUI_PID_STATE TS_State = {0, 0, 0, eLAYER_ESP};
	static uint16_t xDiff, yDiff;//最终装载的点
	/*-------------------------------------------------------*/
	BSP_TS.TouchDetected = BSP_Convert_Pos();//判断是否有触摸//获取当前触摸数据 tp.xc & tp.yc

	/*不在屏幕范围内-------------------------------------------------------*/
	if ((BSP_TS.xc >= LCD_GetXSize()) || (BSP_TS.yc >= LCD_GetYSize())) {
		BSP_TS.xc = 0;
		BSP_TS.yc = 0;
		BSP_TS.TouchDetected = 0;
	}

	/*触摸滤波防抖-------------------------------------------------------*/
	xDiff = (TS_State.x > BSP_TS.xc) ? (TS_State.x - BSP_TS.xc) : (BSP_TS.xc - TS_State.x);
	yDiff = (TS_State.y > BSP_TS.yc) ? (TS_State.y - BSP_TS.yc) : (BSP_TS.yc - TS_State.y);

	/*装载触摸数据-------------------------------------------------------*/
	if ((TS_State.Pressed != BSP_TS.TouchDetected) || (xDiff > 30) || (yDiff > 30)) {
		TS_State.Pressed = BSP_TS.TouchDetected;//按下与没有按下
		if (BSP_TS.TouchDetected) {
			TS_State.x = BSP_TS.xc;
			TS_State.y = BSP_TS.yc;
			GUI_TOUCH_StoreStateEx(&TS_State);
		}
		else {
			GUI_TOUCH_StoreStateEx(&TS_State);
			TS_State.x = 0;
			TS_State.y = 0;
		}

	}
}

