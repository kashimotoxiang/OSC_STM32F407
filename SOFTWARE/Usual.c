#include "Usual.h"
#include <cstdarg>
#include <stdarg.h>
/*-------------------------------------------------------*/
char TemptString[10] = {0};//�����ַ���ת������
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

/*������ʾ-------------------------------------------------------*/
inline void Num_Show (long int data, char* com, u16 x, u16 y) {
	sprintf(TemptString, com, data);
	ESP_LCD_ShowString(x, y, TemptString);
}

/*��������---------------------------------------------------------*/
inline void InsertSort (WAVE_TYPE ai[], int low, int high) {
	int i, j, key;
	for (i = low + 1; i <= high; i++)//������Ҫ�����Ԫ��
	{
		key = ai[i]; //keyΪҪ�����Ԫ��
		for (j = i; j > low && ai[j - 1] > key; j--) //����Ҫ�����λ��,ѭ������,���ҵ�����λ��
		{
			ai[j] = ai[j - 1]; //�ƶ�Ԫ�ص�λ��.��Ҫ����Ԫ��ʹ��
		}
		ai[j] = key; //������Ҫ�����Ԫ��
	}
}

/*����---------------------------------------------------------*/
inline void Swap (WAVE_TYPE ai[], int x_cur, int y_cur) {
	WAVE_TYPE temp = ai[x_cur];
	ai[x_cur] = ai[y_cur];
	ai[y_cur] = temp;
}

