#pragma once

#include "GUIDefine.h"

/* ResourceMap---------------------------------------------------------*/
extern ResourceMap_struct NumPad_RMs;
/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/

WM_HWIN Numpad_CreateWindow (void);
void RMSwitch (u8 state, ResourceMap_struct RMs);
WM_HWIN Text_CreateWindow (void);

