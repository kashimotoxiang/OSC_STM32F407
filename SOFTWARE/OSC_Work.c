#include "OSC_Work.h"

#define INSERT_SIZE 10//小于这个值时切入插入排序
#define FPGA_SAMPL_RATE 1000*1000
#define EQUAL_SAMPL_GATE 10//等效采样阈值，单周期采样点数

/*===================================================================*/

/**
* @brief g_OSCInfo数据处理
* @param
* @note
*/
u8 OSC_DataDeal(I16* WaveArray, int DataLength) {
	static int i = 0, n = 0, m = 0;//下标
	/*触发设置-------------------------------------------------------*/
	g_OSCInfo.DataEnd = MEMORYE_DEPTH - TFT_WIDTH;//边界确定
	OSC_MaxMin_Found(&g_OSCInfo.i_MaxVal, &g_OSCInfo.i_MinVal, g_FPGAData.ADCConvData);//最值确定
	if (!Trig_Init(g_FPGAData.ADCConvData))//触发设置
		return 0;
	/*判断是否等效采样-------------------------------------------------------*/
	g_OSCInfo.ClyDotNum = FPGA_SAMPL_RATE / g_OSCInfo.Freq;
	if (g_OSCInfo.ClyDotNum < EQUAL_SAMPL_GATE)
		g_OSCInfo.Sampl_Mod = eSampl_Mod_Equal;
	else
		g_OSCInfo.Sampl_Mod = eSampl_Mod_DMA;
	/*数据填充-------------------------------------------------------*/
	if (g_OSCInfo.Sampl_Mod == eSampl_Mod_Equal) {//等效采样方式
		for (i = 0 , m = 0 , n = g_OSCInfo.DataBegin; i< DataLength && i < DATAARRAYLENTGH; i++ , n++) {
			WaveArray[i] = g_FPGAData.ADCConvData[n] * g_OSCInfo.Ampli_rat * VOLTDISPCONVCOFI ;
			m += g_OSCInfo.Time_rat;
		}
	}
	else {//正常采样方式
		for (i = 0 , n = g_OSCInfo.DataBegin; i < DataLength && n < g_OSCInfo.DataEnd && i < DATAARRAYLENTGH; i++) {
			WaveArray[i] = g_FPGAData.ADCConvData[n] * g_OSCInfo.Ampli_rat * VOLTDISPCONVCOFI ;
			n += g_OSCInfo.Time_rat;
		}
	}
	g_OSCInfo.DataEnd = i;
	return 1;
}

/*===================================================================*/
/**
* @brief 触发相关
* @param
* @note
*/

/*寻找触发点---------------------------------------------------------*/
uint8_t Trigger_Found (int High, int Low) {
	static int32_t Trigger_Index = 0;
	static int32_t i = 0;
	for (i = g_OSCInfo.TRG_Pos + FOUND_SIZE; i < g_OSCInfo.DataEnd; i++) {//从FOUND_SIZE排序后的点开始寻找
		if (g_FPGAData.ADCConvData[i] > High)
			for (Trigger_Index = 1; Trigger_Index < TRIGGER_WIDTH_TOLRATE; Trigger_Index++)
				if (g_FPGAData.ADCConvData[i + Trigger_Index] < Low) {
					g_OSCInfo.DataBegin = i + Trigger_Index;
					return 1;
				}
	}
	return 0;
}

//触发设置------------------------------------------------------------------------------------------//
uint8_t Trig_Init (WAVE_TYPE Data[]) {
	static uint8_t count = 0;
	int Trigger_Value_High = 0;
	int Trigger_Value_Low = 0;
	int Trigger_Value_Centre = 0;
	int Trigger_Value_Centre_High = 0;
	int Trigger_Value_Centre_Low = 0;
	//------------------------------------------------------------------------------------------//
	switch (g_OSCInfo.Trig_Mod)//触发设置
	{
		case eTrg_Mod_Rising: {
			Trigger_Value_High = g_OSCInfo.i_MaxVal - TRIGGER_TOLRATE_EDGE;
			Trigger_Value_Low = g_OSCInfo.i_MinVal + TRIGGER_TOLRATE_EDGE;
			if (!Trigger_Found(Trigger_Value_High, Trigger_Value_Low)) {
				count++;
				if (count < 5)
					return 0;
				else
					break;
			}
			count = 0;
		}
			break;
			//------------------------------------------------------------------------------------------//
		case eTrg_Mod_Falling: {
			Trigger_Value_High = g_OSCInfo.i_MaxVal - TRIGGER_TOLRATE_EDGE;
			Trigger_Value_Low = g_OSCInfo.i_MinVal + TRIGGER_TOLRATE_EDGE;
			if (!Trigger_Found(Trigger_Value_Low, Trigger_Value_High)) {
				count++;
				if (count < 5)
					return 0;
				else
					break;
			}
			count = 0;
		}
			break;
			//------------------------------------------------------------------------------------------//
		case eTrg_Mod_Centr: {
			Trigger_Value_Centre = (g_OSCInfo.i_MaxVal + g_OSCInfo.i_MinVal) / 2;
			Trigger_Value_Centre_High = Trigger_Value_Centre + TRIGGER_CENTRE_TOLRATE;
			Trigger_Value_Centre_Low = Trigger_Value_Centre - TRIGGER_CENTRE_TOLRATE;
			if (!Trigger_Found(Trigger_Value_Centre_High, Trigger_Value_Centre_Low)) {
				count++;
				if (count < 5)
					return 0;
				else
					break;
			}
			count = 0;
		}
			break;
		default:
			break;
	}
	return 1;
}

//最值确定------------------------------------------------------------------------------------------//
void OSC_MaxMin_Found (__uIO16* Max_Value, __uIO16* Min_Value, WAVE_TYPE Data[]) {
	int i = 0, max = 0, min = 4905;
	for (i = 0; i < FOUND_SIZE; i++) {
		if (max < Data[i])
			max = Data[i];
		if (min > Data[i])
			min = Data[i];
	}
	*Min_Value = min;
	*Max_Value = max;
}

#if QUICK_SORT 
/*===================================================================*/
/**
* @brief 快速排序算法
* @param
* @note
*/
/*划分-------------------------------------------------------*/
int partition (WAVE_TYPE ai[], int low, int high) {
	int pivot = ai[low];
	while (low < high) {
		while (low < high && ai[high] >= pivot)
			high--;
		ai[low] = ai[high];
		while (low < high && ai[low] <= pivot)
			low++;
		ai[high] = ai[low];
	}
	ai[low] = pivot;
	return low;
}

/*快速排序---------------------------------------------------------*/
void Quick_Sort (WAVE_TYPE ai[], int low, int high) {
	std::stack <int> st;
	if (low < high) {
		int mid = partition(ai, low, high);
		if (low < mid - 1) {
			st.push(low);
			st.push(mid - 1);
		}
		if (mid + 1 < high) {
			st.push(mid + 1);
			st.push(high);
		}
		while (!st.empty()) {
			int q = st.top();
			st.pop();
			int p = st.top();
			st.pop();
			if (q - p < INSERT_SIZE) {
				InsertSort(ai, p, q);
				continue;
			}
			else {
				mid = partition(ai, p, q);
			}
			if (p < mid - 1) {
				st.push(p);
				st.push(mid - 1);
			}
			if (mid + 1 < q) {
				st.push(mid + 1);
				st.push(q);
			}
		}
	}
}
#endif

