/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2014  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.26 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : WIDGET_NumPad.c
Purpose     : Shows how to use a numpad as input device on a touch screen
Requirements: WindowManager - (x)
MemoryDevices - ( )
AntiAliasing  - ( )
VNC-Server    - ( )
PNG-Library   - ( )
TrueTypeFonts - ( )
---------------------------END-OF-HEADER------------------------------
*/

#include "GUINumPad.h"
#include "BUTTON_Private.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
//
// Bitmap data for arrow keys
//

ResourceMap_struct Numpad_RMs;

static GUI_CONST_STORAGE GUI_COLOR NumPad_aColorsArrow[] = {
	0xFFFFFF, 0x000000
};

static GUI_CONST_STORAGE GUI_LOGPALETTE NumPad_PalArrow = {
	2, /* number of entries */
	1, /* No transparency */
	&NumPad_aColorsArrow[0]
};

static GUI_CONST_STORAGE unsigned char NumPad_acArrowRight[] = {
	____XX__, ________,
	____XXXX, ________,
	XXXXXXXX, XX______,
	____XXXX, ________,
	____XX__, ________,

};

static GUI_CONST_STORAGE unsigned char NumPad_acArrowLeft[] = {
	____XX__, ________,
	__XXXX__, ________,
	XXXXXXXX, XX______,
	__XXXX__, ________,
	____XX__, ________,

};

static GUI_CONST_STORAGE GUI_BITMAP NumPad_bmArrowRight = {
	10, /* XSize */
	5, /* YSize */
	2, /* BytesPerLine */
	1, /* BitsPerPixel */
	NumPad_acArrowRight, /* Pointer to picture data (indices) */
	&NumPad_PalArrow /* Pointer to palette */
};

static GUI_CONST_STORAGE GUI_BITMAP NumPad_bmArrowLeft = {
	10, /* XSize */
	5, /* YSize */
	2, /* BytesPerLine */
	1, /* BitsPerPixel */
	NumPad_acArrowLeft, /* Pointer to picture data (indices) */
	&NumPad_PalArrow /* Pointer to palette */
};
//
// Array of keys
//
static int _aKey[] = {GUI_KEY_DELETE, GUI_KEY_ENTER , GUI_KEY_LEFT, GUI_KEY_RIGHT};

//
// Dialog resource of numpad
//
static const GUI_WIDGET_CREATE_INFO NumPad_aDialog[] = {
	//
	//  Function                 Text      Id                 Px   Py   Dx   Dy
	//
	{FRAMEWIN_CreateIndirect, "NumPad", GUI_ID_USER + 20, 0, 0, 240, 320, 0, 0},
	{EDIT_CreateIndirect, 0, GUI_ID_EDIT0, 4, 3, 224, 59, 0, 0},
	{BUTTON_CreateIndirect, "7", GUI_ID_USER + 7, 6, 68, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "8", GUI_ID_USER + 8, 82, 68, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "9", GUI_ID_USER + 9, 158, 68, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "4", GUI_ID_USER + 4, 6, 114, 70, 40, 0 , 0},
	{BUTTON_CreateIndirect, "5", GUI_ID_USER + 5, 82, 114, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "6", GUI_ID_USER + 6, 158, 114, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "1", GUI_ID_USER + 1, 6, 160, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "2", GUI_ID_USER + 2, 82, 160, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "3", GUI_ID_USER + 3, 158, 160, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "0", GUI_ID_USER + 0, 6, 206, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, ".", GUI_ID_USER + 10, 82, 206, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "Del", GUI_ID_USER + 11, 158, 206, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, 0, GUI_ID_USER + 13, 6, 252, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, 0, GUI_ID_USER + 14, 82, 252, 70, 40, 0, 0},
	{BUTTON_CreateIndirect, "ENTER", GUI_ID_USER + 12, 158, 252, 70, 40, 0, 0}
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

/*****************************************************************
**      FunctionName:void NumPad_InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void NumPad_InitDialog (WM_MESSAGE* pMsg) {
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
	//GUI_User_ID
	//
	EDIT_SetText(WM_GetDialogItem(hWin, GUI_ID_EDIT0), "EDIT0");
	for (int i = 0; i < 15; i++)
		BUTTON_SetFont(WM_GetDialogItem(hWin, GUI_ID_USER + i), &GUI_Font20B_ASCII);
}

/*********************************************************************
*
*       NumPad_cbDesktop
*
* Function description
*   This routine handles the drawing of the desktop window.
*/
static void NumPad_cbDesktop (WM_MESSAGE* pMsg) {
	unsigned i;
	int NCode;
	unsigned Id;
	int Pressed;
	WM_HWIN hDlg;
	WM_HWIN hItem;

	Pressed = 0;
	hDlg = pMsg->hWin;
	switch (pMsg->MsgId) {
		case WM_PAINT:
			GUI_SetBkColor(GUI_BLACK);
			GUI_Clear();
			break;
		case WM_INIT_DIALOG:
			NumPad_InitDialog(pMsg);
			/* EDITOR INIT */
			hItem = WM_GetDialogItem(hDlg, GUI_ID_EDIT0);
			g_UserInput.Numpad.EdirorHandle = hItem;
			EDIT_SetText(hItem, "");
			EDIT_SetFont(hItem, &GUI_FontD24x32);
			EDIT_SetTextColor(hItem, 0, GUI_RED);
			//当前显示1000，最大5000，最小0，小数点后4位，不显示前导零
			EDIT_SetFloatMode(hItem, 1000, 0, 5000, 4, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
			for (i = 0; i < GUI_COUNTOF (NumPad_aDialog) - 1; i++) {
				hItem = WM_GetDialogItem(hDlg, GUI_ID_USER + i);
				BUTTON_SetFocussable(hItem, 0); /* Set all buttons non focussable */
				switch (i) {
					case 13:
						BUTTON_SetBitmapEx(hItem, 0, &NumPad_bmArrowLeft, 27, 18); /* Set bitmap for arrow left button (unpressed) */
						BUTTON_SetBitmapEx(hItem, 1, &NumPad_bmArrowLeft, 27, 18); /* Set bitmap for arrow left button (pressed) */
						break;
					case 14:
						BUTTON_SetBitmapEx(hItem, 0, &NumPad_bmArrowRight, 27, 18); /* Set bitmap for arrow right button (unpressed) */
						BUTTON_SetBitmapEx(hItem, 1, &NumPad_bmArrowRight, 27, 18); /* Set bitmap for arrow right button (pressed) */
						break;
				}
			}
			WM_GetDialogItem(hDlg, GUI_ID_USER + 12);
			break;
		case WM_NOTIFY_PARENT:
			Id = WM_GetId(pMsg->hWinSrc); /* Id of widget */
			NCode = pMsg->Data.v; /* Notification code */
			switch (NCode) {
				case WM_NOTIFICATION_CLICKED:
					Pressed = 1;
				case WM_NOTIFICATION_RELEASED:
					if ((Id >= GUI_ID_USER) && (Id <= (GUI_ID_USER + GUI_COUNTOF (NumPad_aDialog) - 1))) {
						int Key;
						if (Id < GUI_ID_USER + 11) {
							char acBuffer[10];
							BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof (acBuffer)); /* Get the text of the button */
							Key = acBuffer[0];
						}
						else {
							Key = _aKey[Id - GUI_ID_USER - 11]; /* Get the text from the array */
						}

						GUI_SendKeyMsg(Key, Pressed); /* Send a key message to the focussed window */

						//获取Editor输入值
						if (Key == GUI_KEY_ENTER) {
							if (g_UserInput.InputMode == eAD9834DutyInput)//占空比
							{
								g_UserInput.Numpad.f_InputValue = EDIT_GetFloatValue(g_UserInput.Numpad.EdirorHandle);
								g_Device.AD9834Duty = g_UserInput.Numpad.f_InputValue;
							}
							else {//频率
								g_UserInput.Numpad.i_InputValue = EDIT_GetValue(g_UserInput.Numpad.EdirorHandle);
								g_Device.AD9834Freq = (int)g_UserInput.Numpad.i_InputValue;
							}
						}
					}
					break;
			}
		default:
			WM_DefaultProc(pMsg);
	}
}

/*********************************************************************
*
*       Exported code
*
**********************************************************************
*/
WM_HWIN Numpad_CreateWindow (void) {
	WM_HWIN hWin;
	/*-------------------------------------------------------*/
	hWin = GUI_CreateDialogBox(NumPad_aDialog,GUI_COUNTOF (NumPad_aDialog),
	                           &NumPad_cbDesktop, 0, 0, 0); /* Create the numpad dialog */
	return hWin;
}


/*************************** End of file ****************************/


