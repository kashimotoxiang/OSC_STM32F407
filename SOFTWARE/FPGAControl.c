#include "FPGAControl.h"


void FPGA_GetMeasurePar (void) {
	//状态1：请求基本参数
	FPGASTATION1 ();
	/*初始化数据-------------------------------------------------------*/
	uint32_t cnt_ch1[4] = {0}, cnt_ref1[4] = {0}, ch1_H[4] = {0}, ch1_L[4] = {0};
	uint32_t cnt_ch1_Value = 0, cnt_ref1_Value = 0, ch1_H_Value = 0, ch1_L_Value = 0;
	uint32_t t = 0;
	/*接收数据-------------------------------------------------------*/
	cnt_ch1[0] = UART_SendReceive(97);
	cnt_ch1[1] = UART_SendReceive(98);
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
	cnt_ch1_Value = (cnt_ch1[0] << 24) | (cnt_ch1[1] << 16) | (cnt_ch1[2] << 8) | (cnt_ch1[3]);
	cnt_ref1_Value = (cnt_ref1[0] << 24) | (cnt_ref1[1] << 16) | (cnt_ref1[2] << 8) | (cnt_ref1[3]);
	ch1_H_Value = (ch1_H[0] << 24) | (ch1_H[1] << 16) | (ch1_H[2] << 8) | (ch1_H[3]);
	ch1_L_Value = (ch1_L[0] << 24) | (ch1_L[1] << 16) | (ch1_L[2] << 8) | (ch1_L[3]);

	/*数值计算-------------------------------------------------------*/
	g_FPGAData.SignalFreq = (cnt_ch1_Value / (double)cnt_ref1_Value) * 50000000;//50Mhz为基准频率
	g_FPGAData.DutyCycle = ch1_H_Value / ((double)ch1_H_Value + (double)ch1_L_Value);
}

/*写入FPGA参数-------------------------------------------------------*/
void FPGA_WritePar (uint32_t m_SamplFreq, uint16_t m_HighThres, uint16_t m_LowThres, uint8_t LengthCom, uint8_t m_AddPeriod, FPGAControl_struct g_FPGAPar) {
	/*计算参数-------------------------------------------------------*/
	g_FPGAPar.SamplFreq = m_SamplFreq;//设置采样频率
	g_FPGAPar.HighThreshold = m_HighThres;
	g_FPGAPar.LowThreshold = m_LowThres;
	g_FPGAPar.DataLengthCOM = LengthCom;//转换数据大小

	FPGAConterSet(&g_FPGAPar);//计算高电平低电平计数器时间

	g_FPGAPar.SamplFreq = g_FPGAPar.FPGA_Freq / (g_FPGAPar.HighThreshold + g_FPGAPar.LowThreshold);//重新设置采样频率

	g_FPGAPar.HighTimes += m_AddPeriod;//适当延长采样周期
	//状态2：写入基本参数
	FPGASTATION2 ();
	/*发送参数-------------------------------------------------------*/
	UART_Send32Bit(g_FPGAPar.HighTimes);
	UART_Send32Bit(g_FPGAPar.LowTimes);
	UART_Send16Bit(g_FPGAPar.HighThreshold);
	UART_Send16Bit(g_FPGAPar.LowThreshold);
	UART_Send8Bit(g_FPGAPar.DataLengthCOM);
}


/*并口数据传输-------------------------------------------------------*/
void FPGA_ParallDataTrans (WAVE_TYPE* WaveArray, FPGAControl_struct* FPGACon) {
	int i = 0, length = 0;
	/*判断数据大小-------------------------------------------------------*/
	if (FPGACon->DataLengthCOM == FPGADATALENTGH128)
		length = 128;
	else
		length = 2048;
	//状态3：请求发送数据
	FPGASTATION3 ();
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
	TotalTimes = 1.0 + (double)(FPGACon->FPGA_Freq) / (FPGACon->SamplFreq);//补一个1 保证稍长
	FPGACon->HighTimes = TotalTimes / 2;//高电平时间
	if (TotalTimes & 0x01)//奇数//低电平时间
		FPGACon->LowTimes = FPGACon->HighTimes + 1;
	else//偶数
		FPGACon->LowTimes = FPGACon->HighTimes;
}

/*幅度计算-------------------------------------------------------*/
void FPGA_AmplitudeCompute (FPGAData_struct* FPGAData) {
	/*数据初始化-------------------------------------------------------*/
	int i = 0;
	int max = 0x00, min = 0xffff;
	/*寻找最值-------------------------------------------------------*/
	for (i = 0; i < 128; i++) {
		if (max < FPGAData->ADCParMeasureData[i])
			max = FPGAData->ADCParMeasureData[i];
		if (min > FPGAData->ADCParMeasureData[i])
			min = FPGAData->ADCParMeasureData[i];
	}
	FPGAData->i_AmplitudeMax = max;
	FPGAData->i_AmplitudeMin = min;
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
		if ((FPGAData->ADCUpTimeData[i + 1] - FPGAData->ADCUpTimeData[i]) > JumpPointSub)//找到跳跃点
		{
			seglist[n++] = i;
			break;
		}

	/*开始计算-------------------------------------------------------*/
	for (; i < MEMORYE_DEPTH && n < UPSAMPLEAVETIMES; i++)
		if ((FPGAData->ADCUpTimeData[i + 1] - FPGAData->ADCUpTimeData[i]) > JumpPointSub)//找到跳跃点
			seglist[n++] = i;
	/*计算平均时间-------------------------------------------------------*/
	for (i = 1; i < n; i++)
		sum += seglist[i] - seglist[i - 1];
	sum /= (n - 1);//n个端点有n-1段
	FPGAData->UpTime = sum * FPGAData->TimeSpace;
}

