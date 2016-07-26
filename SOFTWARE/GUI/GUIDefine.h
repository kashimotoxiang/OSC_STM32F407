#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"

/*********************************************************************
*
*       Struct
*
**********************************************************************
*/
/*单个按键-------------------------------------------------------*/
typedef struct {
	int ID;
	WM_HWIN Handle;
} BUTTON_struct;

/*按键信息-------------------------------------------------------*/
typedef struct {
	BUTTON_struct ZoomPlus;
	BUTTON_struct ZoomSub;
	BUTTON_struct AmpliPlus;
	BUTTON_struct AmpliSub;
	BUTTON_struct Measure;
	BUTTON_struct NumPad;
	BUTTON_struct Stop;
	BUTTON_struct InputMode;
} MAINBUTTON_struct;

/*主要显示句柄-------------------------------------------------------*/
typedef struct {
	WM_HWIN ESP_GraphDlg;//Graph界面
	WM_HWIN BSP_NumpadDlg;//键盘界面
	WM_HWIN BSP_ConStDlg;//控制台界面
	WM_HWIN BSP_MersureDlg;//测试数据界面
	WM_HWIN hItemGraph_OSC;//Graph小工具句柄
} Disp_struct;

/*滑条句柄-------------------------------------------------------*/
typedef struct {
	SLIDER_Handle Handle;
	int Value;
	int Class;
} Slider_struct;

/*数字键盘信息-------------------------------------------------------*/
typedef struct {
	WM_HWIN EdirorHandle;
	uint8_t InputEnterFlag;
	int i_InputValue;
	double f_InputValue;
} Numpad_struct;


/*控制台句柄-------------------------------------------------------*/
typedef struct {
	uint8_t InputMode;
	Slider_struct SliderTime;
	Slider_struct SliderAmpli;
	Numpad_struct Numpad;
} UserInput_struct;


/*资源表-------------------------------------------------------*/
typedef struct ResourceMap_struct {
	int Num;
	WM_HWIN RM[30];
} ResourceMap_struct;


#include "main.h"
#include "GUIUsual.h"
#include "GUIGraph.h"
#include "GUIDraw.h"
#include "GUINumPad.h"
#include "GUIMeasureData.h"


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define SCALE_V_Pos 35//坐标位置
#define SCALE_H_Pos 155

