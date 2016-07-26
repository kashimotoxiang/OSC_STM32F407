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
	char* FreqUnit;
	char DutyData[10];
	char* DutyUnit;
	char AmlifData[10];
	char* AmlifUnit;
	EDIT_Handle FreqHandle;
	EDIT_Handle FreqUnitHandle;
	EDIT_Handle DutyHandle;
	EDIT_Handle DutyUnitHandle;
	EDIT_Handle AmlifHandle;
	EDIT_Handle AmlifUnitHandle;
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

