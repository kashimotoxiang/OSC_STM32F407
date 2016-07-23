#include "ChipsDefine.h"

#define AD9852_CTRL_PLL 4     //PLL±¶ÆµµÄ±¶Êý£¬È¡4~20

//------MODE--------
#define	AD9852_CTRL_MODE_SINGLE_TONE   0x00
#define	AD9852_CTRL_MODE_FSK		   0x01
#define	AD9852_CTRL_MODE_RAMPED_FSK	   0x02
#define	AD9852_CTRL_MODE_CHIRP		   0x03
#define	AD9852_CTRL_MODE_BPSK		   0x04

#define AD9852_CTRL_MODE AD9852_CTRL_MODE_SINGLE_TONE
//-------Set----------
//#define AD9852_CTRL_CONTROLDAC_PD
//#define AD9852_CTRL_ALLDAC_PD
#define AD9852_CTRL_PLL_RANGE_HIGH
//#define AD9852_CTRL_PLL_BYPASS
//#define AD9852_CTRL_TRIANGLE
#define AD9852_CTRL_INTCLK
//#define AD9852_CTRL_BYPASS_INV_SINC
#define AD9852_CTRL_OSK_EN
//#define AD9852_CTRL_OSK_INT

//------------------
void AD9852_Init();
void AD9852_WriteByte(uint8_t _addr,uint8_t _data);
void AD9852_SetFrequency1(uint64_t _Frequency);
void AD9852_SetFrequency2(uint64_t _Frequency);
void AD9852_SetAmplitude(uint16_t _value);
void AD9852_SetPhase1(uint16_t _value);
void AD9852_SetPhase2(uint16_t _value);
void AD9852_SetDeltaFrequency(int64_t _Frequency);
void AD9852_RampRate(uint64_t _value);
void AD9852_SetUpdataClock(uint64_t _value);
void AD9852_ControlDAC(int16_t _value);
void AD9852_SetCLRACC1(uint8_t _bit);
void AD9852_SetCLRACC2(uint8_t _bit);


