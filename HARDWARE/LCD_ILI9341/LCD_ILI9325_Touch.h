#pragma once
/* Includes ------------------------------------------------------------------*/
#include "LCD_ILI9325.h"

/* touch panel interface define */
#define  BSP_T_CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  BSP_T_CMD_RDY  0x90
//触摸引脚定义------------------------------------------------------------------------------------------//
#define	BSP_T_CLK_High	      BSP_T_CLK_GPIO_Port->ODR|=BSP_T_CLK_Pin    //数据/命令      		PA11
#define	BSP_T_CS_High	      BSP_T_CS_GPIO_Port->ODR|=BSP_T_CS_Pin    //写数据				PA12
#define	BSP_T_DIN_High	      BSP_T_DIN_GPIO_Port->ODR|=BSP_T_DIN_Pin   //读数据			 	PA13

#define	BSP_T_CLK_Low		  BSP_T_CLK_GPIO_Port->ODR&=~BSP_T_CLK_Pin     //片选端口  	     	PA11
#define	BSP_T_CS_Low	      BSP_T_CS_GPIO_Port->ODR&=~BSP_T_CS_Pin     //读数据			 	PA12
#define	BSP_T_DIN_Low	      BSP_T_DIN_GPIO_Port->ODR&=~BSP_T_DIN_Pin    //写数据			 	PA13

#define BSP_T_IRQ_Dect		  ((BSP_T_IRQ_GPIO_Port->IDR)&BSP_T_IRQ_Pin)
#define BSP_T_DOUT_Dect    　 ((BSP_T_DOUT_GPIO_Port->IDR)&BSP_T_DOUT_Pin)

/*-------------------------------------------------------*/
extern T_Data_Struct BSP_tp_pixad, BSP_TS; //当前触控坐标的AD值,前触控坐标的像素值
extern u16 BSP_vx, BSP_vy; //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 BSP_chx, BSP_chy;//默认像素点坐标为0时的AD起始值

void BSP_TS_Init (void);
void BSP_T_Adjust (void);
void BSP_T_point (void); //绘图函数

u16 BSP_TP_Read_XOY (u8 xy);//SPI （寄存器版本）收发数据
u8 BSP_Convert_Pos (void);
void BSP_Pointer_Update (void);

