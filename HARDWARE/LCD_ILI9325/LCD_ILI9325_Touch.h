#pragma once
/* Includes ------------------------------------------------------------------*/
#include "LCD_ILI9325.h"

/* touch panel interface define */
#define  BSP_T_CMD_RDX  0xD0	 //����IC�����������
#define  BSP_T_CMD_RDY  0x90
//�������Ŷ���------------------------------------------------------------------------------------------//
#define	BSP_T_CLK_High	      BSP_T_CLK_GPIO_Port->ODR|=BSP_T_CLK_Pin    //����/����      		PA11
#define	BSP_T_CS_High	      BSP_T_CS_GPIO_Port->ODR|=BSP_T_CS_Pin    //д����				PA12
#define	BSP_T_DIN_High	      BSP_T_DIN_GPIO_Port->ODR|=BSP_T_DIN_Pin   //������			 	PA13

#define	BSP_T_CLK_Low		  BSP_T_CLK_GPIO_Port->ODR&=~BSP_T_CLK_Pin     //Ƭѡ�˿�  	     	PA11
#define	BSP_T_CS_Low	      BSP_T_CS_GPIO_Port->ODR&=~BSP_T_CS_Pin     //������			 	PA12
#define	BSP_T_DIN_Low	      BSP_T_DIN_GPIO_Port->ODR&=~BSP_T_DIN_Pin    //д����			 	PA13

#define BSP_T_IRQ_Dect		  ((BSP_T_IRQ_GPIO_Port->IDR)&BSP_T_IRQ_Pin)
#define BSP_T_DOUT_Dect    �� ((BSP_T_DOUT_GPIO_Port->IDR)&BSP_T_DOUT_Pin)

/*-------------------------------------------------------*/
extern T_Data_Struct BSP_tp_pixad, BSP_TS; //��ǰ���������ADֵ,ǰ�������������ֵ
extern u16 BSP_vx, BSP_vy; //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 BSP_chx, BSP_chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ

void BSP_TS_Init (void);
void BSP_T_Adjust (void);
void BSP_T_point (void); //��ͼ����

u16 BSP_TP_Read_XOY (u8 xy);//SPI ���Ĵ����汾���շ�����
u8 BSP_Convert_Pos (void);
void BSP_Pointer_Update (void);

