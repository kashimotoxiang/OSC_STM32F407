/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

CRC_HandleTypeDef hcrc;

DAC_HandleTypeDef hdac;

SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

SRAM_HandleTypeDef hsram1;
SRAM_HandleTypeDef hsram2;

/* USER CODE BEGIN PV */

/* Private variables ---------------------------------------------------------*/
UART_TYPE RxBuff[1];
/* Private Struct-------------------------------------------------------*/
ADC_Conv_struct g_SamplData = {{0}};
FSM_struct g_FSM = {eFSM_WorkS_Emp, eFSM_WorkS_Emp, eFSM_MainS_Emp, eFSM_MainS_Emp};
OSC_struct g_OSCInfo = {
	eTrg_Mod_Centr,//触发模式
	eSampl_Mod_DMA, //采样模式
	eReslt_rat_0, //时间缩放比例
	1, //幅度缩放比例
	160, //触发点
	65535,//采样数值整型最大值
	0,//采样数值整型最小值
	0,//采样数值浮点型最大值
	0, //采样数值浮点型最小值
	0,//频率
	0,//显示数据从采样数据中的起始
	MEMORYE_DEPTH,//显示数据从采样数据中的终止
	0, //是否等效采样标识
	eClose,//是否打开测量标识
};
Page_struct g_DispPage = {ePage_Num_1, ePage_Num_2, ePage_Num_2};
Key_struct g_Key = {eKey_Empty, eKey_Empty};
GUIControl_struct g_GUICon = {eOpen, eOpen, eClose, eISEMPTY};
/* Others ---------------------------------------------------------*/
__uIO32 g_DACVal = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
void Error_Handler (void);
static void MX_GPIO_Init (void);
static void MX_DMA_Init (void);
static void MX_FSMC_Init (void);
static void MX_DAC_Init (void);
static void MX_CRC_Init (void);
static void MX_ADC1_Init (void);
static void MX_SPI1_Init (void);
static void MX_USART3_UART_Init (void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main (void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_FSMC_Init();
	MX_DAC_Init();
	MX_CRC_Init();
	MX_ADC1_Init();
	MX_SPI1_Init();
	MX_USART3_UART_Init();

	/* USER CODE BEGIN 2 */
	System_Initial_OSC();
	//	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	//	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0x08f);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		//FPGA_COM_SPISTART;
		FSM_OSC();
		BackgroundUpdata();
	}
	/* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config (void) {

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1 ;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4 ;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2 ;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init (void) {

	ADC_ChannelConfTypeDef sConfig;

	/**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
	*/
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
	*/
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}

}

/* CRC init function */
static void MX_CRC_Init (void) {

	hcrc.Instance = CRC;
	if (HAL_CRC_Init(&hcrc) != HAL_OK) {
		Error_Handler();
	}

}

/* DAC init function */
static void MX_DAC_Init (void) {

	DAC_ChannelConfTypeDef sConfig;

	/**DAC Initialization 
	*/
	hdac.Instance = DAC;
	if (HAL_DAC_Init(&hdac) != HAL_OK) {
		Error_Handler();
	}

	/**DAC channel OUT1 config 
	*/
	sConfig.DAC_Trigger = DAC_TRIGGER_NONE ;
	sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
	if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}

}

/* SPI1 init function */
static void MX_SPI1_Init (void) {

	hspi1.Instance = SPI1 ;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT ;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}

}

/* USART3 init function */
static void MX_USART3_UART_Init (void) {

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart3) != HAL_OK) {
		Error_Handler();
	}

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init (void) {
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	/* DMA1_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	/* DMA2_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init (void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, ESP_T_DIN_Pin | ESP_T_CS_Pin | ESP_T_CLK_Pin | ESP_LCD_REST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BSP_T_CS_GPIO_Port, BSP_T_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, BSP_T_CLK_Pin | BSP_T_IRQ_Pin | BSP_LCD_BL_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BSP_T_DIN_GPIO_Port, BSP_T_DIN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : ESP_T_DIN_Pin ESP_T_CS_Pin ESP_T_CLK_Pin ESP_LCD_REST_Pin */
	GPIO_InitStruct.Pin = ESP_T_DIN_Pin | ESP_T_CS_Pin | ESP_T_CLK_Pin | ESP_LCD_REST_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : BSP_T_CS_Pin */
	GPIO_InitStruct.Pin = BSP_T_CS_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW ;
	HAL_GPIO_Init(BSP_T_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : BSP_T_CLK_Pin BSP_T_IRQ_Pin BSP_LCD_BL_Pin */
	GPIO_InitStruct.Pin = BSP_T_CLK_Pin | BSP_T_IRQ_Pin | BSP_LCD_BL_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : BSP_T_DOUT_Pin */
	GPIO_InitStruct.Pin = BSP_T_DOUT_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(BSP_T_DOUT_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : BSP_T_DIN_Pin */
	GPIO_InitStruct.Pin = BSP_T_DIN_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(BSP_T_DIN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI1_CS_Pin */
	GPIO_InitStruct.Pin = SPI1_CS_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ESP_T_DOUT_Pin ESP_T_IRQ_Pin */
	GPIO_InitStruct.Pin = ESP_T_DOUT_Pin | ESP_T_IRQ_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* FSMC initialization function */
static void MX_FSMC_Init (void) {
	FSMC_NORSRAM_TimingTypeDef Timing;

	/** Perform the SRAM1 memory initialization sequence
	*/
	hsram1.Instance = FSMC_NORSRAM_DEVICE;
	hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram1.Init */
	hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
	hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE ;
	hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 4;
	Timing.AddressHoldTime = 15;
	Timing.DataSetupTime = 60;
	Timing.BusTurnAroundDuration = 0;
	Timing.CLKDivision = 16;
	Timing.DataLatency = 17;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
		Error_Handler();
	}

	/** Perform the SRAM2 memory initialization sequence
	*/
	hsram2.Instance = FSMC_NORSRAM_DEVICE;
	hsram2.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram2.Init */
	hsram2.Init.NSBank = FSMC_NORSRAM_BANK4;
	hsram2.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram2.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram2.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram2.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE ;
	hsram2.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram2.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram2.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram2.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram2.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram2.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram2.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram2.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram2.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 4;
	Timing.AddressHoldTime = 15;
	Timing.DataSetupTime = 60;
	Timing.BusTurnAroundDuration = 0;
	Timing.CLKDivision = 16;
	Timing.DataLatency = 17;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram2, &Timing, NULL) != HAL_OK) {
		Error_Handler();
	}

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler (void) {
	/* USER CODE BEGIN Error_Handler */
	/* User can add his own implementation to report the HAL error return state */
	while (1) {
	}
	/* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
/* USER CODE BEGIN 6 */
/* User can add his own implementation to report the file name and line number,
ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
/* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


