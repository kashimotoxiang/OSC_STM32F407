#include "FPGAControl.h"
#define  PARDATATRANSPORT	GPIOC

#define FPGA_MOSISET		FPGA_MISO_GPIO_Port->ODR|=FPGA_MISO_Pin
#define FPGA_MOSICLEAR		FPGA_MISO_GPIO_Port->ODR&=~FPGA_MISO_Pin

#define FPGA_MISOREAD		 ((FPGA_MISOREAD_GPIO_Port->IDR)&FPGA_MISOREAD_Pin)

#define TRANSPREPARE()  {\
		FPGA_MOSISET;\   
		while (FPGA_MISOREAD);\   
		FPGA_MOSICLEAR;\   
}\


/*并口数据传输-------------------------------------------------------*/
void FPGA_ParallDataTrans (void) {
	int i = 0;
	for (i = 0; i < MEMORYE_DEPTH; i++) {
		TRANSPREPARE();
		g_FPGAData.ADCConvData[i] = (WAVE_TYPE)(PARDATATRANSPORT->IDR) & 0x0F;
	}
	TRANSPREPARE ();
	g_FPGAData.isEquSampl = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	TRANSPREPARE ();
	g_FPGAData.SamplFreq = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	TRANSPREPARE ();
	g_FPGAData.NumpadFreq = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;
	TRANSPREPARE ();
	g_FPGAData.DutyCycle = (uint32_t)(PARDATATRANSPORT->IDR) & 0x0F;

}

