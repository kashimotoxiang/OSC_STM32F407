#pragma once

#include "GUIDefine.h"

/*********************************************************************
*
*       struct
*
**********************************************************************
*/
typedef struct MeasureData_struct//FMS
{
	char FreqData[10];
	char DutyData[10];
	char AmlifData[10];
	EDIT_Handle FreqHandle;
	EDIT_Handle DutyHandle;
	EDIT_Handle AmlifHandle;
} MeasureData_struct;

/*********************************************************************
*
*       extern 
*
**********************************************************************
*/
extern MeasureData_struct g_MeasureData;

/*********************************************************************
*
*       Declartion
*
**********************************************************************
*/
void MeasureDataUpdata (void);
WM_HWIN MeasureData_CreateWindow (void);

