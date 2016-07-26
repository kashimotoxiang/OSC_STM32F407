#include "FPGAControl.h"
#include "myUART.h"

void FPGA_GetMeasurePar (void) {
	//状态1：请求基本参数
	FPGASTATION1();
	/*初始化数据-------------------------------------------------------*/
	uint32_t cnt_ch1[4] = {0}, cnt_ref1[4] = {0}, ch1_H[4] = {0}, ch1_L[4] = {0};
	uint32_t cnt_ch1_Value = 0, cnt_ref1_Value = 0, ch1_H_Value = 0, ch1_L_Value = 0;
	/*接收数据-------------------------------------------------------*/
	cnt_ch1[0] = UART_SendReceive8bit(97);
	cnt_ch1[1] = UART_SendReceive8bit(98);
	cnt_ch1[2] = UART_SendReceive8bit(99);
	cnt_ch1[3] = UART_SendReceive8bit(100);

	cnt_ref1[0] = UART_SendReceive8bit(101);
	cnt_ref1[1] = UART_SendReceive8bit(102);
	cnt_ref1[2] = UART_SendReceive8bit(103);
	cnt_ref1[3] = UART_SendReceive8bit(104);

	ch1_H[0] = UART_SendReceive8bit(105);
	ch1_H[1] = UART_SendReceive8bit(106);
	ch1_H[2] = UART_SendReceive8bit(107);
	ch1_H[3] = UART_SendReceive8bit(108);

	ch1_L[0] = UART_SendReceive8bit(109);
	ch1_L[1] = UART_SendReceive8bit(110);
	ch1_L[2] = UART_SendReceive8bit(111);
	ch1_L[3] = UART_SendReceive8bit(112);

	/*组合数据-------------------------------------------------------*/
	cnt_ch1_Value = (cnt_ch1[0] << 24) | (cnt_ch1[1] << 16) | (cnt_ch1[2] << 8) | (cnt_ch1[3]);
	cnt_ref1_Value = (cnt_ref1[0] << 24) | (cnt_ref1[1] << 16) | (cnt_ref1[2] << 8) | (cnt_ref1[3]);
	ch1_H_Value = (ch1_H[0] << 24) | (ch1_H[1] << 16) | (ch1_H[2] << 8) | (ch1_H[3]);
	ch1_L_Value = (ch1_L[0] << 24) | (ch1_L[1] << 16) | (ch1_L[2] << 8) | (ch1_L[3]);

	/*数值计算-------------------------------------------------------*/
	g_FPGAData.SignalFreq = (cnt_ch1_Value / (double)cnt_ref1_Value) * 50000000;//50Mhz为基准频率
	g_FPGAData.DutyCycle = ch1_H_Value / ((double)ch1_H_Value + (double)ch1_L_Value);
}

/*写入FPGA参数-------------------------------------------------------*/
void FPGA_WritePar (uint32_t m_SamplFreq, uint16_t m_HighThres, uint16_t m_LowThres, uint8_t m_LengthCom, uint8_t m_AddPeriod, FPGAControl_struct* FPGAPar) {
	/*计算参数-------------------------------------------------------*/
	FPGAPar->SamplFreq = m_SamplFreq;//设置采样频率
	FPGAPar->HighThreshold = m_HighThres;
	FPGAPar->LowThreshold = m_LowThres;
	FPGAPar->DataLengthCOM = m_LengthCom;//转换数据大小

	FPGAConterSet(FPGAPar);//计算计数器时间

	FPGAPar->HighTimes += m_AddPeriod;//适当延长采样周期

	FPGAPar->SamplFreq = FPGAPar->FPGA_Freq / (FPGAPar->HighTimes + FPGAPar->LowTimes);//重新设置采样频率

	//状态2：写入基本参数
	FPGASTATION2();
	/*发送参数-------------------------------------------------------*/
	UART_Send32Bit(FPGAPar->HighTimes);
	UART_Send32Bit(FPGAPar->LowTimes);
	UART_Send16Bit(FPGAPar->HighThreshold);
	UART_Send16Bit(FPGAPar->LowThreshold);
	UART_Send8Bit(FPGAPar->DataLengthCOM);
	UART_Send16Bit(FPGAPar->SamplDalayTimes);
}


/*并口数据传输-------------------------------------------------------*/
void FPGA_ParallDataTrans (WAVE_TYPE* WaveArray, uint16_t length) {
	int i = 0;
	//状态3：请求发送数据
	FPGASTATION3();
	/*启动数据传输-------------------------------------------------------*/
	FPGA_SCK_SET ;
	delay_25ns(FPGAWAITTIMES);
	for (i = 0; i < length; i++) {
		FPGA_SCK_CLEAR ;
		delay_25ns(FPGAWAITTIMES);
		WaveArray[i] = (WAVE_TYPE)((FPGA_PARDATATRANSPORT->IDR) & FPGA_PARDATATRANSMASK);
		FPGA_SCK_SET ;
		delay_25ns(FPGAWAITTIMES);
	}
}

/*写入FPGA计数器计算-------------------------------------------------------*/
void FPGAConterSet (FPGAControl_struct* FPGACon) {
	uint32_t TotalTimes = 0;
	TotalTimes = ((double)(FPGACon->FPGA_Freq)) / (FPGACon->SamplFreq);//补一个1 保证稍长
	if (TotalTimes == (int)(1000000 / g_FPGAData.SignalFreq))
		TotalTimes++;
	FPGACon->HighTimes = TotalTimes / 2;//高电平时间
	if (TotalTimes & 0x01)//奇数//低电平时间
		FPGACon->LowTimes = FPGACon->HighTimes + 1;
	else//偶数
		FPGACon->LowTimes = FPGACon->HighTimes;
	FPGACon->SamplDalayTimes = (double)(FPGACon->FPGA_Freq) * FPGADELAYRATIO / (FPGACon->SamplFreq);
}

#define NUMOFFLITER		20

/*幅度计算-------------------------------------------------------*/
void FPGA_AmplitudeCompute (FPGAData_struct* FPGAData) {
	/*排序并过滤-------------------------------------------------------*/
	InsertSort(FPGAData->AmlifMeasureData, 0, AMPLITITUDEDATALENGTH - 1);
	FPGAData->i_AmplitudeMax = FPGAData->AmlifMeasureData[AMPLITITUDEDATALENGTH - NUMOFFLITER];
	FPGAData->i_AmplitudeMin = FPGAData->AmlifMeasureData[NUMOFFLITER];
	FPGAData->i_Amplitude = FPGAData->i_AmplitudeMax - FPGAData->i_AmplitudeMin;
}

/*上升时间计算-------------------------------------------------------*/
void FPGA_UpTimeCompute (FPGAData_struct* FPGAData, FPGAControl_struct* FPGACon) {
	/*数据初始化-------------------------------------------------------*/
	int seglist[UPSAMPLEAVETIMES ] = {0}, i = 0, n = 0;//间断表
	int JumpPointSub = 0;
	double sum = 0;//总间隔点数
	JumpPointSub = (FPGACon->HighThreshold - FPGACon->LowThreshold) / 2;
	FPGAData->TimeSpace = 1000000000.0 / (FPGACon->HighTimes + FPGACon->LowTimes);//两点之间时间间隔(ns)

	/*找到第一个完整上升沿-------------------------------------------------------*/
	for (i = 0; i < MEMORYE_DEPTH; i++)
		if ((FPGAData->UpTimeData[i + 1] - FPGAData->UpTimeData[i]) > JumpPointSub)//找到跳跃点
		{
			seglist[n++] = i;
			break;
		}
	/*开始计算-------------------------------------------------------*/
	for (; i < MEMORYE_DEPTH && n < UPSAMPLEAVETIMES; i++)
		if ((FPGAData->UpTimeData[i + 1] - FPGAData->UpTimeData[i]) > JumpPointSub)//找到跳跃点
			seglist[n++] = i;
	/*计算平均时间-------------------------------------------------------*/
	for (i = 1; i < n; i++)
		sum += seglist[i] - seglist[i - 1];
	sum /= (n - 1);//n个端点有n-1段
	FPGAData->UpTime = sum * FPGAData->TimeSpace;
}

//状态1：请求基本参数
inline void FPGASTATION1 (void) {
	FPGA_COM1_SET ;
	FPGA_COM2_CLR ;
	delay_25ns(FPGAWAITTIMES);
}

//状态2：写入基本参数
inline void FPGASTATION2 (void) {
	FPGA_COM1_CLR ;
	FPGA_COM2_SET ;
	delay_25ns(FPGAWAITTIMES);
}

//状态3：请求发送数据
inline void FPGASTATION3 (void) {
	FPGA_COM1_SET ;
	FPGA_COM2_SET ;
	delay_25ns(FPGAWAITTIMES);
}

