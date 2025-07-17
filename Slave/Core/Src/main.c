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
#include "types.h"
#include "screen.h"
#include "valves.h"
#include "pumps.h"
#include "temperature_control.h"
#include "timer_us.h"
#include "DS18B20.h"
#include "GUI.h"
#include "test.h"
#include "lcd.h"
#include "FONT.h"
#include "temperature_control.h"
#include "stm32f4xx_hal_flash.h"
#include "serial_printing.h"
#include "esp.h"
#include "dma.h"
#include "home_page_layout.h"
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
CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi4;
DMA_HandleTypeDef hdma_spi2_tx;
DMA_HandleTypeDef hdma_spi4_rx;
DMA_HandleTypeDef hdma_spi4_tx;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart2_tx;

/* Definitions for Main */
osThreadId_t MainHandle;
const osThreadAttr_t Main_attributes = {
  .name = "Main",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Valve */
osThreadId_t ValveHandle;
const osThreadAttr_t Valve_attributes = {
  .name = "Valve",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Uart */
osThreadId_t UartHandle;
const osThreadAttr_t Uart_attributes = {
  .name = "Uart",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ds18b20 */
osThreadId_t ds18b20Handle;
const osThreadAttr_t ds18b20_attributes = {
  .name = "ds18b20",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for HuartWorkerTran */
osThreadId_t HuartWorkerTranHandle;
const osThreadAttr_t HuartWorkerTran_attributes = {
  .name = "HuartWorkerTran",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for HuartWorkerRece */
osThreadId_t HuartWorkerReceHandle;
const osThreadAttr_t HuartWorkerRece_attributes = {
  .name = "HuartWorkerRece",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow4,
};
/* Definitions for ds18b20WorkerTr */
osThreadId_t ds18b20WorkerTrHandle;
const osThreadAttr_t ds18b20WorkerTr_attributes = {
  .name = "ds18b20WorkerTr",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow1,
};
/* Definitions for VALVE_WORKER_RE */
osThreadId_t VALVE_WORKER_REHandle;
const osThreadAttr_t VALVE_WORKER_RE_attributes = {
  .name = "VALVE_WORKER_RE",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for ScreenWorkerRec */
osThreadId_t ScreenWorkerRecHandle;
const osThreadAttr_t ScreenWorkerRec_attributes = {
  .name = "ScreenWorkerRec",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ScreenWorkerTra */
osThreadId_t ScreenWorkerTraHandle;
const osThreadAttr_t ScreenWorkerTra_attributes = {
  .name = "ScreenWorkerTra",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Backup */
osThreadId_t BackupHandle;
const osThreadAttr_t Backup_attributes = {
  .name = "Backup",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BackupWorkerRec */
osThreadId_t BackupWorkerRecHandle;
const osThreadAttr_t BackupWorkerRec_attributes = {
  .name = "BackupWorkerRec",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for BackupWorkerTra */
osThreadId_t BackupWorkerTraHandle;
const osThreadAttr_t BackupWorkerTra_attributes = {
  .name = "BackupWorkerTra",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ValveWorkerRece */
osThreadId_t ValveWorkerReceHandle;
const osThreadAttr_t ValveWorkerRece_attributes = {
  .name = "ValveWorkerRece",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow3,
};
/* Definitions for Screen */
osThreadId_t ScreenHandle;
const osThreadAttr_t Screen_attributes = {
  .name = "Screen",
  .stack_size = 4048 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TouchScreen */
osThreadId_t TouchScreenHandle;
const osThreadAttr_t TouchScreen_attributes = {
  .name = "TouchScreen",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime,
};
/* Definitions for SerialPrint */
osThreadId_t SerialPrintHandle;
const osThreadAttr_t SerialPrint_attributes = {
  .name = "SerialPrint",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ESP_BUFFER_RE */
osMessageQueueId_t ESP_BUFFER_REHandle;
const osMessageQueueAttr_t ESP_BUFFER_RE_attributes = {
  .name = "ESP_BUFFER_RE"
};
/* Definitions for ESP_BUFFER_TR */
osMessageQueueId_t ESP_BUFFER_TRHandle;
const osMessageQueueAttr_t ESP_BUFFER_TR_attributes = {
  .name = "ESP_BUFFER_TR"
};
/* Definitions for VALVE_BUFFER_RE */
osMessageQueueId_t VALVE_BUFFER_REHandle;
const osMessageQueueAttr_t VALVE_BUFFER_RE_attributes = {
  .name = "VALVE_BUFFER_RE"
};
/* Definitions for VALVE_BUFFER_TR */
osMessageQueueId_t VALVE_BUFFER_TRHandle;
const osMessageQueueAttr_t VALVE_BUFFER_TR_attributes = {
  .name = "VALVE_BUFFER_TR"
};
/* Definitions for SerialBuffer */
osMessageQueueId_t SerialBufferHandle;
const osMessageQueueAttr_t SerialBuffer_attributes = {
  .name = "SerialBuffer"
};
/* Definitions for DS18B20_BUFFER */
osMessageQueueId_t DS18B20_BUFFERHandle;
const osMessageQueueAttr_t DS18B20_BUFFER_attributes = {
  .name = "DS18B20_BUFFER"
};
/* Definitions for LCD_BUFFER_TRANSMIT */
osMessageQueueId_t LCD_BUFFER_TRANSMITHandle;
const osMessageQueueAttr_t LCD_BUFFER_TRANSMIT_attributes = {
  .name = "LCD_BUFFER_TRANSMIT"
};
/* Definitions for valveMutex */
osMutexId_t valveMutexHandle;
const osMutexAttr_t valveMutex_attributes = {
  .name = "valveMutex"
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex"
};
/* Definitions for ds18b20Mutex */
osMutexId_t ds18b20MutexHandle;
const osMutexAttr_t ds18b20Mutex_attributes = {
  .name = "ds18b20Mutex"
};
/* Definitions for HUART1 */
osMutexId_t HUART1Handle;
const osMutexAttr_t HUART1_attributes = {
  .name = "HUART1"
};
/* Definitions for HUART2 */
osMutexId_t HUART2Handle;
const osMutexAttr_t HUART2_attributes = {
  .name = "HUART2"
};
/* Definitions for UART_RTS */
osEventFlagsId_t UART_RTSHandle;
const osEventFlagsAttr_t UART_RTS_attributes = {
  .name = "UART_RTS"
};
/* Definitions for UART_CTS */
osEventFlagsId_t UART_CTSHandle;
const osEventFlagsAttr_t UART_CTS_attributes = {
  .name = "UART_CTS"
};
/* Definitions for ESP_RECEIVE */
osEventFlagsId_t ESP_RECEIVEHandle;
const osEventFlagsAttr_t ESP_RECEIVE_attributes = {
  .name = "ESP_RECEIVE"
};
/* Definitions for ESP_SEND */
osEventFlagsId_t ESP_SENDHandle;
const osEventFlagsAttr_t ESP_SEND_attributes = {
  .name = "ESP_SEND"
};
/* Definitions for TOUCH_SCREEN_FLAG */
osEventFlagsId_t TOUCH_SCREEN_FLAGHandle;
const osEventFlagsAttr_t TOUCH_SCREEN_FLAG_attributes = {
  .name = "TOUCH_SCREEN_FLAG"
};
/* USER CODE BEGIN PV */


osMemoryPoolId_t SETTINGS_POOLHandle;
const osMemoryPoolAttr_t SETTINGS_POOL_attributes ={
		.name = "SETTINGS_POOL"
};

osMemoryPoolId_t CANVAS_POOLHandle;
const osMemoryPoolAttr_t CANVAS_POOL_attributes ={
		.name = "CANVAS"
};

osMemoryPoolId_t PAGE_POOLHandle;
const osMemoryPoolAttr_t PAGE_POOL_attributes ={
		.name = "PAGE"
};

osMemoryPoolId_t VIEW_POOLHandle;
const osMemoryPoolAttr_t VIEW_POOL_attributes ={
		.name = "VIEW"
};

osMemoryPoolId_t NODE_POOLHandle;
const osMemoryPoolAttr_t NODE_POOL_attributes ={
		.name = "NODE"
};






uint8_t buffer[1];
uint8_t flag;
uint8_t act;
uint8_t command;
uint8_t page;
uint8_t temp_step;
float calc;
float hot_temp;
float terminal_temp;

uint32_t address = 0x08060000;
uint32_t *p = (uint32_t *)0x08060000;

DS18B20 ds18b20_terminal = {
	.SERIAL_NUMBER = {0x28, 0x50, 0x3A, 0x85, 0x00, 0x00, 0x00, 0x51},
	.temperature = 0,
	.delta = 0,
	.pos = 7
};
DS18B20 ds18b20_hot = {
	.SERIAL_NUMBER = {0x28, 0x7F, 0xF8, 0x87, 0x00, 0x00, 0x00, 0x65},
	.temperature = 0,
	.delta = 0,
	.pos = 6
};

Water water;
Valve valve;
ERR err;
ERR_STRING err_string;

Valve valve ={
		.current_steps = 0,
		.degPerStep = 1.8,
		.steps = 0,
		.save_step = 0,
		.aprox = 0
};

Pressed pressed ={
		.Y_ = 0x00, // 0b0000 0 <1,4,7, CLR> <2, 5, 8, 0> <3, 6, 9, SET>
		.X_ = 0x00	// 0b<1, 2, 3> <4, 5, 6> <7, 8, 9> <CLR, 0, SET> 0000
};

Button button ={
		.c = {0},
		.value = -1,
		.data = 0,
		.valid = false,
		.n = 0,
		.rst = false,
		.set = false,
		.temp_set = 0
};


CMD cmd = {
	.args[0] = {0},
	.args[1] = {0},
	.args[2] = {0},
	.args[3] = {0},
	.cmd_all = {0},
	.command = {0},
	.origin = {0},
	.protocol = {0},
	.status_busy = false
};

uint8_t rxBuffer[2] = {0};
uint32_t FLAG_UART = 0x00;
uint32_t len = 1024;

ESP_DATA_CONTROL esp = {
	.checksum = 0x00,
	.esp_uart_buffer_rx = {0},
	.esp_uart_buffer_tx = {0},
	.rx_status = HAL_BUSY,
	.tx_status = HAL_BUSY,
	.huart = &huart1,
	.checksum_s = {0},
	.esp_buffer = {0},
	.huart_test = &huart2,
	.ESP_BUFFER_REHandle = &ESP_BUFFER_REHandle,
	.ESP_BUFFER_TRHandle = &ESP_BUFFER_TRHandle
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM10_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM11_Init(void);
static void MX_SPI4_Init(void);
static void MX_TIM2_Init(void);
static void MX_CRC_Init(void);
void mainFunction(void *argument);
void valveFunction(void *argument);
void uartFunction(void *argument);
void ds18b20Function(void *argument);
void HUART_WORKER_TRANSMIT(void *argument);
void HUART_WORKER_RECIVE(void *argument);
void DS18B20_WORKER_TR(void *argument);
void StartTask08(void *argument);
void SCREEN_WORKER_RECEIVE(void *argument);
void SCREEN_WORKER_TRANSMIT(void *argument);
void BACKUP_FUNCTION(void *argument);
void BACKUP_WORKER_RECEIVE(void *argument);
void BACKUP_WORKER_TRANSMIT(void *argument);
void VALVE_WORKER_RECEIVE(void *argument);
void ScreenFunction(void *argument);
void TouchScreenFunction(void *argument);
void PRINT(void *argument);

/* USER CODE BEGIN PFP */
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void getFloatString(char *buffer, float number);

 float temp_set_f;
 char temp_set_c[1];
 char temp_set_c_number[30];


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
	//esp.ESP_BUFFER_REHandle = ESP_BUFFER_REHandle;
	//esp.ESP_BUFFER_TRHandle = ESP_BUFFER_TRHandle;
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM10_Init();
  MX_SPI2_Init();
  MX_TIM11_Init();
  MX_SPI4_Init();
  MX_TIM2_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init();
  HAL_Delay(1000);
  LCD_DrawPage1();
  HAL_Delay(1000);
  TIMER_US_INIT();
  //VALVE_MODE_FULL_CW();

  flag = 0x00;
  page = 0x01;
  command = 0x0;
  act = 0x00;

  //uint32_t *p = (uint32_t *)FLASH_ADDRESS;
  //int h = *p;

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of valveMutex */
  valveMutexHandle = osMutexNew(&valveMutex_attributes);

  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

  /* creation of ds18b20Mutex */
  ds18b20MutexHandle = osMutexNew(&ds18b20Mutex_attributes);

  /* creation of HUART1 */
  HUART1Handle = osMutexNew(&HUART1_attributes);

  /* creation of HUART2 */
  HUART2Handle = osMutexNew(&HUART2_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ESP_BUFFER_RE */
  ESP_BUFFER_REHandle = osMessageQueueNew (16, sizeof(uint32_t), &ESP_BUFFER_RE_attributes);

  /* creation of ESP_BUFFER_TR */
  ESP_BUFFER_TRHandle = osMessageQueueNew (16, sizeof(uint32_t), &ESP_BUFFER_TR_attributes);

  /* creation of VALVE_BUFFER_RE */
  VALVE_BUFFER_REHandle = osMessageQueueNew (16, sizeof(uint32_t), &VALVE_BUFFER_RE_attributes);

  /* creation of VALVE_BUFFER_TR */
  VALVE_BUFFER_TRHandle = osMessageQueueNew (16, sizeof(uint32_t), &VALVE_BUFFER_TR_attributes);

  /* creation of SerialBuffer */
  SerialBufferHandle = osMessageQueueNew (500, sizeof(uint32_t), &SerialBuffer_attributes);

  /* creation of DS18B20_BUFFER */
  DS18B20_BUFFERHandle = osMessageQueueNew (16, sizeof(uint32_t), &DS18B20_BUFFER_attributes);

  /* creation of LCD_BUFFER_TRANSMIT */
  LCD_BUFFER_TRANSMITHandle = osMessageQueueNew (100, sizeof(uint32_t), &LCD_BUFFER_TRANSMIT_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Main */
  MainHandle = osThreadNew(mainFunction, NULL, &Main_attributes);

  /* creation of Valve */
  ValveHandle = osThreadNew(valveFunction, NULL, &Valve_attributes);

  /* creation of Uart */
  UartHandle = osThreadNew(uartFunction, NULL, &Uart_attributes);

  /* creation of ds18b20 */
  ds18b20Handle = osThreadNew(ds18b20Function, NULL, &ds18b20_attributes);

  /* creation of HuartWorkerTran */
  HuartWorkerTranHandle = osThreadNew(HUART_WORKER_TRANSMIT, NULL, &HuartWorkerTran_attributes);

  /* creation of HuartWorkerRece */
  HuartWorkerReceHandle = osThreadNew(HUART_WORKER_RECIVE, NULL, &HuartWorkerRece_attributes);

  /* creation of ds18b20WorkerTr */
  ds18b20WorkerTrHandle = osThreadNew(DS18B20_WORKER_TR, NULL, &ds18b20WorkerTr_attributes);

  /* creation of VALVE_WORKER_RE */
  VALVE_WORKER_REHandle = osThreadNew(StartTask08, NULL, &VALVE_WORKER_RE_attributes);

  /* creation of ScreenWorkerRec */
  ScreenWorkerRecHandle = osThreadNew(SCREEN_WORKER_RECEIVE, NULL, &ScreenWorkerRec_attributes);

  /* creation of ScreenWorkerTra */
  ScreenWorkerTraHandle = osThreadNew(SCREEN_WORKER_TRANSMIT, NULL, &ScreenWorkerTra_attributes);

  /* creation of Backup */
  BackupHandle = osThreadNew(BACKUP_FUNCTION, NULL, &Backup_attributes);

  /* creation of BackupWorkerRec */
  BackupWorkerRecHandle = osThreadNew(BACKUP_WORKER_RECEIVE, NULL, &BackupWorkerRec_attributes);

  /* creation of BackupWorkerTra */
  BackupWorkerTraHandle = osThreadNew(BACKUP_WORKER_TRANSMIT, NULL, &BackupWorkerTra_attributes);

  /* creation of ValveWorkerRece */
  ValveWorkerReceHandle = osThreadNew(VALVE_WORKER_RECEIVE, NULL, &ValveWorkerRece_attributes);

  /* creation of Screen */
  ScreenHandle = osThreadNew(ScreenFunction, NULL, &Screen_attributes);

  /* creation of TouchScreen */
  TouchScreenHandle = osThreadNew(TouchScreenFunction, NULL, &TouchScreen_attributes);

  /* creation of SerialPrint */
  SerialPrintHandle = osThreadNew(PRINT, NULL, &SerialPrint_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of UART_RTS */
  UART_RTSHandle = osEventFlagsNew(&UART_RTS_attributes);

  /* creation of UART_CTS */
  UART_CTSHandle = osEventFlagsNew(&UART_CTS_attributes);

  /* creation of ESP_RECEIVE */
  ESP_RECEIVEHandle = osEventFlagsNew(&ESP_RECEIVE_attributes);

  /* creation of ESP_SEND */
  ESP_SENDHandle = osEventFlagsNew(&ESP_SEND_attributes);

  /* creation of TOUCH_SCREEN_FLAG */
  TOUCH_SCREEN_FLAGHandle = osEventFlagsNew(&TOUCH_SCREEN_FLAG_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  char terminalBuffer[100]; char hotBuffer[100];
  char updateMsg[300];

	while(1){
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		// Handle command flags
		COMMAND_HANDLE(&htim10, &htim2, &command);
		// Handle touch screen interrupt
		if((flag & SCAN_PAD) != 0){
			//SCAN_HANDLE(&hspi4, &huart2, &button, &pressed, &act, &flag, &PAGE);
			flag &= (~SCAN_PAD);}

		// Update temperatures, storing them in the Struct.
		if((act & TEMP_UPDATE_) == TEMP_UPDATE_){
			LCD_typeDebug("reading temperature");
			terminal_temp = ds18b20_get_temperature(&ds18b20_terminal, false, &huart2);
			hot_temp = ds18b20_get_temperature(&ds18b20_hot, false, &huart2);

			ds18b20_terminal.delta = terminal_temp - ds18b20_terminal.temperature;
			ds18b20_hot.delta = hot_temp - ds18b20_hot.temperature;

			ds18b20_terminal.temperature = terminal_temp;
			ds18b20_hot.temperature = hot_temp;
			act = act & (~TEMP_UPDATE_);
		    // Update the User's screen controller
			getFloatString(terminalBuffer, terminal_temp);
			getFloatString(hotBuffer, hot_temp);
			sprintf(updateMsg, "UPDATE %d %s %s %d", button.data, terminalBuffer, hotBuffer, valve.current_steps);
		    HAL_UART_Transmit(&huart1, (uint8_t *)updateMsg,strlen(updateMsg), 100);}


		if(((act & TEMP_GET_VALV_APROX) == TEMP_GET_VALV_APROX)){
			VALVE_CONTROL_APROX(&valve); // Open Valve to approximate position
			act &= (~TEMP_GET_VALV_APROX);
			command |= RUN;} // Clear bit flag

		if(((command & RUN) == RUN) && (command & RUN_) == RUN_){
			VALVE_CONTROL_RUN(&valve, &ds18b20_terminal, &button);
			command = command & (~RUN_);}

		switch (page){
			case 0x01: // Home Page
				//LCD_HANDLE_PAGE_1(&PAGE, &act, &command);
				break;
			case 0x02: // Readings Page
				break;
		}

		HAL_Delay(10);
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
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
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
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

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
  htim2.Init.Prescaler = 59999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 79999;
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

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 15999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 59999;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 29999;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 0;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */


  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  esp.rx_status = HAL_OK;
  esp.tx_status = HAL_OK;
  /* USER CODE END USART1_Init 2 */

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
  /* DMA2_Stream7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Valve_M1_Pin|Valve_M2_Pin|Valve_M3_Pin|Valve_EN_Pin
                          |Valve_RST_Pin|Valve_CLK_Pin|Valve_CCW_Pin|Valve_LATCH_Pin
                          |Valve_TQ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ESP_RST_Pin|ESP_IO4_Pin|ESP_IO5_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_RST_Pin|LCD_DC_Pin|T_CS_Pin|GPIO_PIN_7
                          |ESP_EN_Pin|LCD_CSB9_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OneWire_GPIO_Port, OneWire_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Valve_M1_Pin Valve_M2_Pin Valve_M3_Pin Valve_EN_Pin
                           Valve_RST_Pin Valve_CLK_Pin Valve_CCW_Pin Valve_LATCH_Pin
                           Valve_TQ_Pin */
  GPIO_InitStruct.Pin = Valve_M1_Pin|Valve_M2_Pin|Valve_M3_Pin|Valve_EN_Pin
                          |Valve_RST_Pin|Valve_CLK_Pin|Valve_CCW_Pin|Valve_LATCH_Pin
                          |Valve_TQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ESP_RST_Pin */
  GPIO_InitStruct.Pin = ESP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ESP_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ESP_IO4_Pin ESP_IO5_Pin */
  GPIO_InitStruct.Pin = ESP_IO4_Pin|ESP_IO5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_DC_Pin T_CS_Pin PB7
                           ESP_EN_Pin LCD_CSB9_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_DC_Pin|T_CS_Pin|GPIO_PIN_7
                          |ESP_EN_Pin|LCD_CSB9_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Valve_ALERT_Pin */
  GPIO_InitStruct.Pin = Valve_ALERT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Valve_ALERT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OneWire_Pin */
  GPIO_InitStruct.Pin = OneWire_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OneWire_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
  HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == GPIO_PIN_8)
	    {
		static uint32_t last_interrupt_tick = 0;
		uint32_t current_tick = HAL_GetTick();

		// This is the debounce logic:
		// Only process this interrupt if more than 50ms have passed since the last one.
		if (current_tick - last_interrupt_tick > 50)
		{
			// This is a valid, debounced event.
			// Update the time of the last valid interrupt.
			last_interrupt_tick = current_tick;

			// Now, safely set your event flag.
			// This needs to be the ISR-safe version if you are using native FreeRTOS.
			// Since you are using CMSIS-OS v2, you might need a different mechanism
			// like osEventFlagsSet from another task, or better yet, a semaphore.
			// Let's assume you have a semaphore for this:
			 osEventFlagsSet(TOUCH_SCREEN_FLAGHandle, SCAN_PAD);
		}
		// If an interrupt arrives within the 50ms window, it is ignored.
	}
}

void getFloatString(char *buffer, float number){
	int intNumber = (int)floor(number);
	int floatNumber = (int)((number - (float)intNumber)*10000);
	sprintf(buffer, "%d.%d°C\n", intNumber, floatNumber);
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if (huart->Instance == USART1) {
        SET_CTS;      // Clear CTS
        FLAG_UART |= ESP_MSG_COMPLETE;
        FLAG_UART |= ESP_UART_RELEASE_MUTEX;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    // Your custom code here
	if(huart->Instance == USART1){
		FLAG_UART |= ESP_MSG_ACK_SENT;
	}
}

void ERR_INIT(ERR_STRING *err_String){
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_mainFunction */
/**
  * @brief  Function implementing the Main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_mainFunction */
void mainFunction(void *argument)
{
  /* USER CODE BEGIN 5 */
	osDelay(200);
	//char msg[100] = "MAIN FUNCTION THREAD\n";
	TickType_t MUTEX_WAIT = 1000;
	osStatus status;
  /* Infinite loop */
  for(;;)
  {	//SerialPrint("MAIN FUNCTION THREAD\n");
	 //HAL_UART_Transmit(&huart2, (uint8_t *)msg,strlen(msg), 100);

    osDelay(1000);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_valveFunction */
/**
* @brief Function implementing the Valve thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_valveFunction */
void valveFunction(void *argument)
{
  /* USER CODE BEGIN valveFunction */
	osDelay(200);
	VALVE_MODE_FULL_CW();
	//char msg[100] = "VALVE FUNCTION THREAD\n";
	//TickType_t MUTEX_WAIT = 1000;
	//osStatus status;
	int count = 0, flag = 0;
  /* Infinite loop */
  for(;;)
  {
	if(count == 0){
		flag = 1;
	}
	else if(count == 250){
		flag = 0;
	}


	if(flag == 1){
		VALVE_CONTROL_CW_STEP(1, &valve);
		count++;
	}
	else{
		VALVE_CONTROL_CCW_STEP(1, &valve);
		count--;
	}
    osDelay(50);
  }
  /* USER CODE END valveFunction */
}

/* USER CODE BEGIN Header_uartFunction */
/**
* @brief Function implementing the Uart thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_uartFunction */
void uartFunction(void *argument)
{
  /* USER CODE BEGIN uartFunction */
	  osDelay(1000);
	  MX_USART1_UART_Init();
	  FLAG_UART = 0x00;
	  SET_CTS;
	  SET_RTS;

	  GPIOB->BSRR = (1U << (16 + 8));
	  GPIOA->BSRR = (1U << (16));
	  osDelay(1000);
	  GPIOA->BSRR = (1U << (0));
	  GPIOB->BSRR = (1U << 8); // Enable ESP
	  T_CS_SET;
	  osDelay(1000);

	  if(esp.rx_status == HAL_OK && esp.tx_status == HAL_OK){
		  for(;;)
			{
			  if((FLAG_UART & ESP_MSG_ACK) && !(GPIOA->IDR & GPIO_IDR_ID7)){
				  ESP_UART_ACK(&esp);

			  }
			  else if(!(GPIOA->IDR & GPIO_IDR_ID6) && !(FLAG_UART & ESP_RTS)){
				  ESP_START_PROTOCOL(&esp);

			  }
			  ESP_HAND_SHAKE_HANDLE(&esp);

			 if(FLAG_UART != ESP_UART_RELEASE_MUTEX){
				 osMutexRelease(HUART1Handle);
				 FLAG_UART &= ~(ESP_UART_RELEASE_MUTEX);
			 }
			 //SerialPrint("UART THREAD\n");
			 osDelay(2000);
		   }
	  }

  /* USER CODE END uartFunction */
}

/* USER CODE BEGIN Header_ds18b20Function */
/**
* @brief Function implementing the ds18b20 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ds18b20Function */
void ds18b20Function(void *argument)
{
  /* USER CODE BEGIN ds18b20Function */
	osDelay(200);
	char temperature[100];
	memset(temperature, 0, sizeof(temperature));
	//TickType_t MUTEX_WAIT = 1000;
	uint8_t prio = 5;
	//osStatus status;
	char *ptr;
  //char msg[100] = "DS18B20 FUNCTION THREAD\n";
  /* Infinite loop */
  for(;;)
  {
	terminal_temp = ds18b20_get_temperature(&ds18b20_terminal, false, &huart2);
	hot_temp = ds18b20_get_temperature(&ds18b20_hot, false, &huart2);

	ds18b20_terminal.delta = terminal_temp - ds18b20_terminal.temperature;
	ds18b20_hot.delta = hot_temp - ds18b20_hot.temperature;

	ds18b20_terminal.temperature = terminal_temp;
	ds18b20_hot.temperature = hot_temp;

	getFloatString(temperature, ds18b20_hot.temperature);
	ptr = temperature;
	osMessageQueuePut(DS18B20_BUFFERHandle, &ptr, prio, osWaitForever);

	getFloatString(temperature, ds18b20_terminal.temperature);
	ptr = temperature;
	osMessageQueuePut(DS18B20_BUFFERHandle, &ptr, prio, osWaitForever);
	//SerialPrint("DS18B20 THREAD\n");
    osDelay(4000);
  }
  /* USER CODE END ds18b20Function */
}

/* USER CODE BEGIN Header_HUART_WORKER_TRANSMIT */
/**
* @brief Function implementing the HuartWorkerTran thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HUART_WORKER_TRANSMIT */
void HUART_WORKER_TRANSMIT(void *argument)
{
  /* USER CODE BEGIN HUART_WORKER_TRANSMIT */
  /* Infinite loop */
  for(;;)
  {	//SerialPrint("HUART WORKER TRANSMIT THREAD \n");
    osDelay(2000);
  }
  /* USER CODE END HUART_WORKER_TRANSMIT */
}

/* USER CODE BEGIN Header_HUART_WORKER_RECIVE */
/**
* @brief Function implementing the HuartWorkerRece thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_HUART_WORKER_RECIVE */
void HUART_WORKER_RECIVE(void *argument)
{
  /* USER CODE BEGIN HUART_WORKER_RECIVE */
  osStatus_t status;
  uint8_t prio = 5;
  char *ptr;
  /* Infinite loop */
  for(;;)
  {
		//SerialPrint("HUART WORKER RECEIVE THREAD\n");
	status = osMessageQueueGet(ESP_BUFFER_REHandle, &ptr, &prio, osWaitForever);
	if(status == osOK){
		strcpy(cmd.cmd_all, ptr);
		PARSE_COMMAND(&cmd);
		//print_CMD(&huart2, &cmd);
	}
    osDelay(100);
  }
  /* USER CODE END HUART_WORKER_RECIVE */
}

/* USER CODE BEGIN Header_DS18B20_WORKER_TR */
/**
* @brief Function implementing the ds18b20WorkerTr thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_DS18B20_WORKER_TR */
void DS18B20_WORKER_TR(void *argument)
{
  /* USER CODE BEGIN DS18B20_WORKER_TR */
	osStatus_t status;
	uint8_t prio = 5;
	char *ptr;
  /* Infinite loop */
  for(;;)
  {
	 // SerialPrint("DS18B20 WORKER TRANSMIT THREA\n");
	//status = osMessageQueueGet(DS18B20_BUFFERHandle, &ptr, &prio, osWaitForever);
	//if(status == osOK){
		//osMutexAcquire()
	//}
    osDelay(10000);
  }
  /* USER CODE END DS18B20_WORKER_TR */
}

/* USER CODE BEGIN Header_StartTask08 */
/**
* @brief Function implementing the VALVE_WORKER_RE thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask08 */
void StartTask08(void *argument)
{
  /* USER CODE BEGIN StartTask08 */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("START TASK 08 THREAD \n");
    osDelay(2000);
  }
  /* USER CODE END StartTask08 */
}

/* USER CODE BEGIN Header_SCREEN_WORKER_RECEIVE */
/**
* @brief Function implementing the ScreenWorkerRec thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SCREEN_WORKER_RECEIVE */
void SCREEN_WORKER_RECEIVE(void *argument)
{
  /* USER CODE BEGIN SCREEN_WORKER_RECEIVE */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("SCREEN WORKER RECEIVE THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END SCREEN_WORKER_RECEIVE */
}

/* USER CODE BEGIN Header_SCREEN_WORKER_TRANSMIT */
/**
* @brief Function implementing the ScreenWorkerTra thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SCREEN_WORKER_TRANSMIT */
void SCREEN_WORKER_TRANSMIT(void *argument)
{
  /* USER CODE BEGIN SCREEN_WORKER_TRANSMIT */
  /* Infinite loop */
  for(;;)
  {
	 // ESP_RECEIVEHandle
	//osEventFlagsWait(ESP_RECEIVEHandle);
	//SerialPrint("SCREEN WORKER TRANSMIT THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END SCREEN_WORKER_TRANSMIT */
}

/* USER CODE BEGIN Header_BACKUP_FUNCTION */
/**
* @brief Function implementing the Backup thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BACKUP_FUNCTION */
void BACKUP_FUNCTION(void *argument)
{
  /* USER CODE BEGIN BACKUP_FUNCTION */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("BACKUP FUNCTION THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END BACKUP_FUNCTION */
}

/* USER CODE BEGIN Header_BACKUP_WORKER_RECEIVE */
/**
* @brief Function implementing the BackupWorkerRec thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BACKUP_WORKER_RECEIVE */
void BACKUP_WORKER_RECEIVE(void *argument)
{
  /* USER CODE BEGIN BACKUP_WORKER_RECEIVE */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("BACKUP WORKER RECEIVE THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END BACKUP_WORKER_RECEIVE */
}

/* USER CODE BEGIN Header_BACKUP_WORKER_TRANSMIT */
/**
* @brief Function implementing the BackupWorkerTra thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_BACKUP_WORKER_TRANSMIT */
void BACKUP_WORKER_TRANSMIT(void *argument)
{
  /* USER CODE BEGIN BACKUP_WORKER_TRANSMIT */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("BACKUP WORKER TRANSMIT THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END BACKUP_WORKER_TRANSMIT */
}

/* USER CODE BEGIN Header_VALVE_WORKER_RECEIVE */
/**
* @brief Function implementing the ValveWorkerRece thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_VALVE_WORKER_RECEIVE */
void VALVE_WORKER_RECEIVE(void *argument)
{
  /* USER CODE BEGIN VALVE_WORKER_RECEIVE */
  /* Infinite loop */
  for(;;)
  {
	//SerialPrint("VALVE WORKER RECEIVE THREAD\n");
    osDelay(2000);
  }
  /* USER CODE END VALVE_WORKER_RECEIVE */
}

/* USER CODE BEGIN Header_ScreenFunction */
/**
* @brief Function implementing the Screen thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ScreenFunction */
void ScreenFunction(void *argument)
{
  /* USER CODE BEGIN ScreenFunction */
	uint8_t canvas_count = 60;
	uint8_t pages_count = 2;

//hspi4
	//flag = osEventFlagsWait(TOUCH_SCREEN_FLAGHandle, flag_all, osFlagsWaitAny, 0);
  /* Infinite loop */
  CANVAS_POOLHandle = osMemoryPoolNew(canvas_count, sizeof(Canvas_t), NULL);
  SETTINGS_POOLHandle = osMemoryPoolNew(canvas_count, sizeof(struct CanvasConf_t), NULL);
  PAGE_POOLHandle = osMemoryPoolNew(pages_count, sizeof(Page_t), NULL);
  NODE_POOLHandle = osMemoryPoolNew(canvas_count, sizeof(struct _nodeCanvas), NULL);
  //uint32_t flag;
  //uint32_t index = (0x03)<<30;
  //uint32_t flag_all = 0xFFFFFFFF & ~(0x01);

  /*Initialize Screen, View and Canvas*/

  //GUI_INIT(&screen);

  //LCD_DrawPage1();
  if(SCREEN_INIT() != 0) return;
  //LCD_UpdateTemperature(&ds18b20_terminal, ds18b20_terminal.temperature);
  //LCD_UpdateTemperature(&ds18b20_hot, ds18b20_hot.temperature);
  for(;;)
  {
	//GUI_DRAW_PAGE(screen.home_page);
	SerialPrint("LOOPING\n");
	GUI_HANDLE();
    osDelay(500);
  }
  /* USER CODE END ScreenFunction */
}

/* USER CODE BEGIN Header_TouchScreenFunction */
/**
* @brief Function implementing the TouchScreen thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TouchScreenFunction */
void TouchScreenFunction(void *argument)
{
  /* USER CODE BEGIN TouchScreenFunction */
  /* Infinite loop */
  Scan scan_reading;
  scan_reading.x = 16;
  scan_reading.y = 50;
  char s[40];
  uint32_t f = 0;
  for(;;)
  {
	f = osEventFlagsWait(TOUCH_SCREEN_FLAGHandle, 0x01, osFlagsWaitAny, osWaitForever);
	if(f & SCAN_PAD){
		scan(&hspi4, &scan_reading);

		if(scan_reading.x > 0){
			sprintf(s, "rx-> %d | ry -> %d\n", (int)scan_reading.x, (int)scan_reading.y);
			SerialPrint(s);
		}
	}
	else{
		print_OS_Status((osThreadState_t)flag);
		SerialPrint("TouchScreenFunction -> FAILED TO GET FLAG\n");
	}
	osEventFlagsClear(TOUCH_SCREEN_FLAGHandle, (uint32_t)SCAN_PAD);
    osDelay(100);
  }
  /* USER CODE END TouchScreenFunction */
}

/* USER CODE BEGIN Header_PRINT */
/**
* @brief Function implementing the SerialPrint thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PRINT */
void PRINT(void *argument)
{
  /* USER CODE BEGIN PRINT */
	//osMessageQueueId_t SerialBufferHandle;
	osStatus_t status;
	uint8_t prio = 5;
	char *ptr;
  /* Infinite loop */
  for(;;)
  {
	status = osMessageQueueGet(SerialBufferHandle, &ptr, &prio, osWaitForever);
	if(status == osOK){
		if(osMutexAcquire(HUART2Handle, osWaitForever) == osOK){
			HAL_UART_Transmit(&huart2, (uint8_t *)ptr, strlen(ptr), 100);
			osMutexRelease(HUART2Handle);
		}
	}
	else{
		print_OS_Status(status);
	}
    osDelay(50);
  }
  /* USER CODE END PRINT */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
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
