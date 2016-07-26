#pragma once
/*  Usual Macro---------------------------------------------------------*/
#define  COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))//����ʱ���У���������֪����
#define  MAX( x_cur, y_cur )		( ((x_cur) > (y_cur))  (x_cur) : (y_cur) )//��ֵ
#define  MIN( x_cur, y_cur )		( ((x_cur) < (y_cur))  (x_cur) : (y_cur) )//Сֵ
#define  UPCASE( c )		( ((c) >= 'a' && (c) <= 'z')  ((c) - 0x20) : (c) )//ת��Ϊ��д
#define  ARR_SIZE( a )		( sizeof( (a) ) / sizeof( (a[0]) ) )//���鳤�Ȼ�ȡ
#define  INC_SAT( val )		(val = ((val)+1 > (val)) ? (val)+1 : (val))//��ֹ���
#define  ABS(x_cur)				( (x_cur)>0(x_cur):-(x_cur) ) //�����;���ֵ
#define  uDiffABS(x_cur,y_cur)			( (x_cur>y_cur)?(x_cur-y_cur):(y_cur-x_cur) ) //���޷����͵Ĳ�ľ���ֵ
#define  Safe_Return(Current, Begin, End) (((Current)>(End)?(Begin):((Current)<(Begin)?(End):(Current))))//��ȫ���أ���ֹ�������
#define  isBound(x,MIN,MAX)       ((x)>(MIN)&&(x)<(MAX))
/*-------------------------------------------------------*/
#include "main.h"
/*  ---------------------------------------------------------*/
void Bubble_Sort (uint8_t volatile* num, int n);
void Num_Show (long int data, char* com, u16 x, u16 y);
void InsertSort (WAVE_TYPE ai[], int low, int high);
void Swap (WAVE_TYPE ai[], int x_cur, int y_cur);

