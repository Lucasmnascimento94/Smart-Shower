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
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint32_t buffer[2500];

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
#define FLASH_RESET_Pin GPIO_PIN_4
#define FLASH_RESET_GPIO_Port GPIOC
#define LCD_T_CS_Pin GPIO_PIN_15
#define LCD_T_CS_GPIO_Port GPIOB
#define FMC_LDQM_Pin GPIO_PIN_12
#define FMC_LDQM_GPIO_Port GPIOD
#define FMC_UDQM_Pin GPIO_PIN_13
#define FMC_UDQM_GPIO_Port GPIOD
#define ESP_RTS_Pin GPIO_PIN_8
#define ESP_RTS_GPIO_Port GPIOC
#define ESP_RST_Pin GPIO_PIN_4
#define ESP_RST_GPIO_Port GPIOD
#define ESP_EN_Pin GPIO_PIN_5
#define ESP_EN_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
