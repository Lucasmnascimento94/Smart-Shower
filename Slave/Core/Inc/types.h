
#ifndef TYPES_H
#define TYPES_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "main.h"

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define FLASH_ADDRESS 0x08060000
/* flag */
#define SCAN_PAD 0x80

/* act */
#define TEMP_UPDATE 0x01
#define TEMP_UPDATE_ 0x02
#define TEMP_GET_TEMP_SET 0x04
#define TEMP_GET_VALV_APROX 0x08
#define TEMP_CONTROL_TEMP 0x10
#define CONTROLLER_COMMAND 0x20

/* command */
#define START	 0x01
#define IDLE	 0x02
#define STALL_START 0x04
#define STALL_STOP 0x08
#define STALL_ACTIVE 0x10
#define RUN 0x20
#define RUN_ 0x40

#define SCREEN_PAGE1 0x01
#define SCREEN_PAGE2 0x02
#define SCREEN_PAGE3 0x04
#define SCREEN_PAGE4 0x08


#define VALVE_ADJUST_COLD 0x10
#define VALVE_ADJUST_HOT 0x20
#define VALVE_ADJUST_STOP 0x30

#define SET_NEW_TEMP 			0x10000
#define READ_TERMINAL_TEMP		0x20000
#define READ_COLD_TEMP			0x30000
#define READ_HOT_TEMP			0x40000


#define RUNNING					0x01000000
#define IDLED					0x02000000
#define delay_ms 				HAL_Delay


#define feed_cold_water (valve1_A__GPIO_Port->BSRR = 1U << 3);
#define return_cold_water (valve1_A__GPIO_Port->BSRR = 1U << (3 + 16));

#define feed_hot_water (valve1_A__GPIO_Port->BSRR = 1U << 2);
#define return_hot_water (valve1_A__GPIO_Port->BSRR = 1U << (2 + 16));

#define DS18B20_PORT GPIOB
#define DS18B20_PIN GPIO_PIN_5
#define MAX_DS18B20 3
#define N_DS18B20_SERIAL_NUMBER_BYTES 9 // 1 for null
#define COLD_TEMP 15

#define LCD_CS_SET (LCD_CS_GPIO_Port->BSRR = 1U << 0)
#define LCD_CLK_SET (LCD_CLK_GPIO_Port->BSRR = 1U << 10)
#define LCD_DC_SET (LCD_DC_GPIO_Port->BSRR = 1U << 2)
#define LCD_RST_SET (LCD_RST_GPIO_Port->BSRR = 1U << 1)

#define LCD_CS_RST (LCD_CS_GPIO_Port->BSRR = 1U << (16 + 0))
#define LCD_CLK_RST (LCD_CLK_GPIO_Port->BSRR = 1U << (16 + 10))
#define LCD_DC_RST (LCD_DC_GPIO_Port->BSRR = 1U << (16 + 2))
#define LCD_RST_RST (LCD_RST_GPIO_Port->BSRR = 1U << (16 + 1))

#define USART_BUFFER_SIZE 1

typedef struct{
    char origin[100];
    char protocol[20];
    char command[100];
    char args[4][20];
    char cmd_all[1024];
    char cmd_all_tmp[1024];
    bool status_busy;
}CMD;

typedef struct{
	uint8_t SERIAL_NUMBER [N_DS18B20_SERIAL_NUMBER_BYTES];
	float temperature;
	float delta;
	int pos;
}DS18B20;

typedef struct{
	bool mode_8;
	bool mode_16;
	uint32_t width;
	uint32_t height;
	uint32_t start_x;
	uint32_t end_x;
	uint32_t start_y;
	uint32_t end_y;
	uint8_t command;
	uint8_t *buffer;
	uint8_t setxcmd;
	uint8_t setycmd;
	uint8_t wramcmd;
	uint16_t data;
	uint16_t pixel_color;
	uint16_t background_color;
}Screen;

typedef struct{
	float hotWaterTemp;
	float coldWaterTemp;
	float terminalWaterTemp;
}Water;

typedef struct{
	uint8_t X_;
	uint8_t Y_;
}Pressed;

typedef struct{
	int value;
	int data;
	int temp_set;
	char c[30];
	bool valid;
	bool rst;
	bool set;
	uint8_t n;
}Button;

typedef struct{
	uint8_t err[100];
	uint8_t err_value;
}ERR;

typedef struct{
	ERR err[15];
}ERR_STRING;


typedef struct{
	int steps;
	int current_steps;
	int save_step;
	uint32_t aprox;
	float degPerStep;
}Valve;

typedef struct {
	DS18B20 *hot_temp;
	DS18B20 *terminal_temp;
	Button *button;
	uint32_t flash_position;
	Valve *valve;
}Page;

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim10;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DS18B20 ds18b20_terminal;
extern DS18B20 ds18b20_hot;
extern bool temperature_control_enable;
extern float temperature_set;
extern uint8_t command;
extern float cold_water_temperature;
extern float hot_water_temperature;
extern float terminal_water_temperature;
extern uint8_t flag;
extern CMD cmd;
extern uint8_t act;
extern Button button;
extern Valve valve;
#endif
