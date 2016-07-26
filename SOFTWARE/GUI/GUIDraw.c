#include "GUIDraw.h"

#include "GUINumPad.h"
#include "GUIMeasureData.h"

/*********************************************************************
*
*       extern data
*
**********************************************************************
*/
/*********************************************************************
*
*       global data
*
**********************************************************************
*/
Disp_struct g_Disp = {0};
UserInput_struct g_UserInput = {eAD9834DutyInput};
/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static GRAPH_DATA_Handle _hData; // 显存一的数据

static GRAPH_SCALE_Handle g_hScaleV; // Handle of vertical scale
static GRAPH_SCALE_Handle g_hScaleH; // Handle of horizontal scale

static GUI_COLOR _aColor = {GUI_RED}; // Array of colors for the GRAPH_DATA objects
/*-------------------------------------------------------*/
static Index_struct index;


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void _ChangeInfoText (char* pStr, int m_MsgId);

/*******************************************************************
*
*       _ChangeInfoText
*
* Function description
*   Sends a message to the background window and invalidate it, so
*   the callback of the background window display the new text.
*/
static void _ChangeInfoText (char* pStr, int m_MsgId) {
	WM_MESSAGE Message;

	Message.MsgId = m_MsgId;
	Message.Data.p = pStr;
	WM_SendMessage(WM_HBKWIN, &Message);
	WM_InvalidateWindow(WM_HBKWIN);
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

/*********************************************************************
*
*       GUIDraw_Init
*/
void ESP_MainTask (void) {
	GUI_SelectLayer(eLAYER_ESP);//选择绘图层
	/*初始化设置-------------------------------------------------------*/
	WINDOW_SetDefaultBkColor(GUI_DARKGRAY);
	/*皮肤设置-------------------------------------------------------*/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	/*装载界面Dual-------------------------------------------------------*/
	g_Disp.ESP_GraphDlg = Main_CreateWindow();//创建界面
	GUI_Exec();//重绘
#ifndef SINGLE_PAGE__
	ESP_SwapPage(&g_DispPage);//交换至第二块页面
	WM_PaintWindowAndDescs(g_Disp.ESP_GraphDlg);//重绘界面于第二块显示器同时重绘
#endif
	/*填充数据-------------------------------------------------------*/
	_hData = GRAPH_DATA_YT_Create(_aColor, TFT_WIDTH, WaveArray, TFT_WIDTH);
	GRAPH_AttachData(g_Disp.hItemGraph_OSC, _hData);//重绘这次的点
	/*获取按键句柄-------------------------------------------------------*/
	g_GraphButton.ZoomPlus.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.ZoomPlus.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.ZoomSub.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.ZoomSub.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.AmpliPlus.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.AmpliPlus.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.AmpliSub.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.AmpliSub.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Measure.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Measure.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.NumPad.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.NumPad.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Stop.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Stop.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.InputMode.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.InputMode.ID);//获取按键句柄//必须放在这里！！！！
	GUI_Exec();//重绘
}

/*********************************************************************
*
*       BSP_MainTask
*/
void BSP_MainTask (void) {
	GUI_SelectLayer(eLAYER_BSP);//选择绘图层
	/*初始化设置-------------------------------------------------------*/
	WINDOW_SetDefaultBkColor(GUI_WHITE);
	WM_SetDesktopColor(GUI_WHITE);
	/*皮肤设置-------------------------------------------------------*/
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX);
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	/*-------------------------------------------------------*/
	g_Disp.BSP_NumpadDlg = Numpad_CreateWindow();
	g_Disp.BSP_MersureDlg = MeasureData_CreateWindow();
	WindowSwitch(g_Disp.BSP_NumpadDlg, eClose);
	GUI_Exec();//重绘
}


/*********************************************************************
*
*       RM（资源表）开关
*	    关闭整个资源表
*/
void RMSwitch (u8 state, ResourceMap_struct RMs) {
	if (state == eOpen) {
		for (int i = 0; i < RMs.Num; i++) {
			WM_ShowWindow(RMs.RM[i]);
		}
	}
	else {
		for (int i = 0; i < RMs.Num; i++) {
			WM_HideWindow(RMs.RM[i]);
		}
	}
}

/*********************************************************************
*
*       ControlStation键盘开关
*
*/
void WindowSwitch (WM_HWIN hWin,u8 state) {
	if (state == eOpen) {
		WM_ShowWindow(hWin);
	}
	else {
		WM_HideWindow(hWin);
	}
}

/*********************************************************************
*
*       关闭BSP所有窗口
*/
void CloseAllBSPDLG (void) {
	if (g_GUICon.MeasureState == eOpen) {
		WindowSwitch(g_Disp.BSP_MersureDlg, eClose);
		g_GUICon.MeasureState = eClose;
	}
	if (g_GUICon.NumpadState == eOpen) {
		WindowSwitch(g_Disp.BSP_NumpadDlg, eClose);
		g_GUICon.NumpadState = eClose;
	}
}

