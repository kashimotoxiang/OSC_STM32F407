#include "Usual.h"
#include <cstdarg>
#include <stdarg.h>
/*-------------------------------------------------------*/
char TemptString[10] = {0};//数字字符串转换数组
//---------------------------------------------------------------------//
inline void Bubble_Sort (uint8_t volatile* num, int n) {
	static int32_t i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; i + j < n - 1; j++) {
			if (num[j] > num[j + 1]) {
				uint8_t temp = num[j];
				num[j] = num[j + 1];
				num[j + 1] = temp;
			}
		}
	}
}

/*数字显示-------------------------------------------------------*/
inline void Num_Show (long int data, char* com, u16 x, u16 y) {
	sprintf(TemptString, com, data);
	ESP_LCD_ShowString(x, y, TemptString);
}

/*插入排序---------------------------------------------------------*/
inline void InsertSort (WAVE_TYPE ai[], int low, int high) {
	int i, j, key;
	for (i = low + 1; i <= high; i++)//控制需要插入的元素
	{
		key = ai[i]; //key为要插入的元素
		for (j = i; j > low && ai[j - 1] > key; j--) //查找要插入的位置,循环结束,则找到插入位置
		{
			ai[j] = ai[j - 1]; //移动元素的位置.供要插入元素使用
		}
		ai[j] = key; //插入需要插入的元素
	}
}

/*交换---------------------------------------------------------*/
inline void Swap (WAVE_TYPE ai[], int x_cur, int y_cur) {
	WAVE_TYPE temp = ai[x_cur];
	ai[x_cur] = ai[y_cur];
	ai[y_cur] = temp;
}

