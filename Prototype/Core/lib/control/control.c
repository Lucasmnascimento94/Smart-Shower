#include "control.h"
char msg1[200];

void CMD_HANDLER(uint32_t frameBuffer){
	char msg8[100];
	sprintf(msg8,"CMD HANDLER FUNCTION CALLED\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);

	if((cmd.cmd_flag & TOUCH) == TOUCH){ // Touch screen
	  	sprintf(msg8,"TOUCH SCREEN FLAGGED\n");
	    HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
		if(cmd.cmd_flag & SET_DECREASE){
			sprintf(msg8,"SET DECREASE FLAGGED\n");
			HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
			update_setBox(frameBuffer, button.data);
			sprintf(cmd.args[0], "%d", button.data);

			cmd.cmd_flag = cmd.cmd_flag &(~TOUCH);
		}
		else if(cmd.status_busy & SET_INCREASE){
			sprintf(msg8,"SET INCREASE FLAGGED\n");
			HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
			update_setBox(frameBuffer, button.data);
			sprintf(cmd.args[0], "%d", button.data);
		}
		else if(cmd.cmd_flag & START){
			sprintf(msg8,"START FLAGGED\n");
			HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
			memset(cmd.command, 0, sizeof(cmd.command));
			sprintf(cmd.command, "RUN");
			sprintf(cmd.cmd_all, "%s dummy %s", cmd.command, cmd.args[0]);
			HAL_UART_Transmit(&huart5, (uint8_t *)cmd.cmd_all, strlen(cmd.cmd_all), 1000);
			//HAL_UART_Transmit(&huart8, (uint8_t *)cmd.cmd_all, strlen(cmd.cmd_all), 1000);
			memset(msg1, 0, sizeof(msg1));
			sprintf(msg1, "Run-> %s", cmd.args[0]);
			LCD_UpdateTypedTemp(frameBuffer, msg1);
			cmd.cmd_flag = cmd.cmd_flag &(~START);
		}
		else if(cmd.cmd_flag & IDLE){
			sprintf(msg8,"IDLE FLAGGED\n");
			HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1000);
			memset(cmd.command, 0, sizeof(cmd.command));
			memset(cmd.cmd_all, 0, sizeof(cmd.cmd_all));
			sprintf(cmd.command, "STOP");
			sprintf(cmd.cmd_all, "%s", cmd.command);
			HAL_UART_Transmit(&huart5, (uint8_t *)cmd.cmd_all, strlen(cmd.cmd_all), 1000);
			HAL_UART_Transmit(&huart8, (uint8_t *)cmd.cmd_all, strlen(cmd.cmd_all), 1000);
			LCD_UpdateTypedTemp(frameBuffer, cmd.command);
			cmd.cmd_flag = cmd.cmd_flag &(~IDLE);
		}
	}
	HANDLE(frameBuffer);
}

void HANDLE(uint32_t frameBuffer){
	switch (cmd.cmd_flag){
		case TEMP_UPDATE:
			printf_(frameBuffer, "TEMP UPDATE");
			update_setTemp(frameBuffer, "SHOULD BE HERE");
			if(strlen(cmd.args[1]) > 0)update_outTemp(frameBuffer, cmd.args[1]);
			if(strlen(cmd.args[2]) > 0)update_hotTemp(frameBuffer, cmd.args[2]);
			cmd.cmd_flag = cmd.cmd_flag &(~TEMP_UPDATE);
			break;
		case RESEND_COMMAND:
			break;
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
			if((token = strsep(&ptr, " ")) != NULL){sprintf(cmd.command, "%s", token);}}
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

void LCD_COMMAND_HANDLER(uint32_t frameBuffer){
	if((LCD_THREAD_CONTROL_BIT & SET_DECREASE) == SET_DECREASE){
		lcdDataHolder.setTemp --;
		update_setBox(frameBuffer, lcdDataHolder.setTemp);
		LCD_THREAD_CONTROL_BIT = LCD_THREAD_CONTROL_BIT & (~ SET_DECREASE);
	}

	if((LCD_THREAD_CONTROL_BIT & SET_INCREASE) == SET_INCREASE){
		lcdDataHolder.setTemp ++;
		update_setBox(frameBuffer, lcdDataHolder.setTemp);
		LCD_THREAD_CONTROL_BIT = LCD_THREAD_CONTROL_BIT & (~ SET_INCREASE);
	}

	if((LCD_THREAD_CONTROL_BIT & UPDATE_CALLING) == UPDATE_CALLING){
		update_setTemp(frameBuffer, lcdDataHolder.callingWaterTemp);
		LCD_THREAD_CONTROL_BIT = LCD_THREAD_CONTROL_BIT & (~ UPDATE_CALLING);
	}

	if((LCD_THREAD_CONTROL_BIT & UPDATE_SHOWER) == UPDATE_SHOWER){
		update_setTemp(frameBuffer, lcdDataHolder.showerWaterTemp);
		LCD_THREAD_CONTROL_BIT = LCD_THREAD_CONTROL_BIT & (~ UPDATE_SHOWER);
	}

	if((LCD_THREAD_CONTROL_BIT & UPDATE_HOT) == UPDATE_HOT){
		update_setTemp(frameBuffer, lcdDataHolder.hotWaterTemp);
		LCD_THREAD_CONTROL_BIT = LCD_THREAD_CONTROL_BIT & (~ UPDATE_HOT);
	}
}




// SET_INCREASE | TOUCH | UPDATE_CALLING | UPDATE_SHOWER| UPDATE_HOT











