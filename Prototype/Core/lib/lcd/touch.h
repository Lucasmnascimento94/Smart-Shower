#ifndef TOUCH_H
#define TOUCH_H

#include "stm32f4xx_hal.h"
#include "types.h"

extern Button button;
void scan_(uint32_t frameBuffer);
void scan(SPI_HandleTypeDef *spi, uint32_t frameBuffer, Button *button, Pressed *pressed,bool print);
void read_col(Pressed *pressed, uint16_t *x);
void read_row(Pressed *pressed, uint16_t *x);
void read_pad(uint32_t frameBuffer, Pressed *pressed, int x, int y);
void get_value(int *x, int *y, int *result, int size);
void sort_(int *x, int *y, int size);
void exchange(int *x, int a, int b);
void get_button(Button *button, Pressed *pressed);
void Set_ADC_Channel_GPIO(uint32_t channel);

#endif
