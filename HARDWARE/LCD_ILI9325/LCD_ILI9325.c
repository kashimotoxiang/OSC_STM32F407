#include "LCD_ILI9325.h"

//LCD的画笔颜色和背景色	   
u16 BSP_POINT_COLOR = 0x0000; //画笔颜色
u16 BSP_BACK_COLOR = 0xFFFF; //背景色 

//初始化lcd

void BSP_LCD_Init (void) {
	BSP_LCD_WriteReg (0x00E5, 0x78F0);
	BSP_LCD_WriteReg (0x0001, 0x0100);
	BSP_LCD_WriteReg (0x0002, 0x0700);
	BSP_LCD_WriteReg (0x0003, 0x1030);
	BSP_LCD_WriteReg (0x0004, 0x0000);
	BSP_LCD_WriteReg (0x0008, 0x0202);
	BSP_LCD_WriteReg (0x0009, 0x0000);
	BSP_LCD_WriteReg (0x000A, 0x0000);
	BSP_LCD_WriteReg (0x000C, 0x0000);
	BSP_LCD_WriteReg (0x000D, 0x0000);
	BSP_LCD_WriteReg (0x000F, 0x0000);
	//power on sequence VGHVGL
	BSP_LCD_WriteReg (0x0010, 0x0000);
	BSP_LCD_WriteReg (0x0011, 0x0007);
	BSP_LCD_WriteReg (0x0012, 0x0000);
	BSP_LCD_WriteReg (0x0013, 0x0000);
	BSP_LCD_WriteReg (0x0007, 0x0000);
	//vgh 
	BSP_LCD_WriteReg (0x0010, 0x1690);
	BSP_LCD_WriteReg (0x0011, 0x0227);
	//delayms(100);
	//vregiout 
	BSP_LCD_WriteReg (0x0012, 0x009D); //0x001b
	//delayms(100); 
	//vom amplitude
	BSP_LCD_WriteReg (0x0013, 0x1900);
	//delayms(100); 
	//vom H
	BSP_LCD_WriteReg (0x0029, 0x0025);
	BSP_LCD_WriteReg (0x002B, 0x000D);
	//gamma
	BSP_LCD_WriteReg (0x0030, 0x0007);
	BSP_LCD_WriteReg (0x0031, 0x0303);
	BSP_LCD_WriteReg (0x0032, 0x0003);// 0006
	BSP_LCD_WriteReg (0x0035, 0x0206);
	BSP_LCD_WriteReg (0x0036, 0x0008);
	BSP_LCD_WriteReg (0x0037, 0x0406);
	BSP_LCD_WriteReg (0x0038, 0x0304);//0200
	BSP_LCD_WriteReg (0x0039, 0x0007);
	BSP_LCD_WriteReg (0x003C, 0x0602);// 0504
	BSP_LCD_WriteReg (0x003D, 0x0008);
	//ram
	BSP_LCD_WriteReg (0x0050, 0x0000);
	BSP_LCD_WriteReg (0x0051, 0x00EF);
	BSP_LCD_WriteReg (0x0052, 0x0000);
	BSP_LCD_WriteReg (0x0053, 0x013F);
	BSP_LCD_WriteReg (0x0060, 0xA700);
	BSP_LCD_WriteReg (0x0061, 0x0001);
	BSP_LCD_WriteReg (0x006A, 0x0000);
	//
	BSP_LCD_WriteReg (0x0080, 0x0000);
	BSP_LCD_WriteReg (0x0081, 0x0000);
	BSP_LCD_WriteReg (0x0082, 0x0000);
	BSP_LCD_WriteReg (0x0083, 0x0000);
	BSP_LCD_WriteReg (0x0084, 0x0000);
	BSP_LCD_WriteReg (0x0085, 0x0000);
	//
	BSP_LCD_WriteReg (0x0090, 0x0010);
	BSP_LCD_WriteReg (0x0092, 0x0600);

	BSP_LCD_WriteReg (0x0007, 0x0133);
	BSP_LCD_WriteReg (0x00, 0x0022);//
	BSP_LCD_Scan_Dir(); //默认扫描方向 

	BSP_LCD_LED_Open ; //点亮背光

}

//LCD开启显示
/*****************************************************************************
** 函数名称:LCD_DisplayOn
** 功能描述: 开启LCD显示
** 功能描述: 关闭LCD显示
*****************************************************************************/

void BSP_LCD_Display (u8 off_on) {
	if (off_on == 1) {
		BSP_LCD_WriteReg (0x07, 0x0173);

	}
	else {
		BSP_LCD_WriteReg (0x07, 0x0)
	}
}

//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
inline void BSP_LCD_SetCursor (u16 Xpos, u16 Ypos) {
	if (
		Horizontal_or_Vertical) {
		//横屏显示
		BSP_LCD_WriteReg (0x20, Ypos);
		BSP_LCD_WriteReg (0x21, 319 - Xpos);
	}
	//竖屏显示					   
	else {
		BSP_LCD_WriteReg (0x20, Xpos);
		BSP_LCD_WriteReg (0x21, Ypos);
	}


}

//设置LCD的自动扫描方向

void BSP_LCD_Scan_Dir (void) {
	u16 regval = 0;
	regval |= L2R_D2U; //从左到右,从上到下
	regval |= 1 << 12;
	BSP_LCD_WriteReg (0X03, regval);
}


//清屏函数
//Color:要清屏的填充色
void BSP_LCD_Clear (u16 Color) {
	u32 index = 0;
	BSP_LCD_SetCursor(0x00, 0x0000); //设置光标位置 
	BSP_LCD_WriteRAM_Prepare (); //开始写入GRAM	 	  
	for (index = 0; index < 76800; index++) {
		BSP_LCD_WR_DATA (Color);
	}
}

inline void BSP_Address_Set (u16 x1, u16 y1, u16 x2, u16 y2) {
	BSP_LCD_WriteReg (0x50, x1); //水平方向GRAM起始地址
	BSP_LCD_WriteReg (0x51, x2); //水平方向GRAM结束地址
	BSP_LCD_WriteReg (0x52, y1); //垂直方向GRAM起始地址
	BSP_LCD_WriteReg (0x53, y2); //垂直方向GRAM结束地址	
	BSP_LCD_SetCursor(x1, y1);//设置光标位置  
}

/*===================================================================*/
//读取个某点的颜色值	 
//x,y:坐标
//返回值:此点的颜色
inline u16 BSP_LCD_ReadPoint (u16 x, u16 y) {
	u16 r = 0, g = 0, b = 0;
	BSP_LCD_SetCursor(x, y);
	BSP_LCD_WR_REG(R34); //其他IC发送读GRAM指令
	if (BSP_LCD_RD_DATA())
		r = 0; //dummy Read	   
	r = BSP_LCD_RD_DATA(); //实际坐标颜色
	return r;//这几种IC直接返回颜色值
}

