
#ifndef TYPES_H
#define TYPES_H

#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "math.h"
#include "stdbool.h"
#include "stdint.h"
#include "main.h"
#include "RGB565_COLOR.h"
#include "pages.h"
/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	      ADC BIT FLAGS                                             #
#########################################################################################################
#########################################################################################################
*/
#define SET_DECREASE (uint32_t)0x01
#define SET_INCREASE (uint32_t)0x02
#define START (uint32_t)0x04
#define IDLE (uint32_t)0x08
#define TOUCH (uint32_t)0x10

/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	      UART BIT FLAGS                                            #
#########################################################################################################
#########################################################################################################
*/

#define UART_AVAILABLE (uint32_t)0x01
#define TEMP_UPDATE (uint32_t)0x01
#define RESEND_COMMAND (uint32_t)0x02
#define MOTOR_ALERT (uint32_t)0x04
#define STATUS (uint32_t)0x08



/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	     LCD ENUMARATION                                            #
#########################################################################################################
#########################################################################################################
*/

#define UPDATE_CALLING 0x20
#define UPDATE_SHOWER (uint32_t)0x40
#define UPDATE_HOT (uint32_t)0x80

#define FRAMEBUFFER_WIDTH  800
#define FRAMEBUFFER_HEIGHT 480
#define BYTES_PER_PIXEL 2 // For RGB565
#define FRAMEBUFFER_SIZE (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT)
#define BUFFER_LAYER_1_ADDRESS  ((uint32_t)0xD0000000)
#define BUFFER_LAYER_2_ADDRESS ((uint32_t)(BUFFER_LAYER_1 + FRAMEBUFFER_SIZE*(BYTES_PER_PIXEL)))
#define INNERBUFFER_WIDTH 800
#define INNERBUFFER_HEIGHT 480
#define RGB565(r, g, b) ( ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3) )

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define FLASH_ADDRESS 0x08060000
/* flag */
#define SCAN_PAD 0x80

/* command */
#define STALL_START 0x04
#define STALL_STOP 0x08
#define STALL_ACTIVE 0x10
#define RUN_ 0x40

#define SCREEN_PAGE1 0x01
#define SCREEN_PAGE2 0x02
#define SCREEN_PAGE3 0x04
#define SCREEN_PAGE4 0x08


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


#define START_BIT  0b10000000 // HIGH
#define X_POSITION 0b01010000 // 101
#define Y_POSITION 0b00010000 // 001
#define MODE_12BIT 0b00000000 // LOW
#define MODE_8BIT  0b00001000 // HIGH
#define MODE_DFR   0b00000111 // LOW

#define T_Y_H (120 - 14)
#define T_X_C1 830
#define T_X_C2 940
#define T_X_C3 1020

#define T_Y_R1 970
#define T_Y_R2 920
#define T_Y_R3 860
#define T_Y_R4 815
#define ERROR_X 50
#define ERROR_Y 30


#define color_index 0
#define offsetX 0
#define offsetY 0
#define WIDTH 800
#define HEIGHT 480
#define word_width 8
#define word_height 16

#define SAMPLE_SIZE 15
#define SAMPLE_LOWER_CUT 1
#define SAMPLE_UPPER_CUT 1


#define PADCLR_X 3120
#define PADCLR_Y 2960

#define PAD0_X 3440
#define PAD0_Y 2950

#define PADSET_X 3760
#define PADSET_Y 2950

#define PAD1_X 3460
#define PAD1_Y 3350

#define PAD2_X 3720
#define PAD2_Y 3340

#define PAD3_X 4009
#define PAD3_Y 3360

#define PAD4_X 3150
#define PAD4_Y 3230

#define PAD5_X 3450
#define PAD5_Y 3220

#define PAD6_X 3750
#define PAD6_Y 3230

#define PAD7_X 3130
#define PAD7_Y 3090

#define PAD8_X 3450
#define PAD8_Y 3090

#define PAD9_X 3730
#define PAD9_Y 3100

#define PADRUN_X 3615
#define PADRUN_Y 3230

#define PADSTOP_X 3615
#define PADSTOP_Y 3110

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


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

typedef struct {
	uint32_t width;
	uint32_t w_gap;
	uint32_t height;
	uint32_t h_gap;
	uint16_t color;
	uint16_t word_color;
	uint16_t stop_color;
	uint32_t start_x;
	uint32_t start_y;
}PAD;


typedef struct LINE LINE;

struct LINE{
	char line_msg[21];
	LINE *next;
};

typedef struct {
	uint32_t x_start;
	uint32_t y_start;
	uint32_t width;
	uint32_t height;
	uint16_t color;
	uint16_t word_color;
	uint8_t line_count;
	uint8_t column_count;
	LINE *first_line;
	LINE *last_line;
	uint32_t header_gap;

}TERMINAL;

typedef struct {
	uint32_t set;
	uint32_t hot;
	uint32_t out;
	uint32_t cmd;
	uint32_t steps;
}SCREEN_VALUES;

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

typedef struct{
	uint8_t X_;
	uint8_t Y_;
	uint16_t adc_x;
	uint16_t adc_y;
}Pressed;

typedef struct{
	int p_01[3];
	int p_02[3];
	int p_11[3];
	int p_12[3];
	int p_13[3];
	int p_21[3];
	int p_22[3];
	int p_23[3];
	int p_31[3];
	int p_32[3];
	int p_33[3];
	int p_41[3];
	int p_42[3];
	int p_43[3];
}MAP;

typedef struct {
	char origin[50];
	char protocol[50];
	char command[50];
	char args[4][50];
	char cmd_all[1024];
	char cmd_all_tmp[1024];
	uint32_t cmd_flag;
	bool status_busy;
}CMD;

typedef struct{
	char callingWaterTemp[50];
	char showerWaterTemp[50];
	char hotWaterTemp[50];
	char motor_position[50];
	int  setTemp;

}DATA_HOLDER;


/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	MAIN THREAD GLOBAL VARIABLES                                    #
#########################################################################################################
#########################################################################################################
*/
extern uint8_t command;
extern uint8_t flag;
extern uint32_t MAIN_THREAD_ADC_CONTROL_BIT;
extern uint32_t MAIN_THREAD_UART_CONTROL_BIT;
extern uint32_t MAIN_THREAD_LCD_CONTROL_BIT;

extern osMutexId_t mainMutexHandle;
extern osMutexId_t adcControllerMutexHandle;
extern osMutexId_t lcdControllerMutexHandle;
extern osMutexId_t uartControllerMutexHandle;

extern DATA_HOLDER mainDataHolder;
/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	       ADC THREAD GLOBAL VARIABLES                                      #
#########################################################################################################
#########################################################################################################
*/
extern Button button;
extern Pressed pressed;
extern uint32_t ADC_THREAD_CONTROL_BIT;

extern osMutexId_t adcMutexHandle;
/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	UART THREAD GLOBAL VARIABLES                                    #
#########################################################################################################
#########################################################################################################
*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart8;

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;

extern uint32_t UART_THREAD_CONTROL_BIT;
extern CMD cmd;

extern char uartBuffer[1024];
extern osMutexId_t uartMutexHandle;

/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	 LCD THREAD GLOBAL VARIABLES                                    #
#########################################################################################################
#########################################################################################################
*/
extern TERMINAL terminal;
extern PAD pad;
extern uint32_t LCD_THREAD_CONTROL_BIT;
extern DATA_HOLDER lcdDataHolder;
extern osMutexId_t lcdMutexHandle;
/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	       TYPEDEF STRUCTS                                              #
#########################################################################################################
#########################################################################################################
*/

#endif
