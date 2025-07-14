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

void SerialPrint(char * string){
	if( SerialBufferHandle == NULL){
		HAL_UART_Transmit(&huart2, (uint8_t *) "HANDLE IS NULL\n", 50, 100);
	}
	else{
		char *ptr = string;
		osMessageQueuePut(SerialBufferHandle, &ptr, 5, 0);
	}
}

void SerialPrintHex(uint32_t n){
	if( SerialBufferHandle == NULL){
		HAL_UART_Transmit(&huart2, (uint8_t *) "HANDLE IS NULL\n", 50, 100);
	}
	else{
		char num[20];
		sprintf(num, "%X\n", (int)n);
		char *ptr = num;
		osMessageQueuePut(SerialBufferHandle, &ptr, 5, 0);
	}
}

void print_Thread_State(osThreadState_t state){
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
	SerialPrint(string_status);
}


void print_OS_Status(osStatus_t status){
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

	SerialPrint(string_status);
}

void print_CMD(CMD *cmd){
	char s[100]={0};

	sprintf(s, "Origin: %s\nCommand: %s\nProtocol: %s\nArg_1: %s\nArg_2: %s\nArg_3: %s\nArg_4: %s\n",
			cmd->origin,
			cmd->command,
			cmd->protocol,
			cmd->args[0],
			cmd->args[1],
			cmd->args[2],
			cmd->args[3]);

	SerialPrint(s);
}





































