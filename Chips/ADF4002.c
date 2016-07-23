#include"ADF4002.h"

uint32_t register_r=0;
uint32_t register_n=0;

uint64_t R_register_slop=100000;

void ADF4002_Init()
{
	register_r=CTRL_R_COUNTER|RCOUNTER_ABP_2_9_ns;
	register_n=CTRL_N_COUNTER|NCOUNTER_G1;

	//ADF4002_WriteLatch(CTRL_INIT_LATCH|INIT_PD_DISABLE|INIT_CPI2(0)|INIT_CPI1(0)|INIT_TC(0)|INIT_FASTLOCK_DISABLE|INIT_MUX_N_DIVIDER);
	//ADF4002_WriteLatch(CTRL_FUNC_LATCH|INIT_PD_DISABLE|INIT_CPI2(0)|INIT_CPI1(0)|INIT_TC(0)|INIT_FASTLOCK_DISABLE|INIT_MUX_N_DIVIDER);
	//ADF4002_WriteLatch(CTRL_INIT_LATCH|0x1C2680);
	//ADF4002_WriteLatch(CTRL_FUNC_LATCH|0x1C2680);
	
	ADF4002_WriteLatch(0x1C26A3);
	ADF4002_WriteLatch(0x1C26A2);  

	ADF4002_SetRCounter(100);
	ADF4002_SetNCounter(2010);	  //2010 TO 2410
			   		
}
void ADF4002_SetRCounter(uint16_t _num)
{
	ADF4002_WriteLatch(register_r|((0x3FFF&_num)<<2));
	switch(_num)
	{
	case 100:
		R_register_slop=100000;
	break;
	case 200:
		R_register_slop=50000;
	break;
	case 400:
		R_register_slop=25000;
	break;
	}
}
void ADF4002_SetNCounter(uint16_t _num)
{
	ADF4002_WriteLatch(register_n|((0x1FFF&_num)<<8));
}
void ADF4002_WriteLatch(uint32_t _register)
{
	 uint8_t i;
	 ADF4002_SCK_0;
	 Delay(0x0F);
	 ADF4002_LE_0;
	 Delay(0x0F);
	 for(i=0;i<24;i++)
	 {
	 	
	 	if(_register&0x800000)
		{
			ADF4002_SDA_1;
		}
		else
		{
			ADF4002_SDA_0;
		}
			 Delay(0x0F);
		ADF4002_SCK_1;
			 Delay(0x0F);	
	 	ADF4002_SCK_0;
		_register<<=1;
			 Delay(0x0F);
	 }
	 ADF4002_LE_1;
	 	 Delay(0x0F);	
	 ADF4002_SCK_1;
	 	 Delay(0x0F);
}

void ADF4002_SetFrequency(uint64_t _freq)
{
	uint16_t tt;
	tt =_freq/100000;
	ADF4002_SetNCounter(tt);
	//LCD_PrintNum(200,100,tt);
}