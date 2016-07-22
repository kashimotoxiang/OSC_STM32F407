#include "GUIControlStation.h"

/*-------------------------------------------------------*/
//GUI_ID_SLIDER0为TimeSIder GUI_ID_SLIDER1为AmplifySider
/*-------------------------------------------------------*/

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO ControlStation_aDialogCreate[] = {
	{FRAMEWIN_CreateIndirect, "Control Station", 0, 0, 0, 240, 320, 0, 0},
	{SLIDER_CreateIndirect, NULL , GUI_ID_SLIDER0, 11, 57, 208, 21, 0, 0},
	{SLIDER_CreateIndirect, NULL , GUI_ID_SLIDER1, 11, 137, 208, 21, 0, 0},
	{TEXT_CreateIndirect, "Time", GUI_ID_TEXT0, 78, 26, 90, 25, 0, 0},
	{TEXT_CreateIndirect, "Amplitude", GUI_ID_TEXT1, 68, 104, 109, 27, 0, 0}
};


/*****************************************************************
**      FunctionName:void ControlStation_PaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in ControlStation__cbCallback --> WM_PAINT
*****************************************************************/

static void ControlStation_PaintDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;

}


/*****************************************************************
**      FunctionName:void ControlStation_InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in ControlStation__cbCallback --> WM_INIT_DIALOG
*****************************************************************/

static void ControlStation_InitDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;
	//
	//FRAMEWIN
	//
	FRAMEWIN_SetTextColor(hWin, 0x0000ff);
	FRAMEWIN_SetFont(hWin, &GUI_Font16B_ASCII);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_TOP | GUI_TA_CENTER);
	FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
	FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
	FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
	FRAMEWIN_SetTitleHeight(hWin, 16);
	//
	//GUI_ID_SLIDER0
	//
	SLIDER_SetRange(WM_GetDialogItem(hWin, GUI_ID_SLIDER0), 0, 100);
	SLIDER_SetValue(WM_GetDialogItem(hWin, GUI_ID_SLIDER0), 1);
	//
	//GUI_ID_SLIDER1
	//
	SLIDER_SetRange(WM_GetDialogItem(hWin, GUI_ID_SLIDER1), 0, 9000);
	SLIDER_SetValue(WM_GetDialogItem(hWin, GUI_ID_SLIDER1), 3000);
	//
	//GUI_ID_TEXT0
	//
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_Font24_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT0), GUI_TA_TOP | GUI_TA_CENTER);
	//
	//GUI_ID_TEXT1
	//
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_Font24_ASCII);
	TEXT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_TEXT1), GUI_TA_TOP | GUI_TA_CENTER);

}


/*********************************************************************
*
*       Dialog callback routine
*/
static void ControlStation__cbCallback (WM_MESSAGE* pMsg) {
	int NCode, Id;
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
		case WM_PAINT:
			ControlStation_PaintDialog(pMsg);
			break;
		case WM_INIT_DIALOG:
			ControlStation_InitDialog(pMsg);
			//获取滑条句柄
			g_UserInput.SliderTime.Handle = WM_GetDialogItem(hWin, GUI_ID_SLIDER0);
			g_UserInput.SliderAmpli.Handle = WM_GetDialogItem(hWin, GUI_ID_SLIDER1);
			break;
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) {
				case GUI_KEY_ESCAPE:
					GUI_EndDialog(hWin, 1);
					break;
				case GUI_KEY_ENTER:
					GUI_EndDialog(hWin, 0);
					break;
			}
			break;
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc);
			NCode = pMsg->Data.v;

			//获取滑条数值
			g_UserInput.SliderTime.Value = SLIDER_GetValue(g_UserInput.SliderTime.Handle);
			g_UserInput.SliderAmpli.Value = SLIDER_GetValue(g_UserInput.SliderAmpli.Handle);

			switch (Id) {
				case GUI_ID_OK:
					if (NCode == WM_NOTIFICATION_RELEASED)
						GUI_EndDialog(hWin, 0);
					break;
				case GUI_ID_CANCEL:
					if (NCode == WM_NOTIFICATION_RELEASED)
						GUI_EndDialog(hWin, 0);
					break;

			}
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


/*********************************************************************
*
*       ControlStation_Task
*
**********************************************************************
*/
WM_HWIN ControlStation_CreateWindow (void) {
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(ControlStation_aDialogCreate, GUI_COUNTOF (ControlStation_aDialogCreate), &ControlStation__cbCallback, 0, 0, 0);
	return hWin;
}

/*********************************************************************
*
*       ControlStation键盘开关
*
*/
void ConStSwitch (u8 state) {
	if (state == eOpen) {
		WM_ShowWindow(g_Disp.BSP_ConStDlg);
	}
	else {
		WM_HideWindow(g_Disp.BSP_ConStDlg);
	}
}

/*********************************************************************
*
*      Slider梯度化处理
*
*/
void SliderClassUpdata (Slider_struct* Slider) {
	if (isBound (Slider->Value, 0, 10))
		Slider->Class = 1;
	else if (isBound (Slider->Value, 10, 20))
		Slider->Class = 2;
	else if (isBound (Slider->Value, 20, 30))
		Slider->Class = 3;
	else if (isBound (Slider->Value, 30, 40))
		Slider->Class = 4;
	else if (isBound (Slider->Value, 40, 50))
		Slider->Class = 5;
	else if (isBound (Slider->Value, 50, 60))
		Slider->Class = 6;
	else if (isBound (Slider->Value, 60, 70))
		Slider->Class = 7;
	else if (isBound (Slider->Value, 70, 80))
		Slider->Class = 8;
	else if (isBound (Slider->Value, 80, 90))
		Slider->Class = 9;
	else if (isBound (Slider->Value, 90, 100))
		Slider->Class = 10;

}

