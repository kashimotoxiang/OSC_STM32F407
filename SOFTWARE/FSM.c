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
		HAL_ADC_Start_DMA(&hadc3, (uint32_t*)g_FPGAData.ADCConvData, MEMORYE_DEPTH);
		hadc3.Instance->SR &= ~ADC_SR_OVR ;
#endif
#ifdef UARTDATATRANS__
		if (HAL_UART_Receive_DMA(&huart3, g_FPGAData.ADCConvData, MEMORYE_DEPTH)) {
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

void FSM_MeasureUpdata (void) {
	/*初始化数据-------------------------------------------------------*/
	uint32_t cnt_ch1[4] = {0}, cnt_ref1[4] = {0}, ch1_H[4] = {0}, ch1_L[4] = {0};
	uint32_t cnt_ch1_Value = 0, cnt_ref1_Value = 0, ch1_H_Value = 0, ch1_L_Value = 0;
uint32_t t=0;
	/*接收数据-------------------------------------------------------*/
	cnt_ch1[0] = UART_SendReceive(97);
	cnt_ch1[1] = UART_SendReceive(98) ;
	cnt_ch1[2] = UART_SendReceive(99);
	cnt_ch1[3] = UART_SendReceive(100);

	cnt_ref1[0] = UART_SendReceive(101);
	cnt_ref1[1] = UART_SendReceive(102);
	cnt_ref1[2] = UART_SendReceive(103);
	cnt_ref1[3] = UART_SendReceive(104);

	ch1_H[0] = UART_SendReceive(105);
	ch1_H[1] = UART_SendReceive(106);
	ch1_H[2] = UART_SendReceive(107);
	ch1_H[3] = UART_SendReceive(108);

	ch1_L[0] = UART_SendReceive(109);
	ch1_L[1] = UART_SendReceive(110);
	ch1_L[2] = UART_SendReceive(111);
	ch1_L[3] = UART_SendReceive(112);

	/*组合数据-------------------------------------------------------*/
	cnt_ch1_Value = (cnt_ch1[0] << 24)|(cnt_ch1[1] << 16 )|(cnt_ch1[2] << 8)|(cnt_ch1[3]);
	cnt_ref1_Value = (cnt_ref1[0] << 24)|(cnt_ref1[1] << 16)|(cnt_ref1[2] << 8 )|(cnt_ref1[3]);
	ch1_H_Value = (ch1_H[0] << 24)|(ch1_H[1] << 16 )|(ch1_H[2] << 8 )|( ch1_H[3]);
	ch1_L_Value = (ch1_L[0] << 24 )|(ch1_L[1] << 16 )|( ch1_L[2] << 8 )|(ch1_L[3]);

	/*数值计算-------------------------------------------------------*/
	g_FPGAData.SamplFreq = (cnt_ch1_Value / (double)cnt_ref1_Value) * 50000000;//50Mhz为基准频率
	g_FPGAData.DutyCycle = ch1_H_Value / ((double)ch1_H_Value + ch1_L_Value);
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

