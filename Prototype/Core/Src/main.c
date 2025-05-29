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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include <stdint.h>
#include <serial_printing.h>
#include "GUI.h"
#include "types.h"
#include "touch.h"
#include "control.h"

// Initialize the framebuffer to a specific color, e.g., 0xFFFF (white in RGB565)

/*static const uint8_t framebuffer[FRAMEBUFFER_SIZE] = {
    [0 ... FRAMEBUFFER_SIZE - 1] = color_index  // GNU C extension to initialize all elements
};*/ /*780 x 400*/
uint32_t adcs[2] = {0};
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile uint8_t uart8_tx_complete = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart8;

SDRAM_HandleTypeDef hsdram1;

/* Definitions for mainTask */
osThreadId_t mainTaskHandle;
const osThreadAttr_t mainTask_attributes = {
  .name = "mainTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for lcdTask */
osThreadId_t lcdTaskHandle;
const osThreadAttr_t lcdTask_attributes = {
  .name = "lcdTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for adcTask */
osThreadId_t adcTaskHandle;
const osThreadAttr_t adcTask_attributes = {
  .name = "adcTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for lcd_task */
osThreadId_t lcd_taskHandle;
const osThreadAttr_t lcd_task_attributes = {
  .name = "lcd_task",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for mainMutexHandle */
osMutexId_t mainMutexHandleHandle;
const osMutexAttr_t mainMutexHandle_attributes = {
  .name = "mainMutexHandle"
};
/* Definitions for adcControllerMutex */
osMutexId_t adcControllerMutexHandle;
const osMutexAttr_t adcControllerMutex_attributes = {
  .name = "adcControllerMutex"
};
/* Definitions for lcdControllerMutex */
osMutexId_t lcdControllerMutexHandle;
const osMutexAttr_t lcdControllerMutex_attributes = {
  .name = "lcdControllerMutex"
};
/* Definitions for uartControllerMutex */
osMutexId_t uartControllerMutexHandle;
const osMutexAttr_t uartControllerMutex_attributes = {
  .name = "uartControllerMutex"
};
/* Definitions for adcMutex */
osMutexId_t adcMutexHandle;
const osMutexAttr_t adcMutex_attributes = {
  .name = "adcMutex"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex"
};
/* Definitions for lcdMutex */
osMutexId_t lcdMutexHandle;
const osMutexAttr_t lcdMutex_attributes = {
  .name = "lcdMutex"
};
/* USER CODE BEGIN PV */
__IO uint16_t uhADCxConvertedValue = 0;
uint8_t         ubAnalogWatchdogStatus = RESET;

ADC_ChannelConfTypeDef sConfig = {0};
GPIO_InitTypeDef GPIO_InitStruct = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_LTDC_Init(void);
static void MX_FMC_Init(void);
static void MX_ADC1_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_SPI2_Init(void);
static void MX_UART4_Init(void);
static void MX_UART8_Init(void);
static void MX_UART5_Init(void);
void mainFunction(void *argument);
void uartFunction(void *argument);
void adcFunction(void *argument);
void lcd_function(void *argument);

/* USER CODE BEGIN PFP */
void RESTORE_ADC();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


char rxBuffer[1024];
uint8_t flag = 0x00;
char uartBuffer[1024] = {};
uint16_t *BUFFER_LAYER_1;
uint16_t *BUFFER_LAYER_2;
uint32_t ADC_THREAD_CONTROL_BIT;
uint32_t UART_THREAD_CONTROL_BIT;
uint32_t LCD_THREAD_CONTROL_BIT;

uint32_t MAIN_THREAD_ADC_CONTROL_BIT;
uint32_t MAIN_THREAD_UART_CONTROL_BIT;
uint32_t MAIN_THREAD_LCD_CONTROL_BIT;

DATA_HOLDER mainDataHolder;
DATA_HOLDER lcdDataHolder;

CMD cmd = {
		.origin = {0},
		.protocol = {0},
		.command = {0},
		.args = {0},
		.cmd_all = {0},
		.cmd_all_tmp = {0},
		.cmd_flag = 0x00,
		.status_busy = false
};

Button button = {
		  .c = {0},
		  .data = 25,
		  .n = 0,
		  .rst = false,
		  .set = false,
		  .temp_set = 0,
		  .valid = false,
		  .value = 0
};
Pressed pressed = {
		  .X_ = 0,
		  .Y_=0
};
char msg2[100];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	strcpy(mainDataHolder.callingWaterTemp, "calling");
	strcpy(mainDataHolder.hotWaterTemp, "hot");
	strcpy(mainDataHolder.motor_position, "motor");
	strcpy(mainDataHolder.showerWaterTemp, "shower");

	lcdDataHolder.setTemp = 25;
	//uint16_t *BUFFER_LAYER_1 = (uint16_t *)BUFFER_LAYER_1_ADDRESS;
	//uint16_t *BUFFER_LAYER_2 = (uint16_t *)BUFFER_LAYER_2_ADDRESS;

    //HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)BUFFER_LAYER_1, (uint16_t *)RGB565_Dark_sky_blue, FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);


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
  MX_DMA_Init();
  MX_LTDC_Init();
  MX_FMC_Init();
  MX_ADC1_Init();
  MX_QUADSPI_Init();
  MX_SPI2_Init();
  MX_UART4_Init();
  MX_UART8_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  /* Infinite loop */
	// Allocate buffer

	  int y_ = 0;

	  uint16_t yellow_rgb565 = RGB565_Solid_pink;
	  for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
	          for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
	        	  uint32_t index = y * FRAMEBUFFER_WIDTH + x; // 1D index
	        	  uint32_t address = (uint32_t)BUFFER_LAYER_1_ADDRESS + index * BYTES_PER_PIXEL; // SDRAM address
	              HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)address, &yellow_rgb565, 1);
	              address += BYTES_PER_PIXEL; // Increment by 2 bytes for each pixel
	          }
	      }
	  HAL_Delay(1000);

	//LCD_Fill(BUFFER_LAYER_1_ADDRESS, 0, 0,FRAMEBUFFER_WIDTH,FRAMEBUFFER_HEIGHT,RGB565_Celtic_blue);
	ESP_RST_GPIO_Port->BSRR = 1U << (4+16);
	ESP_EN_GPIO_Port->BSRR = 1U << (5+16);
	HAL_Delay(1000);
	ESP_RST_GPIO_Port->BSRR = 1U << (4);
	ESP_EN_GPIO_Port->BSRR = 1U << 5;

	//sprintf(msg2, "JUST GOT HERE 7\n");
	//HAL_UART_Transmit(&huart8, (uint8_t *)msg2, strlen(msg2), 1000);

	//draw_terminal(BUFFER_LAYER_1_ADDRESS);
	//keypad(BUFFER_LAYER_1_ADDRESS);
	//draw_futuristic_background(BUFFER_LAYER_1_ADDRESS);
	PAGE_HOME_INIT_VAR();
	PAGE_HOME_Draw(BUFFER_LAYER_1_ADDRESS);
	ESP_RTS_GPIO_Port->BSRR = 1U << (8+16);
	//ESP_RTS_GPIO_Port->BSRR = 1U << (8);
	GPIOC->BSRR = 1U << 2; // YD
	//char msg1[100] = "RUN 2 200";

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of mainMutexHandle */
  mainMutexHandleHandle = osMutexNew(&mainMutexHandle_attributes);

  /* creation of adcControllerMutex */
  adcControllerMutexHandle = osMutexNew(&adcControllerMutex_attributes);

  /* creation of lcdControllerMutex */
  lcdControllerMutexHandle = osMutexNew(&lcdControllerMutex_attributes);

  /* creation of uartControllerMutex */
  uartControllerMutexHandle = osMutexNew(&uartControllerMutex_attributes);

  /* creation of adcMutex */
  adcMutexHandle = osMutexNew(&adcMutex_attributes);

  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

  /* creation of lcdMutex */
  lcdMutexHandle = osMutexNew(&lcdMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of mainTask */
  mainTaskHandle = osThreadNew(mainFunction, NULL, &mainTask_attributes);

  /* creation of lcdTask */
  lcdTaskHandle = osThreadNew(uartFunction, NULL, &lcdTask_attributes);

  /* creation of adcTask */
  adcTaskHandle = osThreadNew(adcFunction, NULL, &adcTask_attributes);

  /* creation of lcd_task */
  lcd_taskHandle = osThreadNew(lcd_function, NULL, &lcd_task_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	//memset(adcReadings, 0, sizeof(adcReadings));
    //HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adcReadings, 1000);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //HAL_SDRAM_Read_8b(&hsdram1, (uint32_t *)BUFFER_LAYER_1, (uint8_t *)myReadData, 4);
	// Reconfigure ADC to use the correct input channel (e.g., ADC_CHANNEL_9 on PB1)
	RESTORE_ADC();
	HAL_ADC_Start(&hadc1);
	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
		y_ = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		if (y_ > 2500) {
			ESP_RTS_GPIO_Port->BSRR = 1U << (8+16);
			scan_(BUFFER_LAYER_1_ADDRESS);
			HAL_Delay(500);
			if (HAL_ADC_Init(&hadc1) != HAL_OK) Error_Handler();
			ESP_RTS_GPIO_Port->BSRR = 1U << (8);
		}
	} else {
		printf_(BUFFER_LAYER_1_ADDRESS, "ADC timeout");
	}

	if(cmd.status_busy == true){
		char *cmdmsg = "BUSY TRUE";
		printf_(BUFFER_LAYER_1_ADDRESS, cmdmsg);
	}
	CMD_HANDLER(BUFFER_LAYER_1_ADDRESS);
	//printf_(buffer2, myReadData);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_VREFINT;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */
	GPIOB->BSRR = (1U << 12);

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};
  LTDC_LayerCfgTypeDef pLayerCfg1 = {0};

  /* USER CODE BEGIN LTDC_Init 1 */



  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 19;
  hltdc.Init.AccumulatedHBP = 86;
  hltdc.Init.AccumulatedVBP = 42;
  hltdc.Init.AccumulatedActiveW = 886;
  hltdc.Init.AccumulatedActiveH = 522;
  hltdc.Init.TotalWidth = 1096;
  hltdc.Init.TotalHeigh = 544;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 800;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 800;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = 800;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = 480;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = 0;
  pLayerCfg1.ImageWidth = 800;
  pLayerCfg1.ImageHeight = 0;
  pLayerCfg1.Backcolor.Blue = 255;
  pLayerCfg1.Backcolor.Green = 255;
  pLayerCfg1.Backcolor.Red = 255;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 40;
  hltdc.Init.VerticalSync = 19;
  hltdc.Init.AccumulatedHBP = 86;
  hltdc.Init.AccumulatedVBP = 42;
  hltdc.Init.AccumulatedActiveW = 886;
  hltdc.Init.AccumulatedActiveH = 522;
  hltdc.Init.TotalWidth = 1096;
  hltdc.Init.TotalHeigh = 544;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 800;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg.FBStartAdress = BUFFER_LAYER_1_ADDRESS;
  pLayerCfg.ImageWidth = FRAMEBUFFER_WIDTH;
  pLayerCfg.ImageHeight = FRAMEBUFFER_HEIGHT;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg1.WindowX0 = 0;
  pLayerCfg1.WindowX1 = INNERBUFFER_WIDTH;
  pLayerCfg1.WindowY0 = 0;
  pLayerCfg1.WindowY1 = INNERBUFFER_HEIGHT;
  pLayerCfg1.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg1.Alpha = 0;
  pLayerCfg1.Alpha0 = 0;
  pLayerCfg1.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  pLayerCfg1.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  pLayerCfg1.FBStartAdress = (uint32_t)BUFFER_LAYER_2_ADDRESS;
  pLayerCfg1.ImageWidth = INNERBUFFER_WIDTH;
  pLayerCfg1.ImageHeight = INNERBUFFER_HEIGHT;
  pLayerCfg1.Backcolor.Blue = 0;
  pLayerCfg1.Backcolor.Green = 0;
  pLayerCfg1.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg1, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART8_Init(void)
{

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = 115200;
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK2;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 6;
  SdramTiming.RowCycleDelay = 6;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 3;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  FMC_SDRAM_CommandTypeDef Command;
   /* Step 1 and Step 2 already done in HAL_SDRAM_Init() */
   /* Step 3: Configure a clock configuration enable command */
    Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK2; // Corrected
    Command.AutoRefreshNumber      = 1;
    Command.ModeRegisterDefinition = 0;
    HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    HAL_Delay(1); /* Step 4: Insert 100 us minimum delay - Min HAL Delay is 1ms */
    /* Step 5: Configure a PALL (precharge all) command */
    Command.CommandMode            = FMC_SDRAM_CMD_PALL; /* Set MODE bits to "010" */
    HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    /* Step 6: Configure an Auto Refresh command */
    Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE; /* Set MODE bits to "011" */
    Command.AutoRefreshNumber      = 2;
    HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    /* Step 7: Program the external memory mode register */
    Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;/*set the MODE bits to "100" */
    Command.ModeRegisterDefinition =  (uint32_t)0 | 0<<3 | 2<<4 | 0<<7 | 1<<9;
    HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
    /* Step 8: Set the refresh rate counter - refer to section SDRAM refresh timer register in RM0455 */
    /* Set the device refresh rate
     * COUNT = [(SDRAM self refresh time / number of row) x  SDRAM CLK] – 20
             = [(64ms/4096) * 100MHz] - 20 = 1562.5 - 20 ~ 1542 */
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 1404);
  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|FLASH_RESET_Pin|ESP_RTS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|LCD_T_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11|FMC_LDQM_Pin|FMC_UDQM_Pin|ESP_RST_Pin
                          |ESP_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 FLASH_RESET_Pin ESP_RTS_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|FLASH_RESET_Pin|ESP_RTS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 LCD_T_CS_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12|LCD_T_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD11 FMC_LDQM_Pin FMC_UDQM_Pin ESP_RST_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_11|FMC_LDQM_Pin|FMC_UDQM_Pin|ESP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_EN_Pin */
  GPIO_InitStruct.Pin = ESP_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PI6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pin : PI7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void RESTORE_ADC(){
	ADC_ChannelConfTypeDef sConfig = {0};
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	osDelay(10);

	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}


// DMA Transfer Complete Callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART8) // Check if it's the UART8 transfer
  {
    uart8_tx_complete = 1;
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_mainFunction */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_mainFunction */
void mainFunction(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
	char msg8[100];
	sprintf(msg8,"Main Function  ###### MAIN #####\n");


	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
    osDelay(500);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_uartFunction */
/**
* @brief Function implementing the lcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uartFunction */
void uartFunction(void *argument)
{
  /* USER CODE BEGIN uartFunction */
  /* Infinite loop */
  for(;;)
  {	char msg8[100];
	sprintf(msg8,"Uart Function  ###### UART #####\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
    osDelay(200);
  }
  /* USER CODE END uartFunction */
}

/* USER CODE BEGIN Header_adcFunction */
/**
* @brief Function implementing the adcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_adcFunction */
void adcFunction(void *argument)
{
  /* USER CODE BEGIN adcFunction */
  int y_;
  /* Infinite loop */
  for(;;)
  {	char msg8[100];
	sprintf(msg8,"ADC Function  ###### ADC #####\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);

	RESTORE_ADC(); // Restore ADC to its starting condition
	HAL_ADC_Start(&hadc1); // Start ADC based on starting condition
	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) { // Read ADC in Polling Mode
		y_ = HAL_ADC_GetValue(&hadc1); // Get Value stored in ADC register
		HAL_ADC_Stop(&hadc1); // Stop Conversion
		if (y_ > 2500) { // Check Reading to identify if it was touched
			scan_(BUFFER_LAYER_1_ADDRESS);
			osDelay(100);

			if (HAL_ADC_Init(&hadc1) != HAL_OK) Error_Handler(); // Make sure it is re-initialized
		}
    }
    osDelay(100);
  }
  /* USER CODE END adcFunction */
}

/* USER CODE BEGIN Header_lcd_function */
/**
* @brief Function implementing the lcd_task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_lcd_function */
void lcd_function(void *argument)
{
  /* USER CODE BEGIN lcd_function */
  /* Infinite loop */
  for(;;)
  {	char msg8[100];
	sprintf(msg8,"LCD Function  ###### LCD #####\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
    osDelay(400);
  }
  /* USER CODE END lcd_function */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
