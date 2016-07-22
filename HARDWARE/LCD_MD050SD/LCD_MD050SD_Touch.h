#pragma once
#include "LCD_MD050SD.h"

/* touch panel interface define */
#define  ESP_T_CMD_RDX  0xD0	 //触摸IC读坐标积存器
#define  ESP_T_CMD_RDY  0x90
//触摸引脚定义------------------------------------------------------------------------------------------//
#define	ESP_T_CLK_High	      ESP_T_CLK_GPIO_Port->ODR|=ESP_T_CLK_Pin    //数据/命令      		PA11
#define	ESP_T_CS_High	      ESP_T_CS_GPIO_Port->ODR|=ESP_T_CS_Pin    //写数据				PA12
#define	ESP_T_DIN_High	      ESP_T_DIN_GPIO_Port->ODR|=ESP_T_DIN_Pin   //读数据			 	PA13

#define	ESP_T_CLK_Low		  ESP_T_CLK_GPIO_Port->ODR&=~ESP_T_CLK_Pin     //片选端口  	     	PA11
#define	ESP_T_CS_Low	      ESP_T_CS_GPIO_Port->ODR&=~ESP_T_CS_Pin     //读数据			 	PA12
#define	ESP_T_DIN_Low	      ESP_T_DIN_GPIO_Port->ODR&=~ESP_T_DIN_Pin    //写数据			 	PA13

#define ESP_T_IRQ_Dect		  ((ESP_T_IRQ_GPIO_Port->IDR)&ESP_T_IRQ_Pin)
#define ESP_T_DOUT_Dect    　 ((ESP_T_DOUT_GPIO_Port->IDR)&ESP_T_DOUT_Pin)

//------------------------------------------------------------------------------------------//

typedef struct T_Data_Struct {
	u16 xc;//当前坐标
	u16 yc;
	u16 xl;//上次坐标
	u16 yl;
	u8 TouchDetected;
} T_Data_Struct;

struct tp_pixu32_ {
	u32 xc;
	u32 yc;
	u32 xl;
	u32 yl;
};


/*-------------------------------------------------------*/
extern T_Data_Struct ESP_tp_pixad, ESP_TS; //当前触控坐标的AD值,前触控坐标的像素值
extern u16 ESP_vx, ESP_vy; //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern u16 ESP_chx, ESP_chy;//默认像素点坐标为0时的AD起始值
void ESP_TS_Init (void);
void ESP_T_Adjust (void);
void ESP_T_point (void); //绘图函数

u16 ESP_TP_Read_XOY (u8 xy);//SPI （寄存器版本）收发数据
u8 ESP_Convert_Pos (void);
void ESP_Pointer_Update (void);

