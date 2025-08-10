/* USER CODE BEGIN Header */
/**
******************************************************************************
* SPI MLX TEST
******************************************************************************
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Buffer used for transmission */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)
uint16_t aTxBuffer[] = {0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0,0x1234,0x5678,0x9ABC,0xDEF0};
__IO uint16_t ubNbDataToTransmit = BUFFERSIZE;
__IO uint8_t ubTransmitIndex = 0;

/* Buffer used for reception */
uint16_t aRxBuffer[BUFFERSIZE];
__IO uint16_t ubNbDataToReceive = BUFFERSIZE;
__IO uint8_t ubReceiveIndex = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
extern void test();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */

  /* Configure LED4 */
  BSP_LED_Init(LED4);

  /* Configure User push-button button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  //+++ VCP UART
//  BSP_COM_Init(COM1, &(COM_InitTypeDef){.BaudRate=115200,.Parity=UART_PARITY_NONE,.WordLength=UART_WORDLENGTH_8B,} );
//  BSP_COM_SelectLogPort(COM1);
  MX_USART2_UART_Init();

  const char message[] = "MLX SPI test1\r\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen((char const*)message), HAL_MAX_DELAY);
 
  test();
  
  /* Wait for User push-button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
    BSP_LED_Toggle(LED4);
    HAL_Delay(100);
  }
  BSP_LED_Off(LED4);

  /*##-1- Start the Full Duplex Communication process ########################*/
  /* Enable SPI before start transmission */
  LL_SPI_Enable(hspi1.Instance);


  while(ubNbDataToReceive > 0)
  {
    /* Check TXE flag to transmit data */
    if(( LL_SPI_IsActiveFlag_TXE(hspi1.Instance)) && (ubNbDataToTransmit > 0))
    {
      /* Transmit 16bit Data */
      LL_SPI_TransmitData16(hspi1.Instance, aTxBuffer[ubTransmitIndex++]);
      ubNbDataToTransmit--;
    }
    /* Check RXNE flag */
    if(LL_SPI_IsActiveFlag_RXNE(hspi1.Instance))
    {
      /* Receive 16bit Data */
      aRxBuffer[ubReceiveIndex++] = LL_SPI_ReceiveData16(hspi1.Instance);
      ubNbDataToReceive--;
    }
    HAL_Delay(1);
  }

  /*##-3- Compare the sent and received buffers ##############################*/
  /* Compare the sent and received buffers */
  if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, COUNTOF(aTxBuffer)))
  {
    /* Transfer error in transmission process */
    Error_Handler();
  }
  else
  {
    /* Turn LED4 on: Transfer in transmission/Reception process is correct */
    BSP_LED_On(LED4);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */
  LL_SPI_SetRxFIFOThreshold(SPI1, LL_SPI_RX_FIFO_TH_QUARTER);
  LL_SPI_Disable(SPI1); // To drop HW NSS, disable the SPI
  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED4_Pin|MLX_TRIG_Pin, GPIO_PIN_RESET);

#if 0  /*Configure GPIO pin : BUTTON_USER_Pin */
  GPIO_InitStruct.Pin = BUTTON_USER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_USER_GPIO_Port, &GPIO_InitStruct);
#endif
  /*Configure GPIO pins : LED4_Pin MLX_TRIG_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|MLX_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MLX_INTR_Pin */
  GPIO_InitStruct.Pin = MLX_INTR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; //$$$ for now use as INPUT --pa01
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MLX_INTR_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
    /* Toggle LED4 for error */
    BSP_LED_Toggle(LED4);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */


static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT; //UART_ADVFEATURE_NO_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}


// Exported to library:
void SPI_MLX_transact(const uint8_t *txbuf, unsigned txlen, uint8_t *rxbuf, unsigned rxlen, uint8_t *status)
{
    SPI_TypeDef *spix = hspi1.Instance;

    // ACTIVATE NSS
    LL_SPI_Enable(spix);
    (void)LL_SPI_GetNSSMode(spix); //$$$ delay after activating NSS

    // Transmit TXBUF
    for (unsigned i = 0; i < txlen; i++) {
        /* Check TXE flag to transmit data */
        while (!LL_SPI_IsActiveFlag_TXE(spix)) {}
        /* Transmit 8 bit Data */
        LL_SPI_TransmitData8(spix, txbuf[i]);
        /* Check RXNE flag */
        while (!LL_SPI_IsActiveFlag_RXNE(spix)) {}
        /* Receive 8 bit dummy data */
        uint8_t v;
        v = LL_SPI_ReceiveData8(spix);
    }

    // Receive:
    // Status is 1st IN byte
    while (!LL_SPI_IsActiveFlag_TXE(spix)) {}
    /* Transmit 8 bit dummy data */
    LL_SPI_TransmitData8(spix, 0);
    /* Check RXNE flag */
    while (!LL_SPI_IsActiveFlag_RXNE(spix)) {}
    /* Receive 8 bit dummy data */
    *status = LL_SPI_ReceiveData8(spix);

    for (unsigned i = 0; i < rxlen; i++) {
        /* Check TXE flag to transmit data */
        while (!LL_SPI_IsActiveFlag_TXE(spix)) {}
        /* Transmit 8 bit dummy data */
        LL_SPI_TransmitData8(spix, 0);
        /* Check RXNE flag */
        while (!LL_SPI_IsActiveFlag_RXNE(spix)) {}
        /* Receive 8 bit dummy data */
        rxbuf[i] = LL_SPI_ReceiveData8(spix);
    }

    // DROP CS $$$$$$$$$$$$$$$$$$$
    LL_SPI_Disable(spix);
}
