/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
AM1002_Data_t multiDataread(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
// {
//   if(htim->Instance == TIM2)
//   {
//     HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//   }
// }

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */
  AM1002_Data_t ProcessAM1002Data(uint8_t *data, AM1002_Data_t *result)
  {

    // 실제 데이터는 시작 바이트(16 13 16) 이후부터 시작
    uint8_t *actual_data = data + 3;
    result->tvoc = (uint16_t)(actual_data[0] << 8 | actual_data[1]);
    result->pm1_0 = (uint16_t)(actual_data[4] << 8 | actual_data[5]);
    result->pm2_5 = (uint16_t)(actual_data[6] << 8 | actual_data[7]);
    result->pm10 = (uint16_t)(actual_data[8] << 8 | actual_data[9]);

    // 온도 계산 (DF11 DF12) ((DF11*256^1 + DF12)-500）/ 10
    uint16_t temp_raw = (uint16_t)(actual_data[10] << 8 | actual_data[11]);
    result->temperature = (int16_t)(temp_raw - 500); // 10배 값을 저장

    // 습도 계산 (DF13 DF14) ((DF13*256^1 + DF14))
    result->humidity = (uint16_t)(actual_data[12] << 8 | actual_data[13]); // 10배 값을 저장

    return *result;
  }

  void PrintAM1002Data(UART_HandleTypeDef * huart, AM1002_Data_t * data)
  {
    char buffer[32];

    sprintf(buffer, "TVOC: %d ppb\r\n", data->tvoc);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    sprintf(buffer, "PM1.0: %d ug/m3\r\n", data->pm1_0);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    sprintf(buffer, "PM2.5: %d ug/m3\r\n", data->pm2_5);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    sprintf(buffer, "PM10: %d ug/m3\r\n", data->pm10);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    // 온도는 10배 값이므로 정수부와 소수부를 분리하여 출력
    sprintf(buffer, "Temperature: %d.%d C\r\n", data->temperature / 10, abs(data->temperature % 10));
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    // 습도는 10배 값이므로 정수부와 소수부를 분리하여 출력
    sprintf(buffer, "Humidity: %d.%d %%\r\n", data->humidity / 10, data->humidity % 10);
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    HAL_UART_Transmit(huart, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
  }

  AM1002_Data_t ProcessAndPrintAM1002(UART_HandleTypeDef * huart, uint8_t *rx_buffer)
  {
    AM1002_Data_t sensorData;
    AM1002_Data_t result = ProcessAM1002Data(rx_buffer, &sensorData);
#ifdef terminal_mode
    PrintAM1002Data(huart, &sensorData);
#endif

    return result;
  }
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  uint8_t rx_data;
  uint8_t cmd[] = {0x11, 0x01, 0x16, 0xD8};
  uint8_t rx_buffer[32];

  int count = 0;
  uint8_t test = 0x00;
  uint8_t *arr_test = &test;
  uint8_t data;

  uint8_t request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0xCD};
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    if (g_timer_ms_1000 == ENABLE)
    {
      g_timer_ms_1000 = DISABLE;

      handle_am1002();

#if 0
      // AM1002_Data_t read_data = multiDataread(); // 임시 변수 사용
      memset(rx_buffer, 0, sizeof(rx_buffer));

      // PrintCalibrationData(read_data);
      // HAL_UART_Transmit(&huart3, (uint8_t *)"Data read done\r\n", 16, HAL_MAX_DELAY);

      HAL_UART_Transmit(&huart2, cmd, sizeof(cmd), 1000);
      HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, rx_buffer, 22, 1000);

      HAL_UART_Receive(&huart3, request, sizeof(request), 1000);
      // HAL_UART_Transmit(&huart3, request, sizeof(request), 100);
      HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

      if (request[0] == 0x01)
      {
        AM1002_Data_t read_data = multiDataread();

        uint8_t response1[] = {0x01, 0x03, 0x0C,
                               (uint8_t)(read_data.tvoc >> 8), (uint8_t)(read_data.tvoc & 0xFF),               // TVOC
                               (uint8_t)(read_data.pm1_0 >> 8), (uint8_t)(read_data.pm1_0 & 0xFF),             // PM1.0
                               (uint8_t)(read_data.pm2_5 >> 8), (uint8_t)(read_data.pm2_5 & 0xFF),             // PM2.5
                               (uint8_t)(read_data.pm10 >> 8), (uint8_t)(read_data.pm10 & 0xFF),               // PM10
                               (uint8_t)(read_data.humidity >> 8), (uint8_t)(read_data.humidity & 0xFF),       // Temperature
                               (uint8_t)(read_data.temperature >> 8), (uint8_t)(read_data.temperature & 0xFF), // Humidity
                               0x00, 0x00};
        uint16_t crc = ModBus_CRC16(response1, sizeof(response1) - 2);
        response1[sizeof(response1) - 2] = crc & 0xFF;        // CRC Low
        response1[sizeof(response1) - 1] = (crc >> 8) & 0xFF; // CRC High

        HAL_UART_Transmit(&huart3, &response1, sizeof(response1), 100);
      }

#if 1
      if (request[0] == 0x02)
      {
        AM1002_Data_t write_data = ProcessAndPrintAM1002(&huart3, rx_buffer);
        multiDatawrite(write_data);

        uint8_t response[] = {0x02, 0x03, 0x0C,
                              (uint8_t)(write_data.tvoc >> 8), (uint8_t)(write_data.tvoc & 0xFF),               // TVOC
                              (uint8_t)(write_data.pm1_0 >> 8), (uint8_t)(write_data.pm1_0 & 0xFF),             // PM1.0
                              (uint8_t)(write_data.pm2_5 >> 8), (uint8_t)(write_data.pm2_5 & 0xFF),             // PM2.5
                              (uint8_t)(write_data.pm10 >> 8), (uint8_t)(write_data.pm10 & 0xFF),               // PM10
                              (uint8_t)(write_data.humidity >> 8), (uint8_t)(write_data.humidity & 0xFF),       // Temperature
                              (uint8_t)(write_data.temperature >> 8), (uint8_t)(write_data.temperature & 0xFF), // Humidity
                              0xB4, 0x27};
        uint16_t crc = ModBus_CRC16(response, sizeof(response) - 2);
        response[sizeof(response) - 2] = crc & 0xFF;        // CRC Low
        response[sizeof(response) - 1] = (crc >> 8) & 0xFF; // CRC High

        HAL_UART_Transmit(&huart3, &response, sizeof(response), 100);
      }
#endif

      if (status == HAL_OK && request[0] == 0x03)
      {

        // char debug[100];
        // sprintf(debug, "Raw Data: ");
        // HAL_UART_Transmit(&huart3, (uint8_t *)debug, strlen(debug), HAL_MAX_DELAY);

        // memset(debug, 0, sizeof(debug));
        // for (int i = 0; i < 22; i++)
        // {
        //   sprintf(debug, "%02X ", rx_buffer[i]);
        //   HAL_UART_Transmit(&huart3, (uint8_t *)debug, strlen(debug), HAL_MAX_DELAY);
        // }
        // HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

        if (1)
        {
          AM1002_Data_t current_data = ProcessAndPrintAM1002(&huart3, rx_buffer);

#ifdef terminal_mode
          HAL_UART_Transmit(&huart3, (uint8_t *)"Humidity is low\r\n", 18, HAL_MAX_DELAY);
#endif
          uint8_t response[] = {0x03, 0x03, 0x0C,
                                (uint8_t)(current_data.tvoc >> 8), (uint8_t)(current_data.tvoc & 0xFF),               // TVOC
                                (uint8_t)(current_data.pm1_0 >> 8), (uint8_t)(current_data.pm1_0 & 0xFF),             // PM1.0
                                (uint8_t)(current_data.pm2_5 >> 8), (uint8_t)(current_data.pm2_5 & 0xFF),             // PM2.5
                                (uint8_t)(current_data.pm10 >> 8), (uint8_t)(current_data.pm10 & 0xFF),               // PM10
                                (uint8_t)(current_data.humidity >> 8), (uint8_t)(current_data.humidity & 0xFF),       // Temperature
                                (uint8_t)(current_data.temperature >> 8), (uint8_t)(current_data.temperature & 0xFF), // Humidity
                                0xB4, 0x27};
          uint16_t crc = ModBus_CRC16(response, sizeof(response) - 2);
          response[sizeof(response) - 2] = crc & 0xFF;        // CRC Low
          response[sizeof(response) - 1] = (crc >> 8) & 0xFF; // CRC High

          HAL_UART_Transmit(&huart3, &response, sizeof(response), 100);
          // HAL_UART_Transmit(&huart4, &response, sizeof(response), 100);
        }
        else
        {
#ifdef terminal_mode
          HAL_UART_Transmit(&huart3, (uint8_t *)"Error\r\n", 7, HAL_MAX_DELAY);
#endif
        }
      }
      else
      {
#ifdef terminal_mode
        HAL_UART_Transmit(&huart3, (uint8_t *)"Error\r\n", 7, HAL_MAX_DELAY);
#endif
      }
#endif
    }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE END TIM2_Init 2 */
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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
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
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
