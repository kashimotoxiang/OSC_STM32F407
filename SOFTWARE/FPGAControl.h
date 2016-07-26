#pragma once

#include "main.h"

//FPGA并口引脚
#define  FPGA_PARDATATRANSPORT	GPIOC
#define  FPGA_PARDATATRANSMASK	GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11

//FPGA延时
#define FPGAWAITTIMES	     	5	//FPGA并口等待时间
#define FPGASTATUSTIMES       10		//FPGA状态转移时间
#define UPSAMPLEAVETIMES	  100		//上升沿最高采样次数


//FGPA采样时钟控制裕度（单位：时钟数）
#define FPGACLKMARGIN		1

//FPGA延迟采样比例因子
#define FPGADELAYRATIO 0.93

//FPGA控制线1
#define FPGA_COM1_SET		FPGA_COM1_GPIO_Port->ODR |=  FPGA_COM1_Pin
#define FPGA_COM1_CLR		FPGA_COM1_GPIO_Port->ODR &=~ FPGA_COM1_Pin

//FPGA控制线2
#define FPGA_COM2_SET		FPGA_COM2_GPIO_Port->ODR |=  FPGA_COM2_Pin
#define FPGA_COM2_CLR		FPGA_COM2_GPIO_Port->ODR &=~ FPGA_COM2_Pin

//FPGA时钟线控制
#define FPGA_SCK_SET		FPGA_SCK_GPIO_Port->ODR|=FPGA_SCK_Pin
#define FPGA_SCK_CLEAR		FPGA_SCK_GPIO_Port->ODR&=~FPGA_SCK_Pin

//转化数据大小
#define FPGADATALENTGH128		0x00
#define FPGADATALENTGH2048		0xFF

void FPGA_ParallDataTrans (WAVE_TYPE* WaveArray, uint16_t length);
void FPGA_GetMeasurePar (void);
void FPGA_WritePar (uint32_t m_SamplFreq, uint16_t m_HighThres, uint16_t m_LowThres, uint8_t LengthCom, uint8_t m_AddPeriod, FPGAControl_struct* FPGAPar);
void FPGAConterSet (FPGAControl_struct* FPGACon);
void FPGA_AmplitudeCompute (FPGAData_struct* FPGAData);
void FPGA_UpTimeCompute (FPGAData_struct* FPGAData, FPGAControl_struct* FPGACon);
void FPGASTATION1 (void);
void FPGASTATION2 (void);
void FPGASTATION3 (void);

