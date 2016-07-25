#pragma once
#include <core_cm4.h>

/*=======================���Ͷ���================================*/
typedef volatile unsigned char __uIO8;
typedef volatile unsigned short int __uIO16;
typedef volatile unsigned int __uIO32;

typedef volatile signed char __IO8;
typedef volatile signed short int __IO16;
typedef volatile signed int __IO32;

typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef unsigned short int u32_t;

typedef u8_t u8;
typedef u16_t u16;
typedef u32_t u32;

typedef __IO
unsigned int vu32;
typedef __IO
unsigned short int vu16;
typedef __IO
unsigned char vu8;

#define   WAVE_TYPE             __uIO8
#define   TFT_DISPLAY_TYPE      __uIO16
#define   SPI_TYPE              uint8_t 
#define   UART_TYPE				char
#define   WAVE_TYPE				I16

/*=========================״̬����==============================*/
//ע���ʼ״ֵ̬һ��Ҫ����1��������������������������������

/*-----------------------��״̬--------------------------------*/
typedef enum {
	eFSM_MainS_Emp = 0x01,//��ʼ״̬����1
	eFSM_MainS_OSC,
} FSM_MainS_;//��״̬

/*-----------------------����״̬--------------------------------*/
typedef enum {
	eFSM_WorkS_Emp = 0x01,//��ʼ״̬����1
	eFSM_WorkS_TriADC,
} FSM_WorkS_;//����״̬

/*-----------------------����ģʽ--------------------------------*/
typedef enum {
	eTrg_Mod_Begin = 0x01,//��ʼ״̬����1
	eTrg_Mod_Rising = eTrg_Mod_Begin,
	eTrg_Mod_Falling,
	eTrg_Mod_Centr,
	eTrg_Mod_End,
} Trg_Mod_;//����ģʽ

/*-----------------------������ʽ--------------------------------*/
typedef enum {
	eSampl_Mod_DMA = 0x01,//��ʼ״̬����1
	eSampl_Mod_Timer,
	eSampl_Mod_Equal,
} Sampl_Mod_;//������ʽ

/*------------------------ҳ��-------------------------------*/
typedef enum {
	ePage_Num_Begin = 0x01,//��ʼ״̬����1
	ePage_Num_1 = ePage_Num_Begin,
	ePage_Num_2,
	ePage_Num_3,
	ePage_Num_4,
	ePage_Num_5,
	ePage_Num_6,
	ePAGE_End = ePage_Num_6,
} Page_Num_;//ҳ��

/*-------------------------�ֱ���------------------------------*/
typedef enum {
	eReslt_rat_Begin = 0x01,//��ʼ״̬����1
	eReslt_rat_0 = eReslt_rat_Begin,
	eReslt_rat_1,
	eReslt_rat_2,
	eReslt_rat_3,
	eReslt_rat_4,
	eReslt_rat_5,
	eReslt_rat_6,
	eReslt_rat_7,
	eReslt_rat_8,
	eReslt_rat_9,
	eReslt_rat_10,
	eReslt_rat_11,
	eReslt_rat_End = eReslt_rat_11,
} Reslt_rat_;//�ֱ���

/*---------------------����������״̬----------------------------------*/
typedef enum {
	//��ʼ״̬����1
	eKey_Empty = 0x01,
	eSampl_rat_ADD,
	eReslt_rat_ADD,
	eReslt_rat_SUB,
	eTRGPosLine_ADD,
	eTRGPosLine_SUB,
	eTRG_Mod_ADD,
	eTRG_Mod_SUB,
} Key_State_;//����������״̬

/*------------------------u16�߽���-------------------------------*/
typedef enum {
	eu16_BEGIN = 0,
	eu16_END = 65535,
} u16_BEGIN_;//

/*------------------------����-------------------------------*/
typedef enum {
	eClose = 0,
	eOpen = 1,
} Switch_;//

/*------------------------LCD��������-------------------------------*/
typedef enum {
	eLCDPar_width = 800, //LCD ���
	eLCDPar_height = 480, //LCD �߶�
	eLCDPar_dir = 1, //���������������ƣ�0��������1��������	
	eLCDPar_wramcmd = 0x0F, //��ʼдgramָ��
	eLCDPar_setxcmd = 0x02, //����x����ָ��
	eLCDPar_setycmd = 0x03, //����y����ָ�� 
} LCD_Par_;

/*------------------------LCD�豸-------------------------------*/
typedef enum {
	eLAYER_BSP = 0,
	eLAYER_ESP = 1,
} LAYER_Par_;

/*------------------------GUI������-------------------------------*/
typedef enum {
	eNumpadInput = 0,
} GUI_CONTROL_;//

/*------------------------SPI������-------------------------------*/
typedef enum {
	eS_SendFreq = 0x0000,
	eS_SendData = 0xFFFF,
} SPI_CONTROL_;//

/*-------------------------��ʾ�������ж�------------------------------*/
typedef enum {
	eISEMPTY = 0,
	eISNOTEMPRT,
} IsDispEmpty_;//��ʾ�����Ƿ�Ϊ�գ�������ʾ��ͻ


