#include "FSM.h"

extern ADC_HandleTypeDef hadc3;
extern UART_HandleTypeDef huart3;
extern DAC_HandleTypeDef hdac;

/*示波器-------------------------------------------------------*/
void FSM_OSCDisp (void) {
	///*无效化按键使之重绘-------------------------------------------------------*/
	//WM_InvalidateWindow (g_GraphButton.ZoomPlus.Handle);
	//WM_InvalidateWindow (g_GraphButton.ZoomSub.Handle);
	//WM_InvalidateWindow (g_GraphButton.AmpliPlus.Handle);
	//WM_InvalidateWindow (g_GraphButton.AmpliSub.Handle);
	//WM_InvalidateWindow (g_GraphButton.Measure.Handle);
	//WM_InvalidateWindow (g_GraphButton.NumPad.Handle);
	//WM_InvalidateWindow (g_GraphButton.Stop.Handle);

	///*控制数据更新-------------------------------------------------------*/
	//g_OSCInfo.Time_rat = g_UserInput.SliderTime.Class;
	//g_OSCInfo.Ampli_rat = g_UserInput.SliderAmpli.Value;

	int i = 0;
	if (!OSC_DataDeal(WaveArray))//数据处理
		return;
	/*显示-------------------------------------------------------*/
	//GRAPH_DATA_YT_Clear(_hData);
	for (i = 0; i < TFT_WIDTH; i++) {
		//GRAPH_DATA_YT_AddValue(_hData, WaveArray[i]);
	}
}


void FSM_DeviceOperation (void) {
	/*调整比较器阈值控制波形占空比-------------------------------------------------------*/
	g_OSCInfo.i_MaxVal = Get_ADC1_Average(1, 10);//第一个参数为通道数，第二个参数为求平均次数
	g_OSCInfo.f_MaxVal = (float)(g_OSCInfo.i_MaxVal) * (3.3 / 4096);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 4096 / 2);//控制增益放大
}

void FSM_DeviceInit (void) {
	/*产生波形-------------------------------------------------------*/
	if (g_Device.AD9834Duty_l == g_Device.AD9834Duty) {//占空比更新
		g_Device.AD9834Duty_l = g_Device.AD9834Duty;
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (g_Device.AD9834Duty) * 4096 / 100);//控制比较器阈值调节占空比
	}
	if (g_Device.AD9834Freq_l == g_Device.AD9834Freq) {//频率更新
		g_Device.AD9834Freq_l = g_Device.AD9834Freq;
		AD9834_Set_Freq(FREQ_0, g_Device.AD9834Freq);//设置频率
	}
}

