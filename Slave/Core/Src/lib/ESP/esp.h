#ifndef ESP_H
#define ESP_H

#include "types.h"
#include "dma.h"

void ESP_HAND_SHAKE_HANDLE(ESP_DATA_CONTROL *esp);
void ESP_START_PROTOCOL(ESP_DATA_CONTROL *esp);
void ESP_UART_ACK(ESP_DATA_CONTROL *esp);
void FLAG_BINARY_PRINT(UART_HandleTypeDef *huart, uint32_t flag);

#endif


/*
 NOTES:

 ****
 - TaskHandle_t xSemaphoreGetMutexHolder(SemaphoreHandle_t xMutex)
 -> Can be used to check if the calling task is the mutex holder.
 ****


 ****
 - BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore)
 -> Used to release a taken semaphore
 ****


  ****
 - BaseType_t xSemaphoreGiveFromISR( SemaphoreHandle_t xSemaphore, Signed BaseType_t *pxHigherPriorityTaskWoken)
 -> Used to release a taken semaphore
 ****




  ****
 - BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t x TicksToWait)
 -> Used to release a taken semaphore
 ****


  ****
 - BaseType_t xSemaphoreTakeFromISR( SemaphoreHandle_t xSemaphore, Signed BaseType_t *pxHigherPriorityTaskWoken)
 -> Used to release a taken semaphore
 ****
 */
