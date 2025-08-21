/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    I2C/I2C_TwoBoards_AdvComIT/Src/main.c
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32c0xx_nucleo.h"
#include <string.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define dbgprintf(fmt,...) dprintf(1, fmt, ## __VA_ARGS__) 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t g_last_i2c_err;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void test(void);
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

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Configure LED4 */
  BSP_LED_Init(LED4);

  dbgprintf("MLX I2C test3\r\n");

  /* Configure User push-button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  test();

  while (1)
  {
    /* USER CODE END WHILE */
  }
    /* USER CODE BEGIN 3 */
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
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
// TODO check & reset I2C................
  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00601854; //Cube, @400 KHz
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  //hi2c1.Init.OwnAddress1 = I2C_ADDRESS;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analog filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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

  /*Configure GPIO pin : BTN_Pin */
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED4_Pin MLX_TRIG_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|MLX_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MLX_READY_Pin */
  GPIO_InitStruct.Pin = MLX_READY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MLX_READY_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void I2C_MLX_transact(const uint8_t *txbuf, unsigned txlen, uint8_t *rxbuf, unsigned rxlen, uint8_t *status)
{
    HAL_StatusTypeDef st;
    static uint16_t DevAddress = 0;
    if (0 == DevAddress) {
        // Detect on known addresses:
        DevAddress = 0b00011000;
        st = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress|1, 1, 10);
        if (st == HAL_OK) goto found;
    
        DevAddress = 0b00011010;
        st = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress|1, 1, 10);
        if (st == HAL_OK) goto found;
    
        DevAddress = 0b00011100;
        st = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress|1, 1, 10);
        if (st == HAL_OK) goto found;
    
        DevAddress = 0b00011110;
        st = HAL_I2C_IsDeviceReady(&hi2c1, DevAddress|1, 1, 10);
        if (st == HAL_OK) goto found;

        *status = 0xFF;
        dbgprintf("MLX not found on I2C\n");
        return;
        found:;
    }

    uint16_t MemAddress = 0;
    uint16_t MemAddSize = 0;
    uint8_t pData[10];
    uint16_t Size = 0;
    uint32_t Timeout = 10;

    // ST lib limitation: "mem address" up to 2 bytes I2C_MEMADD_SIZE_8BIT/I2C_MEMADD_SIZE_16BIT
    // For register write: do separate write and read. Not documented but seems to work.
    // (as in the circuitpython driver by Kevin Townsend)
    if (txlen > 2 && rxlen == 0) {
        st = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, (void*)txbuf, txlen, Timeout);
        if (st != HAL_OK) {
            //I2C error!
            uint32_t err = HAL_I2C_GetError(&hi2c1);
            g_last_i2c_err = err; //Bitmask: HAL_I2C_ERROR_AF=0x4 HAL_I2C_ERROR_TIMEOUT=0x20
            *status = 0xFF;//$$$$$$TBD
            dbgprintf("MLX I2C WR err %4.4X\n", (unsigned)err);
            *status = 0xFF;
            return;
        }
        // Now read status:
        st = HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, 1, Timeout);
        if (st != HAL_OK) {
            //I2C error!
            uint32_t err = HAL_I2C_GetError(&hi2c1);
            g_last_i2c_err = err;
            *status = 0xFF;
            dbgprintf("MLX I2C RD err %4.4X\n", (unsigned)err);
            *status = 0xFF;
            return;
        }
        *status = pData[0];
        return;
    }

    // txbuf[0] is command
    switch(txlen)
    {
    case 1:
        MemAddSize = I2C_MEMADD_SIZE_8BIT;
        MemAddress = (uint8_t)txbuf[0];
        break;
    case 2:
        MemAddSize = I2C_MEMADD_SIZE_16BIT;
        MemAddress = (uint16_t)(txbuf[1] | (uint16_t)txbuf[0] << 8);
        break;
    default: // NOT IMPL YET!
        *status = 0xFF;
        return;
    }

    // Input data: at least one byte
    Size = 1 + rxlen;
    if (Size >= sizeof(pData))
    {
        //NOT IMPL! TODO
        *status = 0xFF;
        return;
    }

    st = HAL_I2C_Mem_Read(&hi2c1, DevAddress, MemAddress, MemAddSize, pData, Size, Timeout);
    if (st == HAL_OK)
    {
        *status = pData[0];
        memcpy(rxbuf, &pData[1], rxlen);
    }
    else
    {
        //I2C error!
        uint32_t err = HAL_I2C_GetError(&hi2c1);
        g_last_i2c_err = err; //Bitmask: HAL_I2C_ERROR_AF=0x4 HAL_I2C_ERROR_TIMEOUT=0x20
        *status = 0xFF;//$$$$$$TBD
        dbgprintf("MLX I2C err %4.4X\n", (unsigned)err);
    }
}

int _write(int fd, const char *p, unsigned len)
{
    if (len == 0) return 0;
    if (!p || len > 500) { // sanity
        __BKPT(3);
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)p, len, HAL_MAX_DELAY);
    return len;
}


void wait_button(void)
{
  // BUTT1 active low
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_RESET)
  {
  }

  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
  }
}


_Bool check_button(void)
{
  // BUTT1 active low
  return (BSP_PB_GetState(BUTTON_USER) == GPIO_PIN_RESET);
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* Error if LED4 is slowly blinking (1 sec. period) */
  while (1)
  {
    __BKPT(3);
    BSP_LED_Toggle(LED4);
    HAL_Delay(1000);
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
  Error_Handler();
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
