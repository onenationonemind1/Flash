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
#include "sensor.h"
#include "uart_handle.h"
#include "parser.h"
#include "function.h"

#define __USE_MODBUS_PARSER_ 1
#define __USE_MODE_RS232_ 1
#define __USE_MODE_RS485_ 0

#define INIT_DATA_1 0x2
#define INIT_DATA_2 0x3

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_REVISION 2

#define RS485_ID 1
#define RS485_FUNCTION_CODE 3

#define PERIOD_SEC_01 1
#define PERIOD_SEC_02 2
#define PERIOD_SEC_05 5
#define PERIOD_SEC_10 10
#define PERIOD_MIN_01 60
#define PERIOD_MIN_02 120
#define PERIOD_MIN_05 300
#define PERIOD_MIN_60 3600

#if 1
#define TIME_CETIFICATION_MIN_01 (60 * 1)
#define TIME_CETIFICATION_MIN_05 (60 * 5)
#define TIME_CETIFICATION_MIN_60 (60 * 60)
#else
#define TIME_CETIFICATION_MIN_01 (1)
#define TIME_CETIFICATION_MIN_05 (5)
#define TIME_CETIFICATION_MIN_60 (60)
#endif

// Key
#define PORT_KEY_POWER GPIOB
#define PORT_KEY_UP GPIOB
#define PORT_KEY_DOWN GPIOB

#define PIN_KEY_POWER GPIO_PIN_9
#define PIN_KEY_UP GPIO_PIN_4
#define PIN_KEY_DOWN GPIO_PIN_5

// Led
#define PORT_LED_RED GPIOC
#define PORT_LED_GREEN GPIOC
#define PORT_LED_BLUE GPIOC

#define PIN_LED_RED GPIO_PIN_13
#define PIN_LED_GREEN GPIO_PIN_14
#define PIN_LED_BLUE GPIO_PIN_15

// Switch
#define PORT_MODE_0 GPIOB
#define PORT_MODE_1 GPIOB
#define PORT_MODE_2 GPIOB
#define PORT_MODE_3 GPIOB

#define PIN_MODE_0 GPIO_PIN_0
#define PIN_MODE_1 GPIO_PIN_1
#define PIN_MODE_2 GPIO_PIN_2
#define PIN_MODE_3 GPIO_PIN_3

// RS485
#define PORT_RS485_ENABLE GPIOA
#define PIN_RS485_ENABLE GPIO_PIN_15

#if 1
#define uprintf printf
#define printf_mb printf
#else
#define uprintf(fmt, ...)
#define printf_mb
#endif

  TIM_HandleTypeDef htim2;

  UART_HandleTypeDef huart1;
  UART_HandleTypeDef huart2;
  UART_HandleTypeDef huart3;

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
