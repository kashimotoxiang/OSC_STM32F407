#pragma once
#include "LCD_MD050SD.h"

/* touch panel interface define */
#define  ESP_T_CMD_RDX  0xD0	 //����IC�����������
#define  ESP_T_CMD_RDY  0x90
//�������Ŷ���------------------------------------------------------------------------------------------//
#define	ESP_T_CLK_High	      ESP_T_CLK_GPIO_Port->ODR|=ESP_T_CLK_Pin    //����/����      		PA11
#define	ESP_T_CS_High	      ESP_T_CS_GPIO_Port->ODR|=ESP_T_CS_Pin    //д����				PA12
#define	ESP_T_DIN_High	      ESP_T_DIN_GPIO_Port->ODR|=ESP_T_DIN_Pin   //������			 	PA13

#define	ESP_T_CLK_Low		  ESP_T_CLK_GPIO_Port->ODR&=~ESP_T_CLK_Pin     //Ƭѡ�˿�  	     	PA11
#define	ESP_T_CS_Low	      ESP_T_CS_GPIO_Port->ODR&=~ESP_T_CS_Pin     //������			 	PA12
#define	ESP_T_DIN_Low	      ESP_T_DIN_GPIO_Port->ODR&=~ESP_T_DIN_Pin    //д����			 	PA13

#define ESP_T_IRQ_Dect		  ((ESP_T_IRQ_GPIO_Port->IDR)&ESP_T_IRQ_Pin)
#define ESP_T_DOUT_Dect    �� ((ESP_T_DOUT_GPIO_Port->IDR)&ESP_T_DOUT_Pin)

//------------------------------------------------------------------------------------------//

typedef struct T_Data_Struct {
	u16 xc;//��ǰ����
	u16 yc;
	u16 xl;//�ϴ�����
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
extern T_Data_Struct ESP_tp_pixad, ESP_TS; //��ǰ���������ADֵ,ǰ�������������ֵ
extern u16 ESP_vx, ESP_vy; //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern u16 ESP_chx, ESP_chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
void ESP_TS_Init (void);
void ESP_T_Adjust (void);
void ESP_T_point (void); //��ͼ����

u16 ESP_TP_Read_XOY (u8 xy);//SPI ���Ĵ����汾���շ�����
u8 ESP_Convert_Pos (void);
void ESP_Pointer_Update (void);

