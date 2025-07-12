/*
 * serial_printing.c
 *
 *  Created on: Jan 13, 2025
 *      Author: lucas
 */
#include "serial_printing.h"


void print_hex_f_array(UART_HandleTypeDef* uart, uint8_t* pToArray, uint32_t Timeout
		, bool s_space, bool s_line, bool e_space, bool e_line, bool hex_format){

		int len = strlen((char *)pToArray);
		char hexStr[3];
		for (int i=0; i< len; i++){
			if (hex_format){
				sprintf(hexStr, "%02X", pToArray[i]);
				HAL_UART_Transmit(uart, (uint8_t *)hexStr,2,Timeout);}
			else{HAL_UART_Transmit(uart, &pToArray[i],1,Timeout);}

			if (s_space){
				uint8_t space_char = ' ';
				HAL_UART_Transmit(uart, &space_char, 1, Timeout);}

			else if (s_line){
				uint8_t space_char = '\n';
				HAL_UART_Transmit(uart, &space_char, 1, Timeout);}
		}
		if (e_space){
			uint8_t eol_char = ' ';
			HAL_UART_Transmit(uart, &eol_char, 1, Timeout);}

		else if (e_line){
			const char* eol_char = "\r\n";
			HAL_UART_Transmit(uart, (uint8_t *)eol_char, 1, Timeout);

		}
}

void print_Thread_State(UART_HandleTypeDef* uart, osThreadState_t state){
	uint32_t Timeout = 1000;
	char *string_status;

	switch (state){
	case osThreadInactive:   string_status = "Thread -> Inactive\n";                				break;
	case osThreadReady:      string_status = "Thread -> Ready\n";                					break;
	case osThreadRunning:    string_status = "Thread -> Running\n";                					break;
	case osThreadBlocked:    string_status = "Thread -> Blocked\n";                					break;
	case osThreadTerminated: string_status = "Thread -> Terminated\n";                				break;
	case osThreadError:      string_status = "Thread -> Error\n";                					break;
	case osThreadReserved:   string_status = "Thread -> Reserved\n";                				break;
	default:                 string_status = "Thread -> Failed to find a state\n";                	break;
	}
	HAL_UART_Transmit(uart, (uint8_t *)string_status, strlen(string_status), Timeout);
}


void print_OS_Status(UART_HandleTypeDef* uart, osStatus_t status){
	uint32_t Timeout = 1000;
	char *string_status;

	switch (status){
		case osOK:             string_status = "Status -> OK\n";                					break;
		case osError:          string_status = "Status -> Error\n";    							    break;
		case osErrorTimeout:   string_status = "Status -> Error_TimeOut\n"; 						break;
		case osErrorResource:  string_status = "Status -> Error_Resource\n";						break;
		case osErrorParameter: string_status = "Status -> Error_Parameter\n";						break;
		case osErrorNoMemory:  string_status = "Status -> Error_NoMemory\n";						break;
		case osErrorISR:       string_status = "Status -> Error_ISR\n";								break;
		case osStatusReserved: string_status = "Status -> RESERVED\n";								break;
		default:               string_status = "Status -> Error_Unknown\n";                    		break;
	}

	HAL_UART_Transmit(uart, (uint8_t *)string_status, strlen(string_status), Timeout);
}




































