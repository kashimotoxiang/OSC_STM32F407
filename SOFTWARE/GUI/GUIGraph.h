#pragma once

#include "GUIDefine.h"

/*********************************************************************
*
*       extern
*
**********************************************************************
*/
extern MAINBUTTON_struct g_GraphButton;

/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/
/*-------------------------------------------------------*/
void Main_UserDraw (WM_HWIN hWin, int Stage);
void Main_cbCallback (WM_MESSAGE* pMsg);
WM_HWIN Main_CreateWindow (void);

