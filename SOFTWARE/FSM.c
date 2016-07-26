#include "FSM.h"

extern ADC_HandleTypeDef hadc3;
extern UART_HandleTypeDef huart3;
extern DAC_HandleTypeDef hdac;

/*示波器-------------------------------------------------------*/
void FSM_OSCDisp (void) {
	if (g_GUICon.GraphDispState == eOpen) {
		/*数据填充-------------------------------------------------------*/
#ifdef ADCTAKESAMPLE__ 
		//ADC采样模式填充数组
		HAL_ADC_Start(&hadc3);
		HAL_ADC_Start_DMA(&hadc3, (uint32_t*)g_FPGAData.ADCDispData, MEMORYE_DEPTH);
		hadc3.Instance->SR &= ~ADC_SR_OVR ;
#endif
#ifdef UARTDATATRANS__
		if (HAL_UART_Receive_DMA(&huart3, g_FPGAData.ADCDispData, MEMORYE_DEPTH)) {
			Error_Handler();
		}
#endif
#ifdef PARALLELDATA__
		FPGA_ParallDataTrans ();//FPGA数据填充
#endif
	}
	/*-------------------------------------------------------*/
	GUIDataUpdata();//显示更新
}



void FSM_DeviceOperation (void) {
	/*调整比较器阈值控制波形占空比-------------------------------------------------------*/
	g_OSCInfo.i_MaxVal = Get_ADC1_Average(1, 10);//第一个参数为通道数，第二个参数为求平均次数
	g_OSCInfo.f_MaxVal = (float)(g_OSCInfo.i_MaxVal) * (3.3 / 4096);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, g_OSCInfo.i_MaxVal);//控制增益放大
}

void FSM_DeviceInit (void) {
	/*产生波形-------------------------------------------------------*/
	AD9834_Select_Wave(g_Device.AD9834Wava);
	AD9834_Set_Freq(FREQ_0, g_Device.AD9834Freq);
}

