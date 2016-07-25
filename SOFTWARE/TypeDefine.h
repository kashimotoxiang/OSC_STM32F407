#pragma once
#include <core_cm4.h>

/*=======================类型定义================================*/
typedef volatile unsigned char __uIO8;
typedef volatile unsigned short int __uIO16;
typedef volatile unsigned int __uIO32;

typedef volatile signed char __IO8;
typedef volatile signed short int __IO16;
typedef volatile signed int __IO32;

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef unsigned short int u32_t;

typedef u8_t u8;
typedef u16_t u16;
typedef u32_t u32;

typedef __IO
unsigned int vu32;
typedef __IO
unsigned short int vu16;
typedef __IO
unsigned char vu8;

#define   WAVE_TYPE             __uIO8
#define   TFT_DISPLAY_TYPE      __uIO16
#define   SPI_TYPE              uint8_t 
#define   UART_TYPE				char
#define   WAVE_TYPE				I16

/*=========================状态定义==============================*/
//注意初始状态值一定要大于1！！！！！！！！！！！！！！！！

/*-----------------------主状态--------------------------------*/
typedef enum {
	eFSM_MainS_Emp = 0x01,//初始状态大于1
	eFSM_MainS_OSC,
} FSM_MainS_;//主状态

/*-----------------------工作状态--------------------------------*/
typedef enum {
	eFSM_WorkS_Emp = 0x01,//初始状态大于1
	eFSM_WorkS_TriADC,
} FSM_WorkS_;//工作状态

/*-----------------------触发模式--------------------------------*/
typedef enum {
	eTrg_Mod_Begin = 0x01,//初始状态大于1
	eTrg_Mod_Rising = eTrg_Mod_Begin,
	eTrg_Mod_Falling,
	eTrg_Mod_Centr,
	eTrg_Mod_End,
} Trg_Mod_;//触发模式

/*-----------------------采样方式--------------------------------*/
typedef enum {
	eSampl_Mod_DMA = 0x01,//初始状态大于1
	eSampl_Mod_Timer,
	eSampl_Mod_Equal,
} Sampl_Mod_;//采样方式

/*------------------------页数-------------------------------*/
typedef enum {
	ePage_Num_Begin = 0x01,//初始状态大于1
	ePage_Num_1 = ePage_Num_Begin,
	ePage_Num_2,
	ePage_Num_3,
	ePage_Num_4,
	ePage_Num_5,
	ePage_Num_6,
	ePAGE_End = ePage_Num_6,
} Page_Num_;//页数

/*-------------------------分辨率------------------------------*/
typedef enum {
	eReslt_rat_Begin = 0x01,//初始状态大于1
	eReslt_rat_0 = eReslt_rat_Begin,
	eReslt_rat_1,
	eReslt_rat_2,
	eReslt_rat_3,
	eReslt_rat_4,
	eReslt_rat_5,
	eReslt_rat_6,
	eReslt_rat_7,
	eReslt_rat_8,
	eReslt_rat_9,
	eReslt_rat_10,
	eReslt_rat_11,
	eReslt_rat_End = eReslt_rat_11,
} Reslt_rat_;//分辨率

/*---------------------触摸屏按键状态----------------------------------*/
typedef enum {
	//初始状态大于1
	eKey_Empty = 0x01,
	eSampl_rat_ADD,
	eReslt_rat_ADD,
	eReslt_rat_SUB,
	eTRGPosLine_ADD,
	eTRGPosLine_SUB,
	eTRG_Mod_ADD,
	eTRG_Mod_SUB,
} Key_State_;//触摸屏按键状态

/*------------------------u16边界检查-------------------------------*/
typedef enum {
	eu16_BEGIN = 0,
	eu16_END = 65535,
} u16_BEGIN_;//

/*------------------------开关-------------------------------*/
typedef enum {
	eClose = 0,
	eOpen = 1,
} Switch_;//

/*------------------------LCD参数设置-------------------------------*/
typedef enum {
	eLCDPar_width = 800, //LCD 宽度
	eLCDPar_height = 480, //LCD 高度
	eLCDPar_dir = 1, //横屏还是竖屏控制：0，竖屏；1，横屏。	
	eLCDPar_wramcmd = 0x0F, //开始写gram指令
	eLCDPar_setxcmd = 0x02, //设置x坐标指令
	eLCDPar_setycmd = 0x03, //设置y坐标指令 
} LCD_Par_;

/*------------------------LCD设备-------------------------------*/
typedef enum {
	eLAYER_BSP = 0,
	eLAYER_ESP = 1,
} LAYER_Par_;

/*------------------------GUI控制字-------------------------------*/
typedef enum {
	eNumpadInput = 0,
} GUI_CONTROL_;//

/*------------------------SPI控制字-------------------------------*/
typedef enum {
	eS_SendFreq = 0x0000,
	eS_SendData = 0xFFFF,
} SPI_CONTROL_;//

/*-------------------------显示区域竞争判断------------------------------*/
typedef enum {
	eISEMPTY = 0,
	eISNOTEMPRT,
} IsDispEmpty_;//显示区域是否为空，避免显示冲突


