/*
 * temperature_control.h
 *
 *  Created on: Jan 16, 2025
 *      Author: lucas
 */

#ifndef SRC_LIB_EQUIPMENTS_TEMPERATURE_CONTROL_H_
#define SRC_LIB_EQUIPMENTS_TEMPERATURE_CONTROL_H_

#include "valves.h"
#include "types.h"

int reach_temperature(Water *water, Valve valve);
int get_rough_position(Water *water, Valve valve);
void update_temperature(Water *water, Valve valve);
uint8_t set_temperature(Water *water, float temperature, Valve valve);
void enable_temperature_control(Water *water, TIM_HandleTypeDef* timer);
void disable_temperature_control(Water *water, TIM_HandleTypeDef*timer);
int VALVE_HANDLE_INIT(uint32_t address, Valve *valve, DS18B20 *hot, FLASH_EraseInitTypeDef * EraseInitStruct, uint8_t *command);
void STALL_HANDLE(Valve *valve, DS18B20 *hot, uint8_t *command);
void COMMAND_HANDLE(TIM_HandleTypeDef *htim, TIM_HandleTypeDef *htim2, uint8_t *command);
void PARSE_COMMAND();
#endif /* SRC_LIB_EQUIPMENTS_TEMPERATURE_CONTROL_H_ */
