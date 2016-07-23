/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define ESP_T_DIN_Pin GPIO_PIN_2
#define ESP_T_DIN_GPIO_Port GPIOE
#define ESP_T_CS_Pin GPIO_PIN_4
#define ESP_T_CS_GPIO_Port GPIOE
#define ESP_T_CLK_Pin GPIO_PIN_5
#define ESP_T_CLK_GPIO_Port GPIOE
#define ESP_LCD_REST_Pin GPIO_PIN_6
#define ESP_LCD_REST_GPIO_Port GPIOE
#define BSP_T_CS_Pin GPIO_PIN_13
#define BSP_T_CS_GPIO_Port GPIOC
#define BSP_T_CLK_Pin GPIO_PIN_0
#define BSP_T_CLK_GPIO_Port GPIOB
#define BSP_T_IRQ_Pin GPIO_PIN_1
#define BSP_T_IRQ_GPIO_Port GPIOB
#define BSP_T_DOUT_Pin GPIO_PIN_2
#define BSP_T_DOUT_GPIO_Port GPIOB
#define BSP_T_DIN_Pin GPIO_PIN_11
#define BSP_T_DIN_GPIO_Port GPIOF
#define BSP_LCD_BL_Pin GPIO_PIN_15
#define BSP_LCD_BL_GPIO_Port GPIOB
#define SPI1_CS_Pin GPIO_PIN_7
#define SPI1_CS_GPIO_Port GPIOG
#define ESP_T_DOUT_Pin GPIO_PIN_2
#define ESP_T_DOUT_GPIO_Port GPIOD
#define ESP_T_IRQ_Pin GPIO_PIN_3
#define ESP_T_IRQ_GPIO_Port GPIOD
#define ADS1110_SCL_Pin GPIO_PIN_0
#define ADS1110_SCL_GPIO_Port GPIOE
#define ADS1110_SDA_Pin GPIO_PIN_1
#define ADS1110_SDA_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
