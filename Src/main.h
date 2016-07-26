#pragma once
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "arm_math.h"
#include "stdint.h"
#include "mxconstants.h"
#include "GUI.h"
#include "core_cm4.h"
#include "TypeDefine.h"
#include "ChipsDefine.h"
/* Private define ------------------------------------------------------------*/
//�������ݴ�С
#define AMPLITITUDEDATALENGTH		128
#define UPTIMEDATALENGTH			1000
#define WAVEDATALENGTH				1000

//��ʾ������
#define  TFT_WIDTH           640//������ʾ���
#define  TFT_BEGINNING   MEMORYE_DEPTH/4//������ʾ��ʼ

//�洢���
#define  MEMORYE_DEPTH       2048//�洢���
#define  MEMORYETEXT_DEPTH       100//���Դ洢���//���������

#define  TRIGGER_TOLRATE_EDGE             50
#define  TRIGGER_WIDTH_TOLRATE       10
#define  TRIGGER_CENTRE_TOLRATE    1
#define  SAMPLING_SPEED       4
#define  MAXMIN_ARRAY_SIZE        20
#define  FOUND_SIZE 300
#define  VOLTCONVCOFFI         3300/255.0     //��ѹת��ϵ��
#define  VOLTDISPCONVCOFI         VOLTCONVCOFFI/10/3000.0     //��ѹ��ʾת��ϵ��//AMLPLIFY�Ͻ�Ϊ5��Ϊʵ��α��������Ϊ10000����Ҫ����2000��ԭ�Ͻ�

/*-----------------------���ܲ鿴ģʽ--------------------------------*/

//#define DEBUG__				 //�Ƿ�������ģʽ
//#define MEM_DEBUG__			 //�ڴ����ģʽ
#define SINGLE_PAGE__		    //ʹ�õ�ҳ����
//#define ADCTAKESAMPLE__		 //ADC����ģʽ�������
//#define PARALLELDATA__		//�������ݴ���

/*----------------------���ܲü�---------------------------------*/

#define TRIBLE_ADC 0
#define QUICK_SORT 0

/*-------------------------------------------------------*/


/*=========================�ṹ�嶨��==============================*/

/* ADC ----------------------------------------------------------*/
typedef struct FPGAData_struct {
	WAVE_TYPE AmlifMeasureData[AMPLITITUDEDATALENGTH];//��ֵ������������
	WAVE_TYPE UpTimeData[UPTIMEDATALENGTH];//����ʱ���������
	WAVE_TYPE DispData[WAVEDATALENGTH];//������ʾ����
	double SignalFreq;
	double Amplitude;
	double DutyCycle;
	double UpTime;
	double DownTime;
	uint16_t i_AmplitudeMax;
	uint16_t i_AmplitudeMin;
	uint16_t i_Amplitude;
	double f_AmplitudeMax;
	double f_AmplitudeMin;
	double f_Amplitude;
	double TimeSpace;//����������ʱ����
} FPGAData_struct;

typedef struct FPGAControl_struct {
	/*FGPA��������-------------------------------------------------------*/
	double FPGA_Freq;
	/*���������(Amplitude)-------------------------------------------------------*/
	double SamplFreq;//ָ������Ƶ��
	uint32_t HighTimes;//�ߵ�ƽ����ʱ��
	uint32_t LowTimes;//�͵�ƽ����ʱ��
	uint16_t HighThreshold;//����ֵ
	uint16_t LowThreshold;//����ֵ
	uint8_t DataLengthCOM;//��������
	uint16_t SamplDalayTimes;//�����ӳ�ʱ��
} FPGAControl_struct;

/* FMS ---------------------------------------------------------*/
typedef struct FSM_struct//FMS
{
	__uIO8 WorkS;
	__uIO8 Old_WorkS;
	__uIO8 MainS;
	__uIO8 Old_MainS;
} FSM_struct;

/* OSC ---------------------------------------------------------*/
typedef struct OSC_struct//OSC
{
	__uIO8 Trig_Mod;
	__uIO8 Sampl_Mod;
	double Time_rat;
	double Ampli_rat;
	__uIO32 TRG_Pos;
	__uIO16 i_MaxVal;
	__uIO16 i_MinVal;
	float f_MaxVal;
	float f_MinVal;
	__uIO32 Freq;
	__uIO32 DataBegin;
	__uIO32 DataEnd;
	__uIO8 ClyDotNum;
	u8 MeasureStatu;
} OSC_struct;

/* GUIControl ---------------------------------------------------------*/
typedef struct GUIControl_struct//OSC
{
	__uIO8 GraphDispState;
	__uIO8 MeasureState;
	__uIO8 NumpadState;
	__uIO8 ConStState;
	__uIO8 BSP_DispState;//BSP��ʾ����
} GUIControl_struct;

/* LCD---------------------------------------------------------*/
typedef struct Page_struct//LCD
{
	__uIO8 One;//��ʾҳ
	__uIO8 Two;//����ҳ
	__uIO8 Temp;//����ҳ����
} Page_struct;

/* Key---------------------------------------------------------*/
typedef struct Key_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Key_struct;

/* Key---------------------------------------------------------*/
typedef struct Index_struct//Key
{
	__uIO8 Cur;
	__uIO8 Last;
} Index_struct;

/*����������-------------------------------------------------------*/
typedef struct SenseData_struct//Key
{
	__uIO32 ADS7883;
	__uIO32 ADS1110;
	__uIO32 ADF4002;
	__uIO32 AD9852;
} SenseData_struct;

/*�������Ʋ���-------------------------------------------------------*/
typedef struct DevicePar_struct {
	uint32_t AD9834Wava;
	uint32_t AD9834Freq;
	double AD9834Duty;
	uint32_t AD9834Freq_l;
	uint32_t AD9834Duty_l;
} DevicePar_struct;


/*=======================================================*/
extern FPGAData_struct g_FPGAData;
extern FSM_struct g_FSM;
extern OSC_struct g_OSCInfo;
extern Page_struct g_DispPage;
extern Key_struct g_Key;
extern GUIControl_struct g_GUICon;
extern SenseData_struct g_Sense;//����������
extern DevicePar_struct g_Device;
extern WAVE_TYPE WaveArray[TFT_WIDTH ];

extern void Error_Handler (void);
/* Includes ------------------------------------------------------------------*/

/*Ӳ��-------------------------------------------------------*/
#include "System.h"
#include "mySPI.h"
#include "myADC.h"
#include "myUART.h"
#include "LCD_MD050SD.h"
#include "ChipsDefine.h"
/*���-------------------------------------------------------*/
#include "FPGAControl.h"
#include "Usual.h"
#include "FSM.h"
#include "OSC_Work.h"
#include "FFT.h"
/*emWin-------------------------------------------------------*/
#include "GUIDefine.h"
/*-------------------------------------------------------*/
#ifdef DEBUG__
#include "DebugSupport.h"
#endif

#ifdef MEM_DEBUG__
#include "GUIUsual.h"
#endif

/*assert���-------------------------------------------------------*/
#if MEMORYE_DEPTH -TFT_WIDTH <0
#pragma message("������ȹ�С����")  
#error
#endif 
/*-------------------------------------------------------*/


