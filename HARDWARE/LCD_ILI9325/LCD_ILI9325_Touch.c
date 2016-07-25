#include "LCD_ILI9325_Touch.h"

u8 CMD_RDX = 0XD0;
u8 CMD_RDY = 0X90;

u16 BSP_vx = 4421, BSP_vy = 6078;
u16 BSP_chx = 247, BSP_chy = 3388;
T_Data_Struct BSP_tp_pixad, BSP_TS;
Touch_Adjust_Parameters Touch_date = {
	0.0675447509 ,
	0.0886917934,
	-16,
	-16,
};

/*-------------------------------------------------------*/
void BSP_TS_Init (void) {
	BSP_T_CS_High ;
	BSP_T_CLK_High ;
	BSP_T_DIN_High ;
	BSP_T_CLK_High ;
}

/*-------------------------------------------------------*/
void BSP_TP_Write_Byte (u8 num) {
	u8 count = 0;
	for (count = 0; count < 8; count++) {
		if (num & 0x80)
		BSP_T_DIN_High ;
		else
		BSP_T_DIN_Low ;
		num <<= 1;
		BSP_T_CLK_Low ;
		delay_us(1);
		BSP_T_CLK_High ; //ÉÏÉýÑØÓÐÐ§	        
	}
}

/*-------------------------------------------------------*/
u16 BSP_TP_Read_AD (u8 CMD) {
	u8 count = 0;
	u16 Num = 0;
	BSP_T_CLK_Low ; //ÏÈÀ­µÍÊ±ÖÓ 	 
	BSP_T_DIN_Low ; //À­µÍÊý¾ÝÏß
	BSP_T_CS_Low ; //Ñ¡ÖÐ´¥ÃþÆÁIC
	BSP_TP_Write_Byte(CMD);//·¢ËÍÃüÁî×Ö
	delay_us(6);//ADS7846µÄ×ª»»Ê±¼ä×î³¤Îª6us
	BSP_T_CLK_Low ;
	delay_us(1);
	BSP_T_CLK_High ; //¸ø1¸öÊ±ÖÓ£¬Çå³ýBUSY
	delay_us(1);
	BSP_T_CLK_Low ;
	for (count = 0; count < 16; count++)//¶Á³ö16Î»Êý¾Ý,Ö»ÓÐ¸ß12Î»ÓÐÐ§ 
	{
		Num <<= 1;
		BSP_T_CLK_Low ; //ÏÂ½µÑØÓÐÐ§  	    	   
		delay_us(1);
		BSP_T_CLK_High ;
		if (((BSP_T_DOUT_GPIO_Port->IDR)&BSP_T_DOUT_Pin))
			Num++;
	}
	Num >>= 4; //Ö»ÓÐ¸ß12Î»ÓÐÐ§.
	BSP_T_CS_High ; //ÊÍ·ÅÆ¬Ñ¡	 
	return (Num);
}

/*-------------------------------------------------------*/
#define READ_TIMES 5 	//¶ÁÈ¡´ÎÊý
#define LOST_VAL 1	  	//¶ªÆúÖµ
u16 BSP_TP_Read_XOY (u8 xy) {
	u16 i, j;
	u16 buf[READ_TIMES ];
	u16 sum = 0;
	u16 temp;
	for (i = 0; i < READ_TIMES; i++)buf[i] = BSP_TP_Read_AD(xy);
	for (i = 0; i < READ_TIMES - 1; i++)//ÅÅÐò
	{
		for (j = i + 1; j < READ_TIMES; j++) {
			if (buf[i] > buf[j])//ÉýÐòÅÅÁÐ
			{
				temp = buf[i];
				buf[i] = buf[j];
				buf[j] = temp;
			}
		}
	}
	sum = 0;
	for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)sum += buf[i];
	temp = sum / (READ_TIMES - 2 * LOST_VAL);
	return temp;
}

/*-------------------------------------------------------*/
u8 BSP_TP_Read_XY (u16* x, u16* y) {
	u16 xtemp, ytemp;
	xtemp = BSP_TP_Read_XOY(CMD_RDX);
	ytemp = BSP_TP_Read_XOY(CMD_RDY);
	//if(xtemp<100||ytemp<100)return 0;//¶ÁÊýÊ§°Ü
	*x = xtemp;
	*y = ytemp;
	return 1;//¶ÁÊý³É¹¦
}

/*-------------------------------------------------------*/
#define ERR_RANGE 50 //Îó²î·¶Î§ 
u8 BSP_TP_Read_XY2 (u16* x, u16* y) {
	u16 x1, y1;
	u16 x2, y2;
	u8 flag;
	flag = BSP_TP_Read_XY(&x1, &y1);
	if (flag == 0)return (0);
	flag = BSP_TP_Read_XY(&x2, &y2);
	if (flag == 0)return (0);
	if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE))//Ç°ºóÁ½´Î²ÉÑùÔÚ+-50ÄÚ
		&& ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE))) {
		*x = (x1 + x2) / 2;
		*y = (y1 + y2) / 2;
		return 1;
	}
	else return 0;
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
	if (BSP_TP_Read_XY2(&BSP_tp_pixad.xc, &BSP_tp_pixad.yc)) {//判断触摸数据是否正常
		state = 1;
		BSP_TS.xc = Touch_date.xfac * (BSP_tp_pixad.xc) + Touch_date.xoff;
		BSP_TS.yc = Touch_date.yfac * (BSP_tp_pixad.yc) + Touch_date.yoff;
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
	static GUI_PID_STATE TS_State = {0, 0, 0, eLAYER_BSP};
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

