#include "AD9852.h"

uint8_t AD9852_ControlReg[4]={0,0,0,0};

void AD9852_GPIO_Config();
uint8_t AD9852_R(uint8_t _data);


void AD9852_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	//addr(5:0)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_InitStructure.GPIO_Pin );
	//DDSRES ,DDSWR
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_12 );

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8 );
	//data(7:0)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_InitStructure.GPIO_Pin );	
}

uint8_t AD9852_R(uint8_t _data)
{
	uint8_t i=0;
	uint8_t tmp=0;
	for(i=0;i<8;i++)
	{
		tmp>>=1;
		if(_data&0x80)
		{
		 	tmp|=0x80;
		}
		_data<<=1;
	} 
	return tmp;	
}



void AD9852_WriteByte(uint8_t _addr,uint8_t _data)
{
	GPIO_Write(GPIOE,GPIO_ReadOutputData(GPIOE)&0xFFC0|(_addr&0x003f));
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);	  //DDSWR=0
	GPIO_Write(GPIOD,GPIO_ReadOutputData(GPIOD)&0xFF00|_data);	
	GPIO_SetBits(GPIOA,GPIO_Pin_12);	  //DDSWR=1
}
void AD9852_MasterReset()
{
//	Delay(0xFFFF);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);	      //DDSRES=1
	Delay(10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);	  //DDSRES=0
}	  

/*
void Delay(vu32 Time)
{
	for(; Time != 0; Time--);
}
			  */

void AD9852_Init()
{
	
	//AD9852_GPIO_Config();


	AD9852_MasterReset();

	//AD9852_SetFrequency1(52776558130000);	 	 //30MHz
	AD9852_SetFrequency1(527765581300);	 	 //30kHz
	//AD9852_SetFrequency1(527765581300);	 	 //30kHz

	//AD9852_ControlDAC(200);
	AD9852_SetAmplitude(367);
	//AD9852_SetFrequency2(3518437200);	 //5kHz
	//AD9852_SetDeltaFrequency(100); 	 	 //100Hz per step
	//AD9852_SetUpdataClock(9999999);
	//AD9852_RampRate(0x01);
	AD9852_SetPhase1(0);	 
									  
#ifdef AD9852_CTRL_CONTROLDAC_PD
	AD9852_ControlReg[0]|=0x04;
#endif	
#ifdef AD9852_CTRL_ALLDAC_PD
	AD9852_ControlReg[0]|=0x02;
#endif

	AD9852_ControlReg[1]=0x1F & AD9852_CTRL_PLL;
#ifdef AD9852_CTRL_PLL_RANGE_HIGH
	AD9852_ControlReg[1]|=0x40;
#endif
#ifdef AD9852_CTRL_PLL_BYPASS
	AD9852_ControlReg[1]|=0x20;
#endif

	AD9852_ControlReg[2]=(AD9852_CTRL_MODE<<1);

#ifdef AD9852_CTRL_TRIANGLE
	AD9852_ControlReg[2]|=0x20;
#endif
#ifdef AD9852_CTRL_INTCLK
	AD9852_ControlReg[2]|=0x01;
#endif
#ifdef AD9852_CTRL_BYPASS_INV_SINC
	AD9852_ControlReg[3]|=0x40;
#endif
#ifdef AD9852_CTRL_OSK_EN
	AD9852_ControlReg[3]|=0x20;
#endif
#ifdef AD9852_CTRL_OSK_INT
	AD9852_ControlReg[3]|=0x10;
#endif
	AD9852_WriteByte(0x1D,AD9852_ControlReg[0]);
	AD9852_WriteByte(0x1E,AD9852_ControlReg[1]);
	AD9852_WriteByte(0x1F,AD9852_ControlReg[2]);
	AD9852_WriteByte(0x20,AD9852_ControlReg[3]); 
	

	//AD9852_SetCLRACC1(1);	

	  
}

void AD9852_SetFrequency1(uint64_t _frequency)			   //调频
{
	AD9852_WriteByte(0x04,(_frequency>>40)&0xFF);			
	AD9852_WriteByte(0x05,(_frequency>>32)&0xFF);			
	AD9852_WriteByte(0x06,(_frequency>>24)&0xFF);			
	AD9852_WriteByte(0x07,(_frequency>>16)&0xFF);			
	AD9852_WriteByte(0x08,(_frequency>>8)&0xFF);	
	AD9852_WriteByte(0x09,(_frequency)&0xFF);					
}
void AD9852_SetFrequency2(uint64_t _frequency)			   //调频
{
	AD9852_WriteByte(0x0A,(_frequency>>40)&0xFF);			
	AD9852_WriteByte(0x0B,(_frequency>>32)&0xFF);			
	AD9852_WriteByte(0x0C,(_frequency>>24)&0xFF);			
	AD9852_WriteByte(0x0D,(_frequency>>16)&0xFF);			
	AD9852_WriteByte(0x0E,(_frequency>>8)&0xFF);	
	AD9852_WriteByte(0x0F,(_frequency)&0xFF);					
}

void AD9852_SetAmplitude(uint16_t _value)				//调幅
{
	AD9852_WriteByte(0x21,(_value>>8)&0xFF);		
	AD9852_WriteByte(0x22,(_value)&0xFF);					
}
void AD9852_SetPhase1(uint16_t _value)					//调相位1
{
	AD9852_WriteByte(0x00,(_value>>8)&0xFF);			
	AD9852_WriteByte(0x01,(_value)&0xFF);				
}
void AD9852_SetPhase2(uint16_t _value)					//调相位2
{
	AD9852_WriteByte(0x02,(_value>>8)&0xFF);		
	AD9852_WriteByte(0x03,(_value)&0xFF);			
}
void AD9852_SetDeltaFrequency(int64_t _frequency)		//步进频率
{
	AD9852_WriteByte(0x10,(_frequency>>40)&0xFF);			
	AD9852_WriteByte(0x11,(_frequency>>32)&0xFF);			
	AD9852_WriteByte(0x12,(_frequency>>24)&0xFF);			
	AD9852_WriteByte(0x13,(_frequency>>16)&0xFF);			
	AD9852_WriteByte(0x14,(_frequency>>8)&0xFF);	
	AD9852_WriteByte(0x15,(_frequency)&0xFF);
}
void AD9852_SetUpdataClock(uint64_t _value)
{
	AD9852_WriteByte(0x16,(_value>>24)&0xFF);			
	AD9852_WriteByte(0x17,(_value>>16)&0xFF);			
	AD9852_WriteByte(0x18,(_value>>8)&0xFF);	
	AD9852_WriteByte(0x19,(_value)&0xFF);
}
void AD9852_RampRate(uint64_t _value)
{
	AD9852_WriteByte(0x1A,(_value>>16)&0xFF);			
	AD9852_WriteByte(0x1B,(_value>>8)&0xFF);
	AD9852_WriteByte(0x1C,(_value)&0xFF);
}

void AD9852_ControlDAC(int16_t _value)
{
	AD9852_WriteByte(0x26,(_value>>8)&0xFF);			
	AD9852_WriteByte(0x27,(_value)&0xFF);	
}

void AD9852_SetCLRACC1(uint8_t _bit)
{
	if(_bit)
 		AD9852_ControlReg[2]=AD9852_ControlReg[2]|0x80;
	else
		AD9852_ControlReg[2]=AD9852_ControlReg[2]&~0x80;		
	AD9852_WriteByte(0x1F,AD9852_ControlReg[2]);		
}
void AD9852_SetCLRACC2(uint8_t _bit)
{
	if(_bit)
 		AD9852_ControlReg[2]=AD9852_ControlReg[2]|0x40;
	else
		AD9852_ControlReg[2]=AD9852_ControlReg[2]&~0x40;		
	AD9852_WriteByte(0x1F,AD9852_ControlReg[2]);		
}