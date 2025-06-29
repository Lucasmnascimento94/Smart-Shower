#ifndef ESP_H
#define ESP_H

#include "types.h"
#include "cmsis_os.h"
#include "dma.h"

void ESP_HAND_SHAKE_HANDLE(ESP_DATA_CONTROL *esp);
void ESP_START_PROTOCOL(ESP_DATA_CONTROL *esp);
void ESP_UART_ACK(ESP_DATA_CONTROL *esp);
void FLAG_BINARY_PRINT(UART_HandleTypeDef *huart, uint32_t flag);

#endif
