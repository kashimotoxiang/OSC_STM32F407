#include "GUIDraw.h"
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
UserInput_struct g_UserInput = {{0}};
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
static GUI_POINT WaveArray[DATAARRAYLENTGH ] = {0};//波型数组

static Index_struct index;


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
static void _ChangeInfoText (char* pStr, int m_MsgId);
static void ResizeWindow (void);

/*******************************************************************
*
*       _DemoResizeWindow
*
* Function description
*   Demonstrates the use of WM_ResizeWindow
*/
static void ResizeWindow (void) {
	int i;
	int tm;
	int tDiff;

	_ChangeInfoText("WM_ResizeWindow()", WM_PAINT);
	GUI_Delay(SPEED);
	for (i = 0; i < 20; i++) {
		tm = GUI_GetTime();
		//WM_ResizeWindow (_hWindow1, 1, 1);
		//WM_ResizeWindow(_hWindow2, -1, -1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	for (i = 0; i < 40; i++) {
		tm = GUI_GetTime();
		//WM_ResizeWindow (_hWindow1, -1, -1);
		//WM_ResizeWindow (_hWindow2, 1, 1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	for (i = 0; i < 20; i++) {
		tm = GUI_GetTime();
		//WM_ResizeWindow(_hWindow1, 1, 1);
		//WM_ResizeWindow(_hWindow2, -1, -1);
		tDiff = 15 - (GUI_GetTime() - tm);
		GUI_Delay(tDiff);
	}
	GUI_Delay(SPEED);
}

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
	ESP_SwapPage(&g_DispPage);//交换至第二块页面
	WM_PaintWindowAndDescs(g_Disp.ESP_GraphDlg);//重绘界面于第二块显示器同时重绘
	/*-------------------------------------------------------*/
	_hData = GRAPH_DATA_XY_Create(_aColor, DATAARRAYLENTGH, WaveArray, DATAARRAYLENTGH);
	GRAPH_AttachData(g_Disp.hItemGraph_OSC, _hData);//重绘这次的点
	/*获取按键句柄-------------------------------------------------------*/
	g_GraphButton.ZoomPlus.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.ZoomPlus.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.ZoomSub.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.ZoomSub.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Measure.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Measure.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.NumPad.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.NumPad.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Stop.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Stop.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.ConSt.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.ConSt.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Reserve2.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Reserve2.ID);//获取按键句柄//必须放在这里！！！！
	g_GraphButton.Reserve3.Handle = WM_GetDialogItem(g_Disp.ESP_GraphDlg, g_GraphButton.Reserve3.ID);//获取按键句柄//必须放在这里！！！！
	/*-------------------------------------------------------*/
	GUI_Exec();//重绘
	/*-------------------------------------------------------*/
#ifdef DEBUG__
#endif
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
	g_Disp.BSP_ConStDlg = ControlStation_CreateWindow();
	ConStSwitch(eClose);//先关闭控制台
	RMSwitch(eClose, NumPad_RMs);//关闭屏幕键盘
	GUI_Exec();//重绘
}

/*********************************************************************
*
*       GUIDataUpdata
*/
void GUIDataUpdata (void) {
	static int j;//下标
	/*控制数据更新-------------------------------------------------------*/
	SliderClassUpdata(&g_UserInput.SliderTime);//时间梯度化处理
	g_OSCInfo.Time_rat = g_UserInput.SliderTime.Class;
	g_OSCInfo.Ampli_rat = g_UserInput.SliderAmpli.Value;

	/*OSC数据处理-------------------------------------------------------*/
	if (!OSC_DataDeal(WaveArray, DATAARRAYLENTGH))//数据处理
		return;
	/*显示-------------------------------------------------------*/
	GRAPH_DATA_XY_Clear(_hData);
	for (j = 0; j < g_OSCInfo.DataEnd; j++) {
		GRAPH_DATA_XY_AddPoint(_hData, &WaveArray[j]);
	}
	/*无效化按键使之重绘-------------------------------------------------------*/
	WM_InvalidateWindow(g_GraphButton.ZoomPlus.Handle);
	WM_InvalidateWindow(g_GraphButton.ZoomSub.Handle);
	WM_InvalidateWindow(g_GraphButton.Measure.Handle);
	WM_InvalidateWindow(g_GraphButton.NumPad.Handle);
	WM_InvalidateWindow(g_GraphButton.Stop.Handle);
	WM_InvalidateWindow(g_GraphButton.ConSt.Handle);
	WM_InvalidateWindow(g_GraphButton.Reserve2.Handle);
	WM_InvalidateWindow(g_GraphButton.Reserve3.Handle);
}

/*********************************************************************
*
*       MeasureInfoChange
*/
void OSC_MeasureInfoSwitch (u8 state) {
	static int FontSizeY;//字体高度
	GUI_SetFont(&GUI_Font20B_ASCII);//设置字体
	FontSizeY = GUI_GetFontSizeY();//返回字体高度
	if (state == eOpen) {
		GUI_SetColor(GUI_RED);
		GUI_DispStringAt("Freq:", MEASURE_INFO_BEGIN_X, MEASURE_INFO_BEGIN_Y);
		GUI_DispFloat(g_OSCInfo.Freq, 4);
		GUI_DispStringAt("Ampl:", MEASURE_INFO_BEGIN_X, MEASURE_INFO_BEGIN_Y + FontSizeY);
		GUI_DispFloat(g_OSCInfo.f_MaxVal, 4);
	}
	else {
		GUI_GotoXY(MEASURE_INFO_BEGIN_X, MEASURE_INFO_BEGIN_Y);
		GUI_DispCEOL();
		GUI_GotoXY(MEASURE_INFO_BEGIN_X, MEASURE_INFO_BEGIN_Y + FontSizeY);
		GUI_DispCEOL();
	}
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
*       关闭BSP所有窗口
*/
void CloseAllBSPDLG (void) {
	if (g_GUICon.NumpadState == eOpen)
		RMSwitch(eClose, NumPad_RMs);
	if (g_GUICon.ConStState == eOpen)
		ConStSwitch(eClose);
}

