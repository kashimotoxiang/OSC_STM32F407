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

/* FMS ---------------------------------------------------------*/


MeasureData_struct g_MeasureData = {
	"0 MHz",
	"0 %",
	"0 mV",
};

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO MeasureData_aDialogCreate[] = {
	{FRAMEWIN_CreateIndirect, "Measured Data", 0, 0, 0, 240, 320, FRAMEWIN_CF_MOVEABLE, 0},
	{TEXT_CreateIndirect, "Frequency:", GUI_ID_TEXT0, 9, 10, 116, 27, 0, 0},
	{TEXT_CreateIndirect, "Amplitude:", GUI_ID_TEXT2, 9, 210, 116, 27, 0, 0},
	{TEXT_CreateIndirect, "Duty Cycle:", GUI_ID_TEXT1, 9, 110, 116, 27, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT1, 9, 43, 220, 30, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT2, 9, 143, 220, 30, 0, 0},
	{EDIT_CreateIndirect, NULL , GUI_ID_EDIT3, 9, 243, 220, 30, 0, 0}
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
	EDIT_SetText(g_MeasureData.FreqHandle, g_MeasureData.FreqData);
	EDIT_SetText(g_MeasureData.DutyHandle, g_MeasureData.DutyData);
	EDIT_SetText(g_MeasureData.AmlifHandle, g_MeasureData.AmlifData);
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
	EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT1), "Frequency Data");
	//
	//GUI_ID_EDIT2
	//
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT2), &GUI_Font24B_ASCII);
	EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT2), "Duty Cycle Data");
	//
	//GUI_ID_EDIT3
	//
	EDIT_SetFont(WM_GetDialogItem(hWin, GUI_ID_EDIT3), &GUI_Font24B_ASCII);
	EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT3), "Amplitude Data");

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
			g_MeasureData.FreqHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT1);//初始化句柄
			g_MeasureData.DutyHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT2);
			g_MeasureData.AmlifHandle = WM_GetDialogItem(hWin, GUI_ID_EDIT3);
			EDIT_EnableBlink(g_MeasureData.FreqHandle, 0, 0);//禁止闪烁
			EDIT_EnableBlink(g_MeasureData.DutyHandle, 0, 0);
			EDIT_EnableBlink(g_MeasureData.AmlifHandle, 0, 0);
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
	GUI_CreateDialogBox(MeasureData_aDialogCreate, GUI_COUNTOF (MeasureData_aDialogCreate), &MeasureData_cbCallback, 0, 0, 0);
	return hWin;
}

