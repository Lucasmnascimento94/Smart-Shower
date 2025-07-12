/*
 * serial_printing.h
 *
 *  Created on: Jan 13, 2025
 *      Author: lucas
 */

#ifndef SRC_LIB_SERIAL_PRINTING_SERIAL_PRINTING_H_
#define SRC_LIB_SERIAL_PRINTING_SERIAL_PRINTING_H_


#include "types.h"

void print_hex_f_array(UART_HandleTypeDef* uart, uint8_t* msg, uint32_t Timeout
		, bool s_space, bool s_line, bool e_space, bool e_line, bool hex_format);

void print_OS_Status(UART_HandleTypeDef* uart, osStatus_t status);
void print_Thread_State(UART_HandleTypeDef* uart, osThreadState_t state);
#endif /* SRC_LIB_SERIAL_PRINTING_SERIAL_PRINTING_H_ */


/*

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







 * */
