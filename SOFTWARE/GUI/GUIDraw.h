#pragma once

#include "GUIDefine.h"

/*********************************************************************
*
*       extern 
*
**********************************************************************
*/
extern Disp_struct g_Disp;
extern UserInput_struct g_UserInput;
/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/
void ESP_MainTask (void);
void BSP_MainTask (void);
void GUIDataUpdata (void);
void OSC_MeasureInfoSwitch (u8 state);
void RMSwitch (u8 state, ResourceMap_struct RMs);//资源表开关
void WindowSwitch (WM_HWIN hWin, u8 state);
void CloseAllBSPDLG (void);

