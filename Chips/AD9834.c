#include "AD9834.h"

/****************************************************************
��������: AD9834_Write_16Bits
��    ��: ��AD9834д��16Ϊ����
��    ��: data --  Ҫд���16λ����
����ֵ  : ��
*****************************************************************/
void AD9834_Write_16Bits(unsigned int data)
{
    unsigned char i = 0 ;

    AD9834_SCLK_SET ;
    AD9834_FSYNC_CLR ;
    for (i=0 ;i<16 ;i++)
    {
        if (data & 0x8000)
            AD9834_SDATA_SET ;
        else
            AD9834_SDATA_CLR ;

        AD9834_SCLK_CLR ;
        data <<= 1 ;
        AD9834_SCLK_SET ;
    }

    AD9834_FSYNC_SET ;
}
/***********************************************************************************
�������ƣ�AD9834_Select_Wave
��    �ܣ����Ϊ���ƣ�
    --------------------------------------------------
    IOUT���Ҳ� ��SIGNBITOUT���� ��дFREQREG0 ��дPHASE0
    ad9834_write_16bit(0x2028)   һ����дFREQREG0
    ad9834_write_16bit(0x0038)   ������дFREQREG0��LSB
    ad9834_write_16bit(0x1038)   ������дFREQREG0��MSB
    --------------------------------------------------
    IOUT���ǲ� ��дPHASE0
    ad9834_write_16bit(0x2002)   һ����дFREQREG0
    ad9834_write_16bit(0x0002)   ������дFREQREG0��LSB
    ad9834_write_16bit(0x1008)   ������дFREQREG0��MSB
��    ����initdata -- Ҫ���������
����ֵ  ����
************************************************************************************/
void AD9834_Select_Wave(unsigned int initdata)
{
//    Init_AD9834();
    AD9834_FSYNC_SET;
    AD9834_SCLK_SET;

    AD9834_RESET_SET;
    AD9834_RESET_SET;
    AD9834_RESET_CLR;

    AD9834_Write_16Bits(initdata);
}
void AD9834_Triangular_Wave()
{
//    Init_AD9834();
    AD9834_FSYNC_SET;
    AD9834_SCLK_SET;

    AD9834_RESET_SET;
    AD9834_RESET_SET;
    AD9834_RESET_CLR;

    AD9834_Write_16Bits(0x0002);
    AD9834_Write_16Bits(0x1008);
}
/****************************************************************
��������: AD9834_Set_Freq
��    ��: ����Ƶ��ֵ
��    ��: freq_number -- Ҫд���ƽ�ʼĴ���(FREQ_0��FREQ_1)
          freq -- Ƶ��ֵ (Freq_value(value)=Freq_data(data)*FCLK/2^28)
����ֵ  : ��
*****************************************************************/
void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq)
{
    unsigned long FREQREG = (unsigned long)(268435456.0/AD9834_SYSTEM_COLCK*freq);

    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG;
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG>>14);
//    Init_AD9834();
    if (freq_number == FREQ_0)
    {
        FREQREG_LSB_14BIT &= ~(1U<<15);
        FREQREG_LSB_14BIT |= 1<<14;
        FREQREG_MSB_14BIT &= ~(1U<<15);
        FREQREG_MSB_14BIT |= 1<<14;
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1<<14);
        FREQREG_LSB_14BIT |= 1U<<15;
        FREQREG_MSB_14BIT &= ~(1<<14);
        FREQREG_MSB_14BIT |= 1U<<15;
    }

    AD9834_Write_16Bits(FREQREG_LSB_14BIT);
    AD9834_Write_16Bits(FREQREG_MSB_14BIT);

}
//============================================================//
/*   ע��ʹ��ʱ�ȵ��ó�ʼ��������ʼ��IO���ã�Ȼ�����AD9834_Select_Wave()ѡ���Σ����ѡ��Ƶ��ֵ��������� */
