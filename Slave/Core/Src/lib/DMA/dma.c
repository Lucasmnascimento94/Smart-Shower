/*
 * dma.c
 *
 *  Created on: Jun 19, 2025
 *      Author: lucas
 */
#include "dma.h"

/*#######################################################################################################
# 	 	 	 	 	 	 	 	 	 	   DMA MEMORYH MAP                                              #
#########################################################################################################
*/

// USART1 - DMA2 - RX->STREAM 5-channel4 - TX->STREAM-channel4
// USART2 - DMA1 - TX->STREAM 6

/*#######################################################################################################
# 	 	 	 	 	 	 	 	 	 	 USART MEMORYH MAP                                              #
#########################################################################################################
*/
#define USART1_ADD (uint32_t)0x40011000 // BASE ADDRESS
#define USART2_ADD (uint32_t)0x40004400 // BASE ADDRESS

#define USART1_SR(uint32_t)(USART1_ADD + 0x00000000)
#define USART1_DR(uint32_t)()

// UART

HAL_StatusTypeDef RESET_DMAR(ESP_DATA_CONTROL *esp){
	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	 	    DMA CONFIGURATION                                           #
	#########################################################################################################
	*/

	  // DMA stream disable if running
	CLEAR_BIT(esp->huart->hdmarx->Instance->CR, DMA_SxCR_EN);
	uint32_t TIME_OUT = 10;

	// Wait until Stream is ready to be configured.
	while(READ_BIT(esp->huart->hdmarx->Instance->CR, DMA_SxCR_EN)){
		if(TIME_OUT-- == 0){
			return HAL_TIMEOUT;
		}

		osDelay(1);
	}

	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	 	    CLEAR FLAGS                                                 #
	#########################################################################################################
	*/
	uint32_t DMA_CLEAR_MASK = 0X00 | DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5 | DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;
	MODIFY_REG(DMA2->HIFCR, 0,DMA_CLEAR_MASK);

	DMA_CLEAR_MASK = 0X00;
	MODIFY_REG(DMA2->LIFCR, 0, DMA_CLEAR_MASK);


	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	   SET DMA CONTROL REGISTER                                         #
	#########################################################################################################
	*/
	uint32_t DMA_CR = 0x00;
	DMA_CR |= 0x02 << 16; // Priority Level (high)
	DMA_CR |= 0x01 << 10; // Memory increment mode
	DMA_CR |= 0x00 << 6; // Data transfer direction (Peripheral-To-Memory)
	DMA_CR |= 0x01 << 4; // Transfer complete interrupt (Enabled)
	DMA_CR |= (uint32_t)4 << 25; // Channel 4
	DMA_CR &= ~(0x01);
	WRITE_REG(esp->huart->hdmarx->Instance->CR, DMA_CR);







	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	 	    UART CONFIGURATION                                          #
	#########################################################################################################
	*/
	SET_BIT(esp->huart->Instance->CR3, USART_CR3_DMAR); // Enable DMA Transmitter
	SET_BIT(esp->huart->Instance->CR1, USART_CR1_RE);
	CLEAR_BIT(esp->huart->Instance->CR3, USART_CR3_HDSEL);
	CLEAR_BIT(esp->huart->Instance->CR1, USART_CR1_IDLEIE);


	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	 	    DMA CONFIGURATION                                           #
	#########################################################################################################
	*/

	uint32_t DMA_CR = 0x00;
	DMA_CR |= 0x02 << 16; // Priority Level (high)
	DMA_CR |= 0x01 << 10; // Memory increment mode
	DMA_CR |= 0x00 << 6; // Data transfer direction (Peripheral-To-Memory)
	DMA_CR |= 0x01 << 4; // Transfer complete interrupt (Enabled)
	DMA_CR |= (uint32_t)5 << 25; // Channel 5
	DMA_CR &= ~(0x01);

	WRITE_REG(esp->huart->hdmarx->Instance->PAR, (uint32_t)&(esp->huart->Instance->DR)); // Source Address
	WRITE_REG(esp->huart->hdmarx->Instance->M0AR, (uint32_t)esp->esp_uart_buffer_rx); // Destination Address
	WRITE_REG(esp->huart->hdmarx->Instance->NDTR, sizeof(esp->esp_uart_buffer_rx));
	WRITE_REG(esp->huart->hdmarx->Instance->CR, DMA_CR);

   // Ensure DMA off
	  __HAL_DMA_CLEAR_FLAG(esp->huart->hdmarx, __HAL_DMA_GET_TC_FLAG_INDEX(esp->huart->hdmarx));  // Clear flags
	  __HAL_DMA_ENABLE(esp->huart->hdmarx);               // Re-enable DMA
	  __HAL_UART_ENABLE_IT(esp->huart, UART_IT_IDLE);   // Re-enable idle IRQ
	  esp->huart->RxState = HAL_UART_STATE_READY;
	  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "RESET DMAR RAN1\n");
	  SEND_DMA(esp);
	  osDelay(100);

	  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "RESET DMAR RAN2\n");
	  SEND_DMA(esp);
	  osDelay(10);
// 	  HAL_UART_DMAStop(esp->huart);                     // Stop DMA cleanly
//	  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "RESET DMAR RAN01\n");
	  //  SEND_DMA(esp);
	 return HAL_UARTEx_ReceiveToIdle_DMA(esp->huart, esp->esp_uart_buffer_rx, sizeof(esp->esp_uart_buffer_rx));
}


void UART_DMA_MFLAG(uint32_t flag, ESP_DATA_CONTROL *esp){
	  if(flag == HAL_BUSY){
		  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "UART FUNCTION THREAD - DMA BUSY\n");
		  SEND_DMA(esp);
	  }
	  if(flag == HAL_TIMEOUT){
		  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "UART FUNCTION THREAD - DMA TIMEOUT\n");
		  SEND_DMA(esp);
	  }
	  if(flag == HAL_ERROR){
		  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "UART FUNCTION THREAD - DMA ERROR\n");
		  SEND_DMA(esp);
	  }

	  if(flag == HAL_OK){
		  snprintf((char *)esp->esp_buffer, sizeof(esp->esp_buffer), "UART FUNCTION THREAD - DMA OK\n");
		  SEND_DMA(esp);
	  }
	  osDelay(10);
}


HAL_StatusTypeDef SET_UP_DMAR(ESP_DATA_CONTROL *esp){



	/*#######################################################################################################
	# 	 	 	 	 	 	 	 	 	   SET DMA MEMORY LOCATIONS                                         #
	#########################################################################################################
	*/
	WRITE_REG(esp->huart->hdmarx->Instance->PAR, (uint32_t)&(esp->huart->Instance->DR)); // Set peripheral address (USARTx->RDR)
	WRITE_REG(esp->huart->hdmarx->Instance->M0AR, (uint32_t)esp->esp_uart_buffer_rx); // Set memory address
	WRITE_REG(esp->huart->hdmarx->Instance->NDTR, sizeof(esp->esp_uart_buffer_rx));

	/*#######################################################################################################
	# 	 	 	 	 	 	 	                UART CR1 CONFIG                                             #
	#########################################################################################################
	*/
	uint32_t CR1 = 0x00;
	CR1 |= ( USART_CR1_UE | USART_CR1_IDLEIE | USART_CR1_RE);
	MODIFY_REG(esp->huart->Instance->CR1, 0, CR1);

	/*#######################################################################################################
	# 	 	 	 	 	 	 	                UART CR2 CONFIG                                             #
	#########################################################################################################
	*/
	uint32_t CR2 = 0x00;
	CR2 |= ( USART_CR2_CLKEN);
	MODIFY_REG(esp->huart->Instance->CR2, 0, CR2);

	/*#######################################################################################################
	# 	 	 	 	 	 	 	                UART CR3 CONFIG                                             #
	#########################################################################################################
	*/
	uint32_t CR3 = 0x00;
	CR3 |= ( USART_CR3_DMAR );
	MODIFY_REG(esp->huart->Instance->CR3, 0, CR3);

	//HAL_UART_Transmit(&huart2, (uint8_t *)"SET UP DMAR\n",20, 100);
	SET_BIT(esp->huart->hdmarx->Instance->CR, DMA_SxCR_EN);
	osDelay(10);
	return HAL_OK;
}




HAL_StatusTypeDef SET_UP_DMAT(ESP_DATA_CONTROL *esp){
	// hdma_usart1_tx
	//_HAL_DMA_DISABLE(huart1.hdmatx);
	uint32_t DMA_CR = 0x00;
	SET_BIT(esp->huart->Instance->CR3, USART_CR3_DMAT); // Enable DMA Transmitter
	SET_BIT(esp->huart->Instance->CR1, USART_CR1_TE);

	DMA_CR |= 0x02 << 16; // Priority Level (high)
	DMA_CR |= 0x01 << 10; // Memory increment mode
	DMA_CR |= 0x01 << 6; // Data transfer direction (Memory-to-Peripheral)
	DMA_CR |= 0x01 << 4; // Transfer complete interrupt (Enabled)
	DMA_CR |= (uint32_t)7 << 25;

	//WRITE_REG(esp->huart->hdmatx->Instance->NDTR, strlen((char*)esp->esp_uart_buffer_tx));
	WRITE_REG(esp->huart->hdmatx->Instance->PAR, (uint32_t)&(esp->huart->Instance->DR)); // Destination Address
	WRITE_REG(esp->huart->hdmatx->Instance->M0AR, (uint32_t)esp->esp_uart_buffer_tx); // Source Base Address
	WRITE_REG(esp->huart->hdmatx->Instance->CR, DMA_CR);

	/*TESTING ANOTHER BUFFER*/

	SET_BIT(esp->huart_test->Instance->CR3, USART_CR3_DMAT); // Enable DMA Transmitter
	SET_BIT(esp->huart_test->Instance->CR1, USART_CR1_TE);

	DMA_CR = 0x00;
	DMA_CR |= 0x02 << 16; // Priority Level (high)
	DMA_CR |= 0x01 << 10; // Memory increment mode
	DMA_CR |= 0x01 << 6; // Data transfer direction (Memory-to-Peripheral)
	DMA_CR |= 0x01 << 4; // Transfer complete interrupt (Enabled)
	DMA_CR |= (uint32_t)6 << 25;
	WRITE_REG(esp->huart_test->hdmatx->Instance->PAR, (uint32_t)&(esp->huart_test->Instance->DR)); // Destination Address
	WRITE_REG(esp->huart_test->hdmatx->Instance->M0AR, (uint32_t)esp->esp_buffer); // Source Base Address
	WRITE_REG(esp->huart_test->hdmatx->Instance->CR, DMA_CR);

	return HAL_OK;
}


HAL_StatusTypeDef SEND_DMA(ESP_DATA_CONTROL *esp) {
    if (esp->huart_test->hdmatx->Instance->CR & DMA_SxCR_EN) {
        return HAL_BUSY;
    }

    uint16_t len = strlen((char*)esp->esp_buffer);
    if (len == 0) return HAL_ERROR;

    // Force disable
    CLEAR_BIT(esp->huart_test->hdmatx->Instance->CR, DMA_SxCR_EN);
    while (esp->huart_test->hdmatx->Instance->CR & DMA_SxCR_EN) {
        __NOP(); // Avoid osDelay in tight DMA timing loop
    }

    // Setup size and buffer
    WRITE_REG(esp->huart_test->hdmatx->Instance->M0AR, (uint32_t)esp->esp_buffer);
    WRITE_REG(esp->huart_test->hdmatx->Instance->NDTR, len);

    // Start DMA stream
    SET_BIT(esp->huart_test->hdmatx->Instance->CR, DMA_SxCR_EN);

    // Optionally: Trigger USART if needed
    // (some STM32 chips require writing DR to start TX)
    //esp->huart->Instance->DR = *(esp->esp_uart_buffer_tx);

    return HAL_OK;
}

HAL_StatusTypeDef SEND_ACK_DMA(ESP_DATA_CONTROL *esp) {
    if (esp->huart->hdmatx->Instance->CR & DMA_SxCR_EN) {
        return HAL_BUSY;
    }

    uint16_t len = strlen((char*)esp->esp_uart_buffer_tx);
    if (len == 0) return HAL_ERROR;

    // Force disable
    CLEAR_BIT(esp->huart->hdmatx->Instance->CR, DMA_SxCR_EN);
    while (esp->huart->hdmatx->Instance->CR & DMA_SxCR_EN) {
        __NOP(); // Avoid osDelay in tight DMA timing loop
    }

    // Setup size and buffer
    WRITE_REG(esp->huart->hdmatx->Instance->M0AR, (uint32_t)esp->esp_uart_buffer_tx);
    WRITE_REG(esp->huart->hdmatx->Instance->NDTR, len);

    // Start DMA stream
    SET_BIT(esp->huart->hdmatx->Instance->CR, DMA_SxCR_EN);

    // Optionally: Trigger USART if needed
    // (some STM32 chips require writing DR to start TX)
    //esp->huart->Instance->DR = *(esp->esp_uart_buffer_tx);

    return HAL_OK;
}
