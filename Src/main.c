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
SRAM_HandleTypeDef hsram4;

/* USER CODE BEGIN PV */

/* Private variables ---------------------------------------------------------*/
UART_TYPE RxBuff[1];
/* Private Struct-------------------------------------------------------*/
WAVE_TYPE WaveArray[TFT_WIDTH ] = {0};//波型数组

FPGAControl_struct g_FPGAPar_A = {500000000};//初始化FPAG器件参数
FPGAControl_struct g_FPGACon_C = {500000000};//初始化FPAG器件参数
FPGAData_struct g_FPGAData = {{0}};
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
	MEMORYE_DEPTH ,//显示数据从采样数据中的终止
	0, //是否等效采样标识
	eClose,//是否打开测量标识
};
Page_struct g_DispPage = {ePage_Num_1, ePage_Num_2, ePage_Num_2};
GUIControl_struct g_GUICon = {eOpen, eOpen, eClose, eClose, eISEMPTY};
SenseData_struct g_Sense = {0};//传感器数据
DevicePar_struct g_Device = {
	Triangle_Wave ,//波形
	1000000,//频率
	50,//占空比
};
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
	Display_Initial();
	FSM_DeviceInit();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (g_GUICon.GraphDispState == eOpen) {//停止时不进行任何活动 ，开启时才进入任务
			//FPGA_GetMeasurePar();//请求基础数据

			FPGA_WritePar(10 * g_FPGAData.SignalFreq, 0x0FFF,
			              0x0000, FPGADATALENTGH128, 0, &g_FPGAPar_A);
			//FPGA_ParallDataTrans(g_FPGAData.ADCParMeasureData, AMPLITITUDEDATALENGTH);//采样128个数据，为了计算幅度
			FPGA_AmplitudeCompute(&g_FPGAData);//计算出幅度

			FPGA_WritePar(g_FPGAData.SignalFreq, 0.9 * g_FPGAData.i_Amplitude + g_FPGAData.i_AmplitudeMin,
			              0.1 * g_FPGAData.i_Amplitude + g_FPGAData.i_AmplitudeMin, FPGADATALENTGH2048, 1, &g_FPGACon_C);
			//FPGA_ParallDataTrans(g_FPGAData.ADCUpTimeData, UPTIMEDATALENGTH);//采样2048个数据，为了计算上升时间
			FPGA_UpTimeCompute(&g_FPGAData, &g_FPGACon_C);//计算上升时间

			//FPGA_ParallDataTrans(g_FPGAData.ADCUpTimeData, WAVEDATALENGTH);//采样2048个数据，为了显示波形

			FSM_OSCDisp();//波形显示

			FSM_DeviceOperation();//器件数据更新
		}
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
	RCC_OscInitStruct.PLL.PLLN = 250;
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
	__HAL_RCC_DMA2_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

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
	HAL_GPIO_WritePin(GPIOB, BSP_T_CLK_Pin | BSP_LCD_BL_Pin | FPGA_COM2_Pin | FPGA_COM1_Pin
	                                         | FPGA_SCK_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(BSP_T_DIN_GPIO_Port, BSP_T_DIN_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, AD9834_SDATA_Pin | AD9834_SCLK_Pin | AD9834_RESET_Pin | AD9834_FSYNC_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, ADS1110_SCL_Pin | ADS1110_SDA_Pin, GPIO_PIN_SET);

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

	/*Configure GPIO pins : PC0 PC1 PC2 PC3 
	                         PC4 PC5 PC6 PC7 
	                         PC8 PC9 PC11 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
	                                   | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7
	                                   | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_11 ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : BSP_T_CLK_Pin BSP_LCD_BL_Pin FPGA_COM2_Pin FPGA_COM1_Pin 
	                         FPGA_SCK_Pin */
	GPIO_InitStruct.Pin = BSP_T_CLK_Pin | BSP_LCD_BL_Pin | FPGA_COM2_Pin | FPGA_COM1_Pin
	                                      | FPGA_SCK_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : BSP_T_IRQ_Pin BSP_T_DOUT_Pin */
	GPIO_InitStruct.Pin = BSP_T_IRQ_Pin | BSP_T_DOUT_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
	GPIO_InitStruct.Pull = GPIO_PULLUP ;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : BSP_T_DIN_Pin */
	GPIO_InitStruct.Pin = BSP_T_DIN_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(BSP_T_DIN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : AD9834_SDATA_Pin AD9834_SCLK_Pin AD9834_RESET_Pin AD9834_FSYNC_Pin */
	GPIO_InitStruct.Pin = AD9834_SDATA_Pin | AD9834_SCLK_Pin | AD9834_RESET_Pin | AD9834_FSYNC_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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

	/*Configure GPIO pins : ADS1110_SCL_Pin ADS1110_SDA_Pin */
	GPIO_InitStruct.Pin = ADS1110_SCL_Pin | ADS1110_SDA_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD ;
	GPIO_InitStruct.Pull = GPIO_NOPULL ;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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
	Timing.AddressHoldTime = 4;
	Timing.DataSetupTime = 60;
	Timing.BusTurnAroundDuration = 0;
	Timing.CLKDivision = 0;
	Timing.DataLatency = 0;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK) {
		Error_Handler();
	}

	/** Perform the SRAM4 memory initialization sequence
	*/
	hsram4.Instance = FSMC_NORSRAM_DEVICE;
	hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram4.Init */
	hsram4.Init.NSBank = FSMC_NORSRAM_BANK4;
	hsram4.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram4.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram4.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE ;
	hsram4.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram4.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram4.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram4.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram4.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram4.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram4.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram4.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram4.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	/* Timing */
	Timing.AddressSetupTime = 4;
	Timing.AddressHoldTime = 4;
	Timing.DataSetupTime = 60;
	Timing.BusTurnAroundDuration = 0;
	Timing.CLKDivision = 0;
	Timing.DataLatency = 0;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram4, &Timing, NULL) != HAL_OK) {
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


