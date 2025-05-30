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



/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
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
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOH
#define Valve_M1_Pin GPIO_PIN_0
#define Valve_M1_GPIO_Port GPIOC
#define Valve_M2_Pin GPIO_PIN_1
#define Valve_M2_GPIO_Port GPIOC
#define Valve_M3_Pin GPIO_PIN_2
#define Valve_M3_GPIO_Port GPIOC
#define Valve_EN_Pin GPIO_PIN_3
#define Valve_EN_GPIO_Port GPIOC
#define ESP_RST_Pin GPIO_PIN_0
#define ESP_RST_GPIO_Port GPIOA
#define ESP_RTS_Pin GPIO_PIN_4
#define ESP_RTS_GPIO_Port GPIOA
#define ESP_CTS_Pin GPIO_PIN_5
#define ESP_CTS_GPIO_Port GPIOA
#define ESP_IO4_Pin GPIO_PIN_6
#define ESP_IO4_GPIO_Port GPIOA
#define ESP_IO5_Pin GPIO_PIN_7
#define ESP_IO5_GPIO_Port GPIOA
#define Valve_RST_Pin GPIO_PIN_4
#define Valve_RST_GPIO_Port GPIOC
#define Valve_CLK_Pin GPIO_PIN_5
#define Valve_CLK_GPIO_Port GPIOC
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_2
#define LCD_DC_GPIO_Port GPIOB
#define T_CS_Pin GPIO_PIN_12
#define T_CS_GPIO_Port GPIOB
#define Valve_CCW_Pin GPIO_PIN_6
#define Valve_CCW_GPIO_Port GPIOC
#define Valve_LATCH_Pin GPIO_PIN_7
#define Valve_LATCH_GPIO_Port GPIOC
#define Valve_TQ_Pin GPIO_PIN_8
#define Valve_TQ_GPIO_Port GPIOC
#define LCD_T_DIN_Pin GPIO_PIN_15
#define LCD_T_DIN_GPIO_Port GPIOA
#define Valve_ALERT_Pin GPIO_PIN_10
#define Valve_ALERT_GPIO_Port GPIOC
#define OneWire_Pin GPIO_PIN_5
#define OneWire_GPIO_Port GPIOB
#define ESP_EN_Pin GPIO_PIN_8
#define ESP_EN_GPIO_Port GPIOB
#define LCD_CSB9_Pin GPIO_PIN_9
#define LCD_CSB9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define ERROR_COLD_WATER "Cold Water out of acceptable temperature inlet."
#define ERROR_HOT_WATER "Hot Water out of acceptable temperature inlet."

#define ERROR_TEMPERATURE_SET "Not a valid input temperature."
extern char error_msg_[100];

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
