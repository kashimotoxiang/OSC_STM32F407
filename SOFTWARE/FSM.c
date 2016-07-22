#include "FSM.h"
extern ADC_HandleTypeDef hadc3;
//extern DAC_HandleTypeDef hdac;
uint8_t ax[MEMORYE_DEPTH] = {0};
uint8_t FPGACOMStr[] = {"2345"};

/*示波器-------------------------------------------------------*/
void FSM_OSC (void) {
	static u16 freq = 0, as = 0;
	if (g_GUICon.GraphDispState == eOpen) {
		/*数据填充-------------------------------------------------------*/
#ifdef DEBUG__ 
		//内部数据生成，仿真模式 
	WaveCreate(eSineWave, g_SamplData.Data, MEMORYE_DEPTH, 440, 100, as++);//重新生成这次的点
	if (as > 20)
		as = 0;
#endif
#ifdef ADCTAKESAMPLE__ 
		//ADC采样模式填充数组
		HAL_ADC_Start(&hadc3);
		HAL_ADC_Start_DMA(&hadc3, (uint32_t*)g_SamplData.Data, MEMORYE_DEPTH);
		hadc3.Instance->SR &= ~ADC_SR_OVR ;
#endif
#ifdef SPIDATATRANS__
		//HAL_GPIO_WritePin (SPI3_CS_PIN_GPIO_Port, SPI3_CS_PIN_Pin, GPIO_PIN_RESET);
		//HAL_SPI_Transmit (&hspi3, (uint8_t *)FPGACOMStr, 4, 100);
		//while (HAL_SPI_GetState (&hspi3) != HAL_SPI_STATE_READY) {
		//}
		//HAL_GPIO_WritePin (SPI3_CS_PIN_GPIO_Port, SPI3_CS_PIN_Pin, GPIO_PIN_SET);

		//HAL_GPIO_WritePin (SPI3_CS_PIN_GPIO_Port, SPI3_CS_PIN_Pin, GPIO_PIN_RESET);
		//HAL_SPI_TransmitReceive_DMA (&hspi3, (uint8_t *)ax, (uint8_t *)g_SamplData.Data, MEMORYE_DEPTH);
		//while (HAL_SPI_GetState (&hspi3) != HAL_SPI_STATE_READY) {
		//}
		//HAL_GPIO_WritePin (SPI3_CS_PIN_GPIO_Port, SPI3_CS_PIN_Pin, GPIO_PIN_SET);
#endif
	}
	/*-------------------------------------------------------*/
	GUIDataUpdata();//显示更新
	//控制部分------------------------------------------------------------------------------------------//
	g_OSCInfo.i_MaxVal = Get_ADC1_Average(1, 4);//第一个参数为通道数，第二个参数为求平均次数
	g_OSCInfo.f_MaxVal = (float)(g_OSCInfo.i_MaxVal) * (3.3 / 4096);
	//	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, g_OSCInfo.i_MaxVal);//控制增益放大
}

/*频谱分析仪-------------------------------------------------------*/
void FSM_SP (void) {
	FFT_Conv((q15_t*)g_SamplData.Data, (q15_t*)g_SamplData.FFT_Data, ARR_SIZE (g_SamplData.Data));
}

