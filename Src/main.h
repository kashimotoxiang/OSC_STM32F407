#pragma once
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "stdint.h"
#include "mxconstants.h"
#include "GUI.h"
#include "core_cm4.h"
#include "TypeDefine.h"
/* Private define ------------------------------------------------------------*/
#define  TFT_WIDTH           640
#define  TFT_BEGINNING   MEMORYE_DEPTH/4
#define  MEMORYE_DEPTH       2048
#define  Zone_Size           0.1
#define  TRIGGER_TOLRATE_EDGE             50
#define  TRIGGER_WIDTH_TOLRATE       10
#define  TRIGGER_CENTRE_TOLRATE    1
#define  SAMPLING_SPEED       4
#define  MAXMIN_ARRAY_SIZE        20
#define  FOUND_SIZE 300
#define  VOLTCONVCOFFI         3300/255.0     //电压转换系数
#define  VOLTDISPCONVCOFI         VOLTCONVCOFFI/10/3000.0     //电压显示转换系数//AMLPLIFY上界为5，为实现伪连续，设为10000，需要除以2000还原上界
/*-----------------------是否进入调试模式--------------------------------*/

//#define DEBUG__				 //功能查看模式
//#define MEM_DEBUG__			 //内存调试模式
//#define SINGLE_PAGE__		 //使用单页缓存
//#define ADCTAKESAMPLE__		 //ADC采样模式填充数组
#define SPIDATATRANS__		 //SPI数据传输模式

/*----------------------功能裁剪---------------------------------*/

#define TRIBLE_ADC 0
#define QUICK_SORT 0

/*-------------------------------------------------------*/


/*=========================结构体定义==============================*/

/* ADC ----------------------------------------------------------*/
typedef struct ADC_Conv_struct {
	WAVE_TYPE Data[MEMORYE_DEPTH];
	uint16_t FFT_Data[MEMORYE_DEPTH / 2];
} ADC_Conv_struct;

/* FMS ---------------------------------------------------------*/
typedef struct FSM_struct//FMS
{
	__uIO8 WorkS;
	__uIO8 Old_WorkS;
	__uIO8 MainS;
	__uIO8 Old_MainS;
} FSM_struct;

/* OSC ---------------------------------------------------------*/
typedef struct OSC_struct//OSC
{
	__uIO8 Trig_Mod;
	__uIO8 Sampl_Mod;
	double Time_rat;
	double Ampli_rat;
	__uIO32 TRG_Pos;
	__uIO16 i_MaxVal;
	__uIO16 i_MinVal;
	float f_MaxVal;
	float f_MinVal;
	__uIO32 Freq;
	__uIO32 DataBegin;
	__uIO32 DataEnd;
	__uIO8 ClyDotNum;
	u8 MeasureStatu;
} OSC_struct;

/* GUIControl ---------------------------------------------------------*/
typedef struct GUIControl_struct//OSC
{
	__uIO8 NumpadState;
	__uIO8 GraphDispState;
	__uIO8 ConStState;
	__uIO8 BSP_DispState;//BSP显示竞争
} GUIControl_struct;

/* LCD---------------------------------------------------------*/
typedef struct Page_struct//LCD
{
	__uIO8 One;//显示页
	__uIO8 Two;//操作页
	__uIO8 Temp;//交换页码用
} Page_struct;

/* Key---------------------------------------------------------*/
typedef struct Key_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Key_struct;

/* Key---------------------------------------------------------*/
typedef struct Index_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Index_struct;

/*传感器数据-------------------------------------------------------*/
typedef struct SenseData_struct//Key
{
	__uIO32 ADS7883;
	__uIO32 ADS1110;
	__uIO32 ADF4002;
	__uIO32 AD9852;
} SenseData_struct;

/*=======================================================*/
extern ADC_Conv_struct g_SamplData;
extern FSM_struct g_FSM;
extern OSC_struct g_OSCInfo;
extern Page_struct g_DispPage;
extern Key_struct g_Key;
extern GUIControl_struct g_GUICon;
extern SenseData_struct g_Sense;//传感器数据

extern void Error_Handler (void);
/* Includes ------------------------------------------------------------------*/

/*硬件-------------------------------------------------------*/
#include "System.h"
#include "mySPI.h"
#include "myADC.h"
#include "myKey.h"
#include "LCD_MD050SD.h"
/*软件-------------------------------------------------------*/
#include "Usual.h"
#include "FSM.h"
#include "OSC_Work.h"
#include "FFT.h"
/*emWin-------------------------------------------------------*/
#include "GUIDefine.h"
/*-------------------------------------------------------*/
#ifdef DEBUG__
#include "DebugSupport.h"
#endif

#ifdef MEM_DEBUG__
#include "GUIUsual.h"
#endif

/*assert诊断-------------------------------------------------------*/
#if MEMORYE_DEPTH-TFT_WIDTH<0
#pragma message("采样深度过小！！")  
#error
#endif 
/*-------------------------------------------------------*/


