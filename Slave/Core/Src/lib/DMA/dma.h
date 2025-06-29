/*
 * dma.h
 *
 *  Created on: Jun 19, 2025
 *      Author: lucas
 */

#ifndef SRC_LIB_DMA_DMA_H_
#define SRC_LIB_DMA_DMA_H_

#include "types.h"
#include "cmsis_os.h"


extern UART_HandleTypeDef huart2;

// void START_HANDSHAKE(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef SET_UP_DMAR(ESP_DATA_CONTROL *esp);
HAL_StatusTypeDef RESET_DMAR(ESP_DATA_CONTROL *esp);
HAL_StatusTypeDef SEND_DMA(ESP_DATA_CONTROL *esp);
HAL_StatusTypeDef SEND_ACK_DMA(ESP_DATA_CONTROL *esp);
void UART_DMA_MFLAG(uint32_t flag, ESP_DATA_CONTROL *esp);

HAL_StatusTypeDef SET_UP_DMAT(ESP_DATA_CONTROL *esp);

#endif /* SRC_LIB_DMA_DMA_H_ */
