#include "esp.h"
#include "serial_printing.h"


void ESP_HAND_SHAKE_HANDLE(ESP_DATA_CONTROL *esp){
	if((FLAG_UART & ESP_RTS) && (FLAG_UART & ESP_MSG_COMPLETE)){
		//HAL_UART_Transmit(&huart2, (uint8_t *) "RECEIVE CPLT\n", 15, 100);

		//HAL_UART_Transmit(&huart2, esp->esp_uart_buffer_rx, strlen((char *)esp->esp_uart_buffer_rx), 100);
		//HAL_UART_Transmit(&huart2, (uint8_t *) "MSG ABOVE\n", 15, 100);


		esp->checksum = strlen((char*)esp->esp_uart_buffer_rx) - 2;
		snprintf((char *)esp->esp_uart_buffer_tx, sizeof(esp->esp_uart_buffer_tx), "%d\n", esp->checksum);
		FLAG_UART &= ~(ESP_MSG_COMPLETE); // Clear Flag of Data transfered
		FLAG_UART |= ESP_MSG_ACK; // Raise flag for ACK response
		RESET_RTS; // Request to Send CHECKSUM
	}

	if((FLAG_UART & ESP_MSG_ACK) && (FLAG_UART & ESP_MSG_ACK_SENT)){
		//HAL_UART_Transmit(&huart2, (uint8_t *)"ACK RESPONSE SENT\n", 18, 100);
		osDelay(10);
		FLAG_UART = 0x00;
		SET_RTS;
		memset(esp->esp_uart_buffer_tx, 0, sizeof(esp->esp_uart_buffer_tx));
		memset(esp->esp_uart_buffer_rx, 0, sizeof(esp->esp_uart_buffer_rx));
		memset(esp->checksum_s, 0, sizeof(esp->checksum_s));
		esp->checksum = 0;
	}
}

void ESP_START_PROTOCOL(ESP_DATA_CONTROL *esp){
	TickType_t MUTEX_WAIT = 1000;
	esp->rx_status = HAL_TIMEOUT;
	osStatus_t status = osMutexAcquire(HUART1Handle, MUTEX_WAIT);
	if(osMutexAcquire(HUART2Handle, MUTEX_WAIT) == osOK){
		print_OS_Status(&huart2, status);
		osMutexRelease(HUART2Handle);
	}

	if(status == osOK){
		esp->rx_status = HAL_UARTEx_ReceiveToIdle_DMA(esp->huart, esp->esp_uart_buffer_rx, sizeof(esp->esp_uart_buffer_rx));
		  if(esp->rx_status == HAL_OK){
			  FLAG_UART |= ESP_RTS;
			  RESET_CTS;
			  //HAL_UART_Transmit(&huart2, (uint8_t *) "START PROTOCOL\n", 15, 100);
			  return;
		  }
		 else{
			 osMutexRelease(HUART1Handle);
			 //print_OS_Status(&huart2, osMutexRelease(HUART1Handle));
		 }
	}
	//HAL_UART_Transmit(&huart2, (uint8_t *) "FAIL PROTOCOL\n", 15, 100);
}


void ESP_UART_ACK(ESP_DATA_CONTROL *esp){
	TickType_t MUTEX_WAIT = 1000;
	esp->tx_status = HAL_TIMEOUT;
	osStatus_t status = osMutexAcquire(HUART1Handle, MUTEX_WAIT);
	//print_OS_Status(&huart2, status);

	if(!(FLAG_UART & ESP_MSG_ACK_SENT)){
		if(status == osOK){
			esp->tx_status = HAL_UART_Transmit_DMA(esp->huart, esp->esp_uart_buffer_tx, strlen((char *)esp->esp_uart_buffer_tx));

			if(esp->tx_status != HAL_OK){
				//HAL_UART_Transmit(&huart2, (uint8_t *) "FAIL ACK\n", 15, 100);
				osMutexRelease(HUART1Handle);
				//print_OS_Status(&huart2, osMutexRelease(HUART1Handle));
			}
		}
	}
}


void FLAG_BINARY_PRINT(UART_HandleTypeDef *huart, uint32_t flag){
	char binary_s[100];
	for(int i=0; i< 32; i++){
		binary_s[i] = (0x80000000 & (flag << i)) == 0? '0': '1';
	}
	binary_s[32] = '\n';
	binary_s[33] = '\0';

	//HAL_UART_Transmit(huart, (uint8_t *)binary_s, strlen(binary_s), 100);
	osDelay(10);
}






/*

void ESP_HAND_SHAKE_HANDLE(ESP_DATA_CONTROL *esp){
	if((FLAG_UART & ESP_MSG_READY) && (FLAG_UART & ESP_MSG_COMPLETE)){
		char msg[1000];
		uint16_t size = strlen((char*)esp->esp_uart_buffer_rx);

		esp->checksum = strlen((char*)esp->esp_uart_buffer_rx) - 2;
		snprintf((char *)esp->esp_uart_buffer_tx, sizeof(esp->esp_uart_buffer_tx), "%d\n", esp->checksum);
		FLAG_UART &= ~(ESP_MSG_COMPLETE); // Clear Flag of Data transfered
		FLAG_UART |= ESP_MSG_ACK; // Raise flag for ACK response
		RESET_RTS; // Request to Send CHECKSUM
		snprintf(msg, sizeof(msg), "\nSENT: %s | CHECKSUM: %d\n", esp->esp_uart_buffer_tx, esp->checksum);

		//HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen((char *)msg), 100);
		osDelay(10);
		//HAL_UART_Transmit(&huart2, esp->esp_uart_buffer_rx, size, 100);
		osDelay(10);
		//HAL_UART_Transmit(&huart2, (uint8_t *)"Raised FLAG for ACK response\n", 30, 100);
		osDelay(10);

		memset(esp->esp_uart_buffer_rx, 0, size);
	}

	if((FLAG_UART & ESP_MSG_ACK) && (FLAG_UART & ESP_MSG_ACK_SENT)){
		//HAL_UART_Transmit(&huart2, (uint8_t *)"ACK RESPONSE SENT\n", 18, 100);
		osDelay(10);
		FLAG_UART = 0x00;
		SET_RTS;
		memset(esp->esp_uart_buffer_tx, 0, sizeof(esp->esp_uart_buffer_tx));
		memset(esp->esp_uart_buffer_rx, 0, sizeof(esp->esp_uart_buffer_rx));
		memset(esp->checksum_s, 0, sizeof(esp->checksum_s));
		esp->checksum = 0;
	}
}

void ESP_START_PROTOCOL(ESP_DATA_CONTROL *esp){
	esp->rx_status = RESET_DMAR(esp);

	//HAL_UART_Transmit(&huart2, (uint8_t *)"\nSTART PROTOCOL\n",20, 100);
	  if(esp->rx_status == HAL_OK){
		  osDelay(50);
		  FLAG_UART |= ESP_MSG_READY;
		  RESET_CTS;
	  }
}
void ESP_UART_ACK(ESP_DATA_CONTROL *esp){
	//char msg[100];
	if(!(FLAG_UART & ESP_MSG_ACK_SENT)){
		FLAG_UART |= ESP_MSG_ACK_SENT;
		//HAL_UART_Transmit(esp->huart, esp->esp_uart_buffer_tx, strlen((char*)esp->esp_uart_buffer_tx), 100);
		snprintf((char *)esp->esp_uart_buffer_tx, sizeof(esp->esp_uart_buffer_tx), "HELLO WORLD");
		osDelay(10);
		SEND_ACK_DMA(esp);
		osDelay(50);
		//HAL_UART_Transmit(esp->huart, esp->esp_uart_buffer_tx, strlen((char *)esp->esp_uart_buffer_tx), 100);
		//snprintf(msg, sizeof(msg), "CHECKSUM SENT: %d\n", (int)esp->checksum);
		//HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), 100);

	}
}

void FLAG_BINARY_PRINT(UART_HandleTypeDef *huart, uint32_t flag){
	char binary_s[100];
	for(int i=0; i< 32; i++){
		binary_s[i] = (0x80000000 & (flag << i)) == 0? '0': '1';
	}
	binary_s[32] = '\n';
	binary_s[33] = '\0';

	//HAL_UART_Transmit(huart, (uint8_t *)binary_s, strlen(binary_s), 100);
	osDelay(10);
}
*/
