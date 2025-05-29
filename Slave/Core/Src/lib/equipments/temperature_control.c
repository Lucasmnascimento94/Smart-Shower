/*
 * temperature_control.c
 *
 *  Created on: Jan 16, 2025
 *      Author: lucas
 */

#include "temperature_control.h"

int reach_temperature(Water *water, Valve valve){
	return 0;
}
int get_rough_position(Water *water, Valve valve){
	return (int)water->terminalWaterTemp/valve.degPerStep;
}
void update_temperature(Water *water, Valve valve){

}
uint8_t set_temperature(Water *water, float temperature, Valve valve){
	return 0;
}
void enable_temperature_control(Water *water, TIM_HandleTypeDef* timer){

}
void disable_temperature_control(Water *water, TIM_HandleTypeDef*timer){

}

int VALVE_HANDLE_INIT(uint32_t address, Valve *valve, DS18B20 *hot, FLASH_EraseInitTypeDef * EraseInitStruct, uint8_t *command){
		return 0;
}
void STALL_HANDLE(Valve *valve, DS18B20 *hot, uint8_t *command){
	// Raise or erase STALL flag
	*command = (hot->temperature < 40)? *command | STALL_START : *command & (0xFF ^ STALL_START);

	if((*command & STALL_START) == STALL_START){
		if((*command & STALL_ACTIVE) == 0){
			valve->save_step = valve->steps; // Hold current position
			VALVE_CONTROL_CW_STEP((valve->steps + 50), valve); // Will turn the valve backwards to run hot water only
			*command = *command | STALL_ACTIVE; // Raise flag for active STALL action
		}
	}

	if((*command & STALL_START) == STALL_START && (*command & STALL_ACTIVE) == STALL_ACTIVE){
		if(hot->temperature > 40){
			*command = (*command & (~STALL_ACTIVE)) & (~STALL_ACTIVE);
		}
	}
}

void COMMAND_HANDLE(TIM_HandleTypeDef *htim, TIM_HandleTypeDef *htim2, uint8_t *command){
	if((*command & START) == START){
		LCD_typeDebug("STARTED TIM");
		HAL_TIM_Base_Start_IT(htim); // Enable Clock Interrupt to update the temperature.
		HAL_TIM_Base_Start_IT(htim2);
		// Clear Command flag and set RUNNING flag
		*command = *command & (~START);
	}
	if((*command & IDLE) == IDLE){
		HAL_TIM_Base_Stop_IT(htim);// Disable Clock Interrupt
		HAL_TIM_Base_Stop_IT(htim2);
		// Clear Command flag and set IDLED flag
		*command = *command & (~IDLE);
	}

	if(!(ESP_CTS_GPIO_Port->IDR & ESP_CTS_Pin) && (cmd.status_busy)){
		PARSE_COMMAND();
		cmd.status_busy = false;
		memset(cmd.cmd_all, 0, sizeof(cmd.cmd_all));
	}

	if((act & CONTROLLER_COMMAND) == CONTROLLER_COMMAND){
		act = act & (~CONTROLLER_COMMAND);
		act = act | TEMP_GET_TEMP_SET | TEMP_GET_VALV_APROX;
		char msg10[200];
		sprintf(msg10, "command: %s", cmd.command);
		LCD_typeDebug(msg10);
		HAL_Delay(5000);
		if(strcmp(cmd.command, "RUN") == 0){
			LCD_typeDebug("RUN COMMAND RECEIVE");
			*command |= RUN;
			*command |= START;

			if(strlen(cmd.args[1]) > 0){
				button.temp_set = atoi(cmd.args[1]);
				if(button.temp_set > 0) LCD_UpdateTempSet(button.temp_set);
				int calc = 250*((button.temp_set - COLD_TEMP)/((float)50 - COLD_TEMP));
				valve.aprox = (int)calc;
			}
		}
		else if(strcmp(cmd.command, "STOP") == 0){
			LCD_typeDebug("STOP COMMAND RECEIVE");
			HAL_Delay(4000);
			*command |= IDLE;
		}
	}
}


void VALVE_CONTROL_RUN(Valve *valve, DS18B20 *sensor, Button *button){
	float delta = sensor->temperature - button->temp_set;
	if(abs(delta) > 0.5){
		if(delta > 0){
			VALVE_CONTROL_CW_STEP(1, valve);
		}
		else{
			VALVE_CONTROL_CCW_STEP(1, valve);
		}
	}
}

void PARSE_COMMAND(){
	char *token;
	char *ptr;
	sprintf(cmd.cmd_all_tmp, cmd.cmd_all);
	ptr = cmd.cmd_all_tmp;
	while((token  = strsep(&ptr, " ")) != NULL){
		if(strcmp(token, "Origin:") == 0){memset(cmd.origin, 0, sizeof(cmd.origin));
			if((token = strsep(&ptr, " ")) != NULL){sprintf(cmd.origin, "%s", token); }}
		else if(strcmp(token, "Protocol:") == 0){
			if((token = strsep(&ptr, " ")) != NULL){sprintf(cmd.protocol, "%s", token); }}
		else if (strcmp(token, "Command:") == 0){memset(cmd.command, 0, sizeof(cmd.command));
			if((token = strsep(&ptr, " ")) != NULL){sprintf(cmd.command, "%s", token); act |= CONTROLLER_COMMAND;}}
		else if(strcmp(token, "Arg_0:") == 0){memset(cmd.args[0], 0, sizeof(cmd.args[0]));
			if((token = strsep(&ptr, " ")) != NULL){strcpy(cmd.args[0], token);}}
		else if(strcmp(token, "Arg_1:") == 0){memset(cmd.args[1], 0, sizeof(cmd.args[1]));
			if((token = strsep(&ptr, " ")) != NULL){strcpy(cmd.args[1], token); }}
		else if(strcmp(token, "Arg_2:") == 0){memset(cmd.args[2], 0, sizeof(cmd.args[2]));
			if((token = strsep(&ptr, " ")) != NULL){strcpy(cmd.args[2], token);}}
		else if(strcmp(token, "Arg_3:") == 0){memset(cmd.args[3], 0, sizeof(cmd.args[3]));
			if((token = strsep(&ptr, " ")) != NULL){strcpy(cmd.args[3], token);}}
	}
}







