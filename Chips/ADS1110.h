#include "ChipsDefine.h"


/*
Ӧ��˵����
�ڷ���I2C�豸ǰ�����ȵ��� i2c_CheckDevice ���I2C�豸�Ƿ��������ú���������GPIO
*/


/* �����дSCL��SDA�ĺ� */
#define I2C_SCL_1()  ADS1110_SCL_GPIO_Port->ODR |= ADS1110_SCL_Pin				/* SCL = 1 */
#define I2C_SCL_0()  ADS1110_SCL_GPIO_Port->ODR &=~ ADS1110_SCL_Pin				/* SCL = 0 */

#define I2C_SDA_1()  ADS1110_SDA_GPIO_Port->ODR |= ADS1110_SDA_Pin				/* SDA = 1 */
#define I2C_SDA_0() ADS1110_SDA_GPIO_Port->ODR &= ~ADS1110_SDA_Pin				/* SDA = 0 */

#define I2C_SDA_READ()  ((ADS1110_SDA_GPIO_Port->IDR & ADS1110_SDA_Pin) != 0)	/* ��SDA����״̬ */
#define I2C_SCL_READ()  ((ADS1110_SCL_GPIO_Port->IDR & ADS1110_SCL_Pin) != 0)	/* ��SCL����״̬ */

/*
#define   ADS1110_DIN_1 	ADS1110_SDA_GPIO_Port->ODR|=ADS1110_SDA_Pin
#define   ADS1110_DIN_0 	ADS1110_SDA_GPIO_Port->ODR&=~ADS1110_SDA_Pin			//  SDA

#define   ADS1110_SCLK_1	ADS1110_SCL_GPIO_Port->ODR|=ADS1110_SCL_Pin
#define   ADS1110_SCLK_0	ADS1110_SCL_GPIO_Port->ODR&=~ADS1110_SCL_Pin

#define   ADS1110_DOUT		((ADS1110_SDA_GPIO_Port->IDR) & ADS1110_SDA_Pin)		//  SDA

#define   ADS1110_ADDRESS   0x90
*/

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

void ADS1110_Init (void);
void i2c_Start (void);
void i2c_Stop (void);
void i2c_SendByte (uint8_t _ucByte);
uint8_t i2c_ReadByte (void);
uint8_t i2c_WaitAck (void);
void i2c_Ack (void);
void i2c_NAck (void);
uint8_t i2c_CheckDevice (uint8_t _Address);
void ads1110Config (void);
u16 RD_ADS (void);
u16 get_ad_result (void);

