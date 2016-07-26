/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include "GUIMeasureData.h"

//EventsFunctionList
//EndofEventsFunctionList


/*********************************************************************
*
*       static data
*
**********************************************************************
*/
static char _FreqUnit_Hz[] = "Mz";
static char _FreqUnit_KHz[] = "KHz";
static char _FreqUnit_MHz[] = "MHz";

static char _DutyUnit[] = "%";

static char _AmpliUnit_mV[] = "mV";
static char _AmpliUnit_V[] = "V";
/* FMS ---------------------------------------------------------*/


MeasureData_struct g_MeasureData = {
	"0", _FreqUnit_Hz,
	"0", _DutyUnit,
	"0", _AmpliUnit_mV,
};


/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO MeasureData_aDialog[] = {
	{FRAMEWIN_CreateIndirect, "Measured Data", GUI_ID_USER + 21, 0, 0, 240, 320, 0, 0},
	{TEXT_CreateIndirect, "Frequency:", GUI_ID_TEXT0, 9, 10, 116, 27, 0, 0},
	{TEXT_CreateIndirect, "Amplitude:", GUI_ID_TEXT2, 9, 210, 116, 27, 0, 0},
	{TEXT_CreateIndirect, "Duty Cycle:", GUI_ID_TEXT1, 9, 110, 116, 27, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT1, 9, 43, 175, 31, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT2, 9, 143, 175, 30, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT3, 9, 243, 175, 30, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT4, 189, 43, 40, 31, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT5, 189, 143, 40, 31, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT6, 189, 242, 40, 31, 0, 0}
};


/*****************************************************************
**      FunctionName:void MeasureDataPaintDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in MeasureData_cbCallback --> WM_PAINT
*****************************************************************/

void MeasureDataPaintDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;
}

/*****************************************************************
**
**		测量数据更新
**
**
*****************************************************************/
void MeasureDataUpdata (void) {
	/*单位转化-------------------------------------------------------*/
	//频率
	if (g_FPGAData.SignalFreq > 1000000) {//Mhz
		g_MeasureData.FreqUnit = _FreqUnit_MHz;
		g_FPGAData.SignalFreq /= 1000000.0;
	}
	else if (g_FPGAData.SignalFreq > 1000) { //Khz
		g_MeasureData.FreqUnit = _FreqUnit_KHz;
		g_FPGAData.SignalFreq /= 1000.0;
	}
	else {//Hz
		g_MeasureData.FreqUnit = _FreqUnit_Hz;
	}

	//幅度
	if (g_FPGAData.Amplitude > 1000) {//V
		g_MeasureData.AmlifUnit = _AmpliUnit_V;
		g_FPGAData.Amplitude /= 1000.0;
	}
	else {//mV
		g_MeasureData.AmlifUnit = _AmpliUnit_mV;
	}

	/*转化数据为字符串-------------------------------------------------------*/
	sprintf(g_MeasureData.FreqData, "%g", g_FPGAData.SignalFreq);
	sprintf(g_MeasureData.AmlifData, "%g", g_FPGAData.Amplitude);
	sprintf(g_MeasureData.DutyData, "%g", g_FPGAData.DutyCycle);

	/*显示数据-------------------------------------------------------*/
	EDIT_SetText(g_MeasureData.FreqHandle, g_MeasureData.FreqData);
	EDIT_SetText(g_MeasureData.DutyHandle, g_MeasureData.DutyData);
	EDIT_SetText(g_MeasureData.AmlifHandle, g_MeasureData.AmlifData);

	/*显示单位-------------------------------------------------------*/
	EDIT_SetText(g_MeasureData.FreqUnitHandle, g_MeasureData.FreqUnit);
	EDIT_SetText(g_MeasureData.DutyUnitHandle, g_MeasureData.DutyUnit);
	EDIT_SetText(g_MeasureData.AmlifUnitHandle, g_MeasureData.AmlifUnit);
}


/*****************************************************************
**      FunctionName:void MeasureData_InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in MeasureData_cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void MeasureData_InitDialog (WM_MESSAGE* pMsg) {
	WM_HWIN hWin = pMsg->hWin;
	//
	//FRAMEWIN
	//
	FRAMEWIN_SetBarColor(hWin, 1, 0xff901e);
	FRAMEWIN_SetClientColor(hWin, 0xc0c0c0);
	FRAMEWIN_SetTextColor(hWin, 0xcc3299);
	FRAMEWIN_SetFont(hWin, &GUI_Font16B_ASCII);
	FRAMEWIN_SetTextAlign(hWin, GUI_TA_VCENTER | GUI_TA_CENTER);
	FRAMEWIN_AddCloseButton(hWin, FRAMEWIN_BUTTON_RIGHT, 0);
	FRAMEWIN_AddMaxButton(hWin, FRAMEWIN_BUTTON_RIGHT, 1);
	FRAMEWIN_AddMinButton(hWin, FRAMEWIN_BUTTON_RIGHT, 2);
	FRAMEWIN_SetTitleHeight(hWin, 16);
	//
	//GUI_ID_TEXT0
	//
	TEXT_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), 0xc0c0c0);
	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT0), 0x228b22);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT0), &GUI_Font24B_ASCII);
	//
	//GUI_ID_TEXT2
	//
	TEXT_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_TEXT2), 0xc0c0c0);
	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT2), 0x4763ff);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2), &GUI_Font24B_ASCII);
	//
	//GUI_ID_TEXT1
	//
	TEXT_SetBkColor(WM_GetDialogItem(hWin, GUI_ID_TEXT1), 0xc0c0c0);
	TEXT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_TEXT1), 0xe16941);
	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1), &GUI_Font24B_ASCII);
	//
	//GUI_ID_EDIT1
	//
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT1), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT1), GUI_TA_VCENTER | GUI_TA_LEFT);
	//
	//GUI_ID_EDIT2
	//
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT2), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT2), GUI_TA_VCENTER | GUI_TA_LEFT);
	//
	//GUI_ID_EDIT3
	//
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT3), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT3), GUI_TA_VCENTER | GUI_TA_LEFT);
	//
	//GUI_ID_EDIT4
	//
	EDIT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_EDIT4), 0, 0x228b22);
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT4), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT4), GUI_TA_VCENTER | GUI_TA_CENTER);
	//
	//GUI_ID_EDIT5
	//
	EDIT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_EDIT5), 0, 0xff901e);
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT5), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT5), GUI_TA_VCENTER | GUI_TA_CENTER);
	//
	//GUI_ID_EDIT6
	//
	EDIT_SetTextColor(WM_GetDialogItem(hWin, GUI_ID_EDIT6), 0, 0x4763ff);
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT6), &GUI_Font24B_ASCII);
	EDIT_SetTextAlign(WM_GetDialogItem(hWin, GUI_ID_EDIT6), GUI_TA_VCENTER | GUI_TA_CENTER);
}


/*********************************************************************
*
*       Dialog callback routine
*/
static void MeasureData_cbCallback (WM_MESSAGE* pMsg) {
	int NCode, Id;
	WM_HWIN hWin = pMsg->hWin;
	switch (pMsg->MsgId) {
		case WM_PAINT:
			MeasureDataPaintDialog(pMsg);
			MeasureDataUpdata();
			break;
		case WM_INIT_DIALOG:
			MeasureData_InitDialog(pMsg);
			/*初始化句柄-------------------------------------------------------*/
			g_MeasureData.FreqHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT1);
			g_MeasureData.DutyHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
			g_MeasureData.AmlifHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
			g_MeasureData.FreqUnitHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT4);
			g_MeasureData.DutyUnitHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT5);
			g_MeasureData.AmlifUnitHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT6);
			/*禁止闪烁-------------------------------------------------------*/
			EDIT_EnableBlink(g_MeasureData.FreqHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.DutyHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.AmlifHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.FreqUnitHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.DutyUnitHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.AmlifUnitHandle, 0, 0);
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
*       MainTask
*
**********************************************************************
*/
WM_HWIN MeasureData_CreateWindow (void) {
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(MeasureData_aDialog, GUI_COUNTOF (MeasureData_aDialog),
	                           &MeasureData_cbCallback, 0, 0, 0);
	return hWin;
}

