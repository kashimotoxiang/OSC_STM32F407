#include "FPGAControl.h"
#define  PARDATATRANSPORT	GPIOC

#define FPGA_SCKSET			FPGA_SCK_GPIO_Port->ODR|=FPGA_SCK_Pin
#define FPGA_SCKCLEAR		FPGA_SCK_GPIO_Port->ODR&=~FPGA_SCK_Pin


/*并口数据传输-------------------------------------------------------*/
void FPGA_ParallDataTrans (void) {
	int i = 0;
	FPGA_SCKSET;
	for (i = 0; i < MEMORYE_DEPTH; i++) {
		FPGA_SCKCLEAR;
		g_FPGAData.ADCConvData[i] = (WAVE_TYPE)(PARDATATRANSPORT->IDR) & 0x0F;
		FPGA_SCKSET;
	}
	FPGA_SCKCLEAR;
	g_FPGAData.isEquSampl = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	FPGA_SCKSET;

	FPGA_SCKCLEAR;
	g_FPGAData.SamplFreq = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	FPGA_SCKSET;

	FPGA_SCKCLEAR;
	g_FPGAData.NumpadFreq = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	FPGA_SCKSET;

	FPGA_SCKCLEAR;
	g_FPGAData.DutyCycle = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	FPGA_SCKSET;

}

