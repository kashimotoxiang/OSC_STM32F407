#include "System.h" 
#include "LCD_ILI9325/LCD_ILI9325.h"

//"系统" 常用功能函数集-头文件
extern Disp_struct g_Disp;

/*-------------------------------------------------------*
*
*Display_Initial
*
*-------------------------------------------------------*/
void Display_Initial (void) {
	/*BSP------------------------------------------------------*/
	BSP_LCD_Init();
	BSP_TS_Init();

	/*ESP-------------------------------------------------------*/
	ESP_TS_Init();
	ESP_LCD_Init();

	/* 清屏 ---------------------------------------------------------*/
#ifdef SINGLE_PAGE__
	ESP_ShowPage(ePage_Num_1);
	ESP_WritePage(ePage_Num_1);
	ESP_LCD_Clear(BLACK);
#else
	ESP_SwapPage(&g_DispPage);
	ESP_LCD_Clear(BLACK);
	ESP_SwapPage(&g_DispPage);
	ESP_LCD_Clear(BLACK);
#endif
	/*-------------------------------------------------------*/
	GUI_Init();//GUI初始化
	ESP_MainTask();//界面初始化
	BSP_MainTask();//界面初始化
}

/*-------------------------------------------------------*
*
*BackgroundUpdata
*
*-------------------------------------------------------*/
void BackgroundUpdata (void) {
	//	/*BSP-------------------------------------------------------*/
	//	GUI_SelectLayer(eLAYER_BSP);//选择绘图层
	//	BSP_Pointer_Update();
	//	GUI_Exec();
	/*ESP-------------------------------------------------------*/
	GUI_SelectLayer(eLAYER_ESP);//选择绘图层
	ESP_Pointer_Update();
	GUI_Exec();
	/*-------------------------------------------------------*/
#ifndef SINGLE_PAGE__
	ESP_SwapPage(&g_DispPage);//换页
#endif
}

//*****************************************************************************
//
// delay
//
//*****************************************************************************

/*-------------------------------------------------------*
*
*delay_us
*
*-------------------------------------------------------*/
void delay_us (int nus) {
	u32 temp;
	SysTick->LOAD = 21 * nus;//168/8=21,8个tick一次systick
	SysTick->VAL = 0X00;//清空计数器
	SysTick->CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	do {
		temp = SysTick->CTRL;//读取当前倒计数值
	}
	while ((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL = 0x00; //关闭计数器
	SysTick->VAL = 0X00; //清空计数器
}

/*-------------------------------------------------------*
*
*delay_ms
*
*-------------------------------------------------------*/
void delay_ms (int nms) {
	u32 temp;
	SysTick->LOAD = 21000 * nms;
	SysTick->VAL = 0X00;//清空计数器
	SysTick->CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	do {
		temp = SysTick->CTRL;//读取当前倒计数值
	}
	while ((temp & 0x01) && (!(temp & (1 << 16))));//等待时间到达
	SysTick->CTRL = 0x00; //关闭计数器
	SysTick->VAL = 0X00; //清空计数器
}

/*-------------------------------------------------------*
*
*delay_25ns
*
*-------------------------------------------------------*/
void delay_25ns (int ns) {
	while (ns--);
	return;
}

