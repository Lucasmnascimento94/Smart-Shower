#include "valves.h"
#include "math.h"

int VALVE_MODE_STANDBY(void){
	VALVE_M1_RST;
	VALVE_M2_RST;
	VALVE_M3_RST;
	return 0;
}

int VALVE_MODE_FULL_CW(void){
	VALVE_M1_RST;
	VALVE_M2_RST;
	VALVE_M3_SET;
	return 0;
}
int VALVE_MODE_1_2A_CW(void){
	VALVE_M1_RST;
	VALVE_M2_SET;
	VALVE_M3_RST;
	return 0;
}
int VALVE_MODE_1_2B_CW(void){
	VALVE_M1_RST;
	VALVE_M2_SET;
	VALVE_M3_SET;
	return 0;
}
int VALVE_MODE_1_4_CW(void){
	VALVE_M1_SET;
	VALVE_M2_RST;
	VALVE_M3_RST;
	return 0;
}
int VALVE_MODE_1_8_CW(void){
	VALVE_M1_SET;
	VALVE_M2_RST;
	VALVE_M3_SET;
	return 0;
}
int VALVE_MODE_1_16_CW(void){
	VALVE_M1_SET;
	VALVE_M2_SET;
	VALVE_M3_RST;
	return 0;
}

int VALVE_CONTROL_CW_STEP(int step, Valve *valve){ // FEEDS COLD WATER
	HAL_Delay(1);
	VALVE_CCW_SET;
	TIMER_US_DELAY(1);
	VALVE_RST_SET;
	TIMER_US_DELAY(1);
	VALVE_EN_SET;
	TIMER_US_DELAY(1);
	VALVE_TQ_SET;
	TIMER_US_DELAY(1);

	VALVE_CLK_RST;
	for(uint32_t i=0; i<step; i++){//&& (valve->steps < MAX_STEPS)
		VALVE_CLK_SET;
		HAL_Delay(10);
		VALVE_CLK_RST;
		HAL_Delay(10);
		valve->steps--;
	}
	VALVE_TQ_RST;

	FLASH_HANDLE(valve->steps);
	return 0;
}

int VALVE_CONTROL_CCW_STEP(int step, Valve *valve){ // FEEDS HOT WATER
	HAL_Delay(1);
	VALVE_CCW_RST;
	TIMER_US_DELAY(1);
	VALVE_RST_SET;
	TIMER_US_DELAY(1);
	VALVE_EN_SET;
	TIMER_US_DELAY(1);
	VALVE_TQ_SET;
	TIMER_US_DELAY(1);

	VALVE_CLK_RST;
	for(uint32_t i=0; i<step; i++){//&& (valve->steps > 0)
		VALVE_CLK_SET;
		HAL_Delay(10);
		VALVE_CLK_RST;
		HAL_Delay(10);
		valve->steps++;
	}

	FLASH_HANDLE(valve->steps);
	VALVE_TQ_RST;
	return 0;
}

int VALVE_CONTROL_APROX(Valve *valve){
	if(valve->aprox > valve->steps){
		VALVE_CONTROL_CCW_STEP((valve->aprox) - valve->steps, valve);
	}
	else if(valve->aprox < valve->steps){
    	VALVE_CONTROL_CW_STEP((valve->steps - valve->aprox), valve);
	}
	FLASH_HANDLE(valve->steps);
	return 0;
}

int VALVE_CONTROL_HANDLE(Valve *valve, Button *button, DS18B20 *terminal){
	if(terminal->temperature < button->temp_set){
		VALVE_CONTROL_CCW_STEP(1, valve);
	}
	else if(terminal->temperature > button->temp_set){
		VALVE_CONTROL_CW_STEP(1, valve);
	}

	FLASH_HANDLE(valve->steps);
	return 0;
}

void VALVE_CONTROL_HANDLE_INIT(Valve *valve){
	// Reset motor positioning.
	uint32_t *p = (uint32_t *)FLASH_ADDRESS;
	int h = (int)*p;
	while(h !=0){
		if(h > 0){
			VALVE_CONTROL_CW_STEP(1 , valve);
			h--;
		}
		else{
			VALVE_CONTROL_CCW_STEP(1, valve);
			h++;
		}
		LCD_UpdateOpening(h);
	}

	FLASH_HANDLE(valve->steps + h);
	h = (int)*p;
	valve->steps = h;
}


void FLASH_HANDLE(int data){
	static FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS; // Specifies that we are erasing sectors
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3; // Set this according to your voltage range
	EraseInitStruct.Sector = FLASH_SECTOR_7; // Specify the sector number
	EraseInitStruct.NbSectors = 1;
	HAL_FLASHEx_Erase_IT(&EraseInitStruct);
	HAL_FLASH_Program_IT(FLASH_TYPEPROGRAM_WORD, (uint32_t)FLASH_ADDRESS, data);
	HAL_FLASH_Lock();
}












