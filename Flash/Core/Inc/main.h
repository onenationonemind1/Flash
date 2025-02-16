/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timer.h"
#include "flash.h"
#include "modbus.h"

  extern TIM_HandleTypeDef htim2;
  extern UART_HandleTypeDef huart2;
  extern UART_HandleTypeDef huart3;
  extern DMA_HandleTypeDef hdma_usart2_tx;
  extern DMA_HandleTypeDef hdma_usart2_rx;

  typedef struct
  {
    uint16_t tvoc;       // TVOC concentration (ppb)
    uint16_t pm1_0;      // PM1.0 (μg/m3)
    uint16_t pm2_5;      // PM2.5 (μg/m3)
    uint16_t pm10;       // PM10 (μg/m3)
    int16_t temperature; // Temperature (°C * 10)
    uint16_t humidity;   // Humidity (% * 10)
  } AM1002_Data_t;

  /* USER CODE END Includes */

  /* Exported types ------------------------------------------------------------*/
  /* USER CODE BEGIN ET */
  #if 0
  #define terminal_mode 1
  #endif

  #if 1
  #define terminal_modbus 1
  #endif


  /* USER CODE END ET */

  /* Exported constants --------------------------------------------------------*/
  /* USER CODE BEGIN EC */

  /* USER CODE END EC */

  /* Exported macro ------------------------------------------------------------*/
  /* USER CODE BEGIN EM */

  /* USER CODE END EM */

  /* Exported functions prototypes ---------------------------------------------*/
  void Error_Handler(void);

  /* USER CODE BEGIN EFP */

  /* USER CODE END EFP */

  /* Private defines -----------------------------------------------------------*/

  /* USER CODE BEGIN Private defines */

  /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
