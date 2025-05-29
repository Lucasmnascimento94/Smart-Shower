#include "touch.h"
#include "GUI.h"
#include "types.h"
#include "math.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

extern ADC_AnalogWDGConfTypeDef AnalogWDGConfig;
extern ADC_ChannelConfTypeDef sConfig;
extern ADC_HandleTypeDef hadc1;
extern int ADC_VALUE;
extern GPIO_InitTypeDef GPIO_InitStruct;
#define T_CS_RESET GPIOB->BSRR = (1U << (15+16))
#define T_CS_SET GPIOB->BSRR = (1U << (15))
extern uint32_t color_;
extern uint8_t flag;
int x_ = 2500, y_ = 2500;
int i = 0;
int map[14][3] = {
		{PADRUN_X, PADRUN_Y, 0XA0},
		{PADSTOP_X, PADSTOP_Y, 0XB0},
		{PAD1_X, PAD1_Y, 0X84},
		{PAD2_X, PAD2_Y, 0X82},
		{PAD3_X, PAD3_Y, 0X81},
		{PAD4_X, PAD4_Y, 0X44},
		{PAD5_X, PAD5_Y, 0X42},
		{PAD6_X, PAD6_Y, 0X41},
		{PAD7_X, PAD7_Y, 0X24},
		{PAD8_X, PAD8_Y, 0X22},
		{PAD9_X, PAD9_Y, 0X21},
		{PADCLR_X, PADCLR_Y, 0X14},
		{PAD0_X, PAD0_Y, 0X12},
		{PADSET_X, PADSET_Y, 0X11}
};

void scan_g(uint32_t frameBuffer) {
    int x_ = 2500, y_ = 500, i=0;
    char msg[40];
    //draw_terminal(frameBuffer);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    //int k = 0;
    while (x_ > 2100) {
    	// === READ X POSITION ===
    	// Y+ (PC2) → VCC
    	GPIO_InitStruct.Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

    	// Y− (PC0) → GND
    	GPIO_InitStruct.Pin = GPIO_PIN_0;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    	// X+ and X− floating (PC1 + PB1)
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    	HAL_Delay(5);

    	// ADC CHANNEL 9 - PB1 (X read)
    	sConfig.Channel = ADC_CHANNEL_9;
    	sConfig.Rank = 1;
    	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();

    	HAL_ADC_Start(&hadc1);
    	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    	    y_ = HAL_ADC_GetValue(&hadc1);
    	else {
    	    printf_(frameBuffer, "Y ADC Timeout");
    	    continue;
    	}
    	HAL_ADC_Stop(&hadc1);

    	// === READ Y POSITION ===
    	// X+ (PB1) → VCC
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    	// X− (PC1) → GND
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

    	// Y+ and Y− floating (PC2, PC0)
    	GPIO_InitStruct.Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    	GPIO_InitStruct.Pin = GPIO_PIN_0;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    	HAL_Delay(5);

    	// ADC CHANNEL 12 - PC2 (Y read)
    	sConfig.Channel = ADC_CHANNEL_12;
    	sConfig.Rank = 1;
    	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();

    	HAL_ADC_Start(&hadc1);
    	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    	    x_ = HAL_ADC_GetValue(&hadc1);
    	else {
    	    printf_(frameBuffer, "X ADC Timeout");
    	    continue;
    	}
    	HAL_ADC_Stop(&hadc1);

    	// Print values
    	//int sx_ = 400 - ((float)x_/4500)*200;
    	int sy_ = 400 - ((float)y_/4500)*200;
    	sprintf(msg, "VALUE -> %d | %d", x_, y_);
    	printf_(frameBuffer, msg);
    	printf_g(frameBuffer, i++, sy_);


    }
    // Restore PB1 to analog input mode
   // Set_ADC_Channel_GPIO(ADC_CHANNEL_9);
}

void Set_ADC_Channel_GPIO(uint32_t channel) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    switch (channel) {
        case ADC_CHANNEL_9:  // PB1
            __HAL_RCC_GPIOB_CLK_ENABLE();
            GPIO_InitStruct.Pin = GPIO_PIN_1;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
            break;

        case ADC_CHANNEL_10:  // PC0
            __HAL_RCC_GPIOC_CLK_ENABLE();
            GPIO_InitStruct.Pin = GPIO_PIN_0;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            break;

        case ADC_CHANNEL_12:  // PC2
            __HAL_RCC_GPIOC_CLK_ENABLE();
            GPIO_InitStruct.Pin = GPIO_PIN_2;
            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
            break;

        default:
            // Optional: log or handle unsupported channel
            break;
    }
}

void scan_(uint32_t frameBuffer) {
    int x_reading[SAMPLE_SIZE] = {0};
    int y_reading[SAMPLE_SIZE] = {0};
    int result[2] = {0};
    int x_ = 2500, y_ = 500, i=0;
    char msg8[100], msg[100];

	sprintf(msg8,"ENTERED SCAN_\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    ADC_ChannelConfTypeDef sConfig = {0};

    while (x_ > 2100 && i < SAMPLE_SIZE) {
    	// === READ X POSITION ===
    	// Y+ (PC2) → VCC
    	GPIO_InitStruct.Pin = GPIO_PIN_2;
    	sprintf(msg8,"ENTERED SCAN_ while loop1\n");
    	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);

    	// Y− (PC0) → GND
    	GPIO_InitStruct.Pin = GPIO_PIN_0;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);

    	// X+ and X− floating (PC1 + PB1)
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	osDelay(20);
    	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    	osDelay(20);

    	sprintf(msg8,"ENTERED SCAN_ while loop2\n");
    	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);
    	// ADC CHANNEL 9 - PB1 (X read)
    	sConfig.Channel = ADC_CHANNEL_9;
    	sConfig.Rank = 1;
    	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();

    	HAL_ADC_Start(&hadc1);
    	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK){
    	    y_ = HAL_ADC_GetValue(&hadc1);
    		y_reading[i] = y_;}
    	else {
    	    printf_(frameBuffer, "Y ADC Timeout");
    	    continue;
    	}
    	HAL_ADC_Stop(&hadc1);

    	// === READ Y POSITION ===
    	// X+ (PB1) → VCC
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

    	// X− (PC1) → GND
    	GPIO_InitStruct.Pin = GPIO_PIN_1;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);

    	// Y+ and Y− floating (PC2, PC0)
    	GPIO_InitStruct.Pin = GPIO_PIN_2;
    	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    	GPIO_InitStruct.Pin = GPIO_PIN_0;
    	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    	osDelay(10);

    	// ADC CHANNEL 12 - PC2 (Y read)
    	sConfig.Channel = ADC_CHANNEL_12;
    	sConfig.Rank = 1;
    	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) Error_Handler();

    	HAL_ADC_Start(&hadc1);
    	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK){
    	    x_ = HAL_ADC_GetValue(&hadc1);
    		x_reading[i] = x_;}
    	else {
    	    printf_(frameBuffer, "X ADC Timeout");
    	    continue;
    	}
    	HAL_ADC_Stop(&hadc1);

    	// Print values
    	//sprintf(msg, "X: %d | Y: %d", x_, y_);
    	//printf_(frameBuffer, msg);
    	sprintf(msg8,"X: %d | Y: %d\n", x_, y_);
    	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);

    	i++;
    	sprintf(msg8,"LOOPING #%d\n", i);
        HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);
    }

    // === Compute average (or call a better filtering function) ===
    if(i < (SAMPLE_LOWER_CUT + SAMPLE_UPPER_CUT + 1)) return;
    get_value(x_reading, y_reading, result, i);
    if (result[0] == 0 || result[1] == 0){
    	sprintf(msg8,"SCAN_ RETURNING ALREADY\n");
    	osMutexAcquire(adcMutexHandle, osWaitForever);
        HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);
        osMutexRelease(adcMutexHandle);
    	return;
    }

    sprintf(msg, "RESULT -> X2: %d | Y2: %d", result[0], result[1]);
    HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 100);

    uint16_t x = (uint16_t) result[0];
    uint16_t y = (uint16_t) result[1];

    osMutexAcquire(adcMutexHandle, osWaitForever);
    read_pad(frameBuffer, &pressed, x, y); // ✅
    get_button(&button, &pressed); // ✅

// 1 - 2 - SET - STOP
	if(button.value == 1){ // Decrease
		  sprintf(msg8,"DECREASE BUTTON\n");
		  HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
		ADC_THREAD_CONTROL_BIT |= SET_DECREASE;
		button.data -= 1;
	}


	else if(button.value == 2){ // Increase
		  sprintf(msg8,"INCREASE BUTTON\n");
		  HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
		button.data += 1;
		ADC_THREAD_CONTROL_BIT |= SET_INCREASE;
	}
	else if(button.value == -4){ // Run or Set
		  sprintf(msg8,"RUN BUTTON\n");
		  HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
		ADC_THREAD_CONTROL_BIT |= START;
	}
	else if(button.value == -5){ // Stop
		  sprintf(msg8,"STOP BUTTON\n");
		  HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
		ADC_THREAD_CONTROL_BIT |= IDLE;
	}

	ADC_THREAD_CONTROL_BIT |= TOUCH;
	sprintf(msg8,"ACQUIRING adcControllerMutexHandle\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
	osMutexRelease(adcMutexHandle);
	osMutexAcquire(adcControllerMutexHandle, osWaitForever);
	MAIN_THREAD_ADC_CONTROL_BIT |= ADC_THREAD_CONTROL_BIT; // -> Carry bit flags to the main thread
	osMutexRelease(adcControllerMutexHandle);

	osMutexAcquire(adcMutexHandle, osWaitForever);
	ADC_THREAD_CONTROL_BIT = 0x00; // Reset bits

	sprintf(msg8,"LEFT SCAN WITH SUCCESS\n");
	HAL_UART_Transmit(&huart8, (uint8_t *)msg8, strlen(msg8), 1);
	osMutexRelease(adcMutexHandle);
}

void get_value(int *x, int *y, int *result, int size){
	sort_(x, y, size);
	result[0] = 0; result[1] = 0;
	int k = 0;
	for(k=SAMPLE_LOWER_CUT; k < size-SAMPLE_UPPER_CUT; k++){
		result[0] += x[k];
		result[1] += y[k];
	}

	result[0] /= (k - SAMPLE_LOWER_CUT);
	result[1] /= (k - SAMPLE_LOWER_CUT);
}

void sort_(int *x, int *y, int size){
	for(int i=0; i<size - 1; i++){
		for(int j = size-1; j >i; j--){
			if(x[j] < x[i]){
				exchange(x, i, j);
			}
			if(y[j] < y[i]){
				exchange(y, i, j);
			}
		}
	}
}

void exchange(int *x, int a, int b){
	int temp = x[a];
	x[a] = x[b];
	x[b] = temp;
}

void read_pad(uint32_t frameBuffer, Pressed *pressed, int x, int y){
	int col[14][3] = {0};
	int result[3] = {0,0,0};
	int k = 0;
	pressed->X_ = 0; pressed->Y_ = 0;
	for(int i=0; i< 14; i++){

		if(abs(x - map[i][0]) < ERROR_X){
			col[k][0] = map[i][0];
			col[k][1] = map[i][1];
			col[k++][2] = map[i][2];
		}
	}

	for(int i=0; i < k; i++){
		char msg[50] = {0};
		memset(msg, 0, strlen(msg));
		sprintf(msg, "->%X| %d |%d", col[i][2], col[i][0],col[i][1]);
		//printf_(frameBuffer, msg);
		if(abs(y - result[1]) > abs(y - col[i][1]) && (abs(y - col[i][1]) < ERROR_Y)){
			result[0] = col[i][0];
			result[1] = col[i][1];
			result[2] = col[i][2];
		}
	}

	pressed->X_ = (uint8_t)result[2] & 0x0F;
	pressed->Y_ = (uint8_t)result[2] & 0xF0;
	char msg[50] = {0};
	memset(msg, 0, strlen(msg));
	sprintf(msg, "%X| %X |%X", result[2], pressed->X_, pressed->Y_);
	//printf_(frameBuffer, msg);
}


void get_button(Button *button, Pressed *pressed){
	memset(button->c, 0 , strlen(button->c));
	uint8_t r = (pressed->X_ | pressed->Y_);
	/*		(4) (2) (1)
	 * (8)   1    2   3
	 * (4)   4    5   6
	 * (2)   7    8   9
	 * (1)  clr   0   set
	 * */
	switch (r){
		case 0x84:
			button->value = 1; // <
			break;
		case 0x82:
			button->value = 2; // >
			break;
		case 0x81:
			button->value = 3; // Set
			break;
		case 0x44:
			button->value = 4; // Stop
			break;
		case 0x42:
			button->value = 5;
			break;
		case 0x41:
			button->value = 6;
			break;
		case 0x24:
			button->value = 7;
			break;
		case 0x22:
			button->value = 8;
			break;
		case 0x21:
			button->value = 9;
			break;
		case 0xA0:
			button->value = -4;
			break;
		case 0xB0:
			button->value = -5;
			break;
		case 0x14:
			button->value = -2;
			break;
		case 0x12:
			button->value = 0;
			break;
		case 0x11:
			button->value = -3;
			break;
		default:
			button->value = -1;
			break;
	}
}



void scan(SPI_HandleTypeDef *spi, uint32_t frameBuffer, Button *button, Pressed *pressed,bool print){
	uint8_t a;
	uint8_t data_X[2];
	uint8_t data_Y[2];

	int x_reading[SAMPLE_SIZE] = {0};
	int y_reading[SAMPLE_SIZE] = {0};
	int result[2] = {0};

	for(int i=0; i< SAMPLE_SIZE; i++){
	/* X-POSITION*/
		a = 0x01;
		a = START_BIT | X_POSITION;
		//a = START_BIT | X_POSITION | MODE_8BIT;
		memset(data_X, 0, sizeof(data_X));
		memset(data_Y, 0, sizeof(data_Y));

		T_CS_RESET; // @suppress("Statement has no effect")
		HAL_SPI_Transmit(&hspi2, &a, 1, HAL_MAX_DELAY); // @suppress("Statement has no effect")
		HAL_SPI_Receive(&hspi2, data_X, 2, HAL_MAX_DELAY);
		T_CS_SET;

		osDelay(1);
		/* Y-POSITION*/
		a = 0x01;
		a = START_BIT | Y_POSITION;
		T_CS_RESET;
		HAL_SPI_Transmit(&hspi2, &a, 1, HAL_MAX_DELAY);

		HAL_SPI_Receive(&hspi2, data_Y, 2, HAL_MAX_DELAY);
		T_CS_SET;

		uint16_t r_X = data_X[0] << 4 | (data_X[1] >> 3);
		uint16_t r_Y = data_Y[0] << 4 | (data_Y[1] >> 3);

		x_reading[i] = (int)r_X;
		y_reading[i] = (int)r_Y;
		char msg5[40];
		memset(msg5, 0, sizeof(msg5));
		sprintf(msg5, "X> %d | Y>%d", r_X, r_Y);
		printf_(frameBuffer, msg5);
	}

	get_value(x_reading, y_reading, result, SAMPLE_SIZE);
	if(result[0] == 0 || result[1] == 0) return;

	char msg[40];
	sprintf(msg, "X: %d | Y: %d", result[0], result[1]);
	printf_(frameBuffer, msg);

	uint16_t x = (uint16_t) result[0];
	uint16_t y = (uint16_t) result[1];

	read_pad(frameBuffer, pressed, x, y);

	get_button(button, pressed);
	int n = button->value;
	if(n != -1){
		if(n > -1){
			button->rst = false;
			memset(button->c, 0, sizeof(button->c));
			button->temp_set = button->data;
			button->data = (!button->rst)?button->data*10 + button->value: 0;
			sprintf(button->c, "%d", button->data);
			LCD_UpdateTypedTemp(frameBuffer, button->c);
			button->valid = true;
		}

		else{
			button->valid = true;
			button->rst = true;
			if(n == -3){button->set = true;}
		}
	}

	if(print && n != -1){
		char msg1[100];
		if(button->value == -2){
			button->data = 0;
			memset(msg1, 0, sizeof(msg1));
			sprintf(msg1, " ");
			LCD_UpdateTypedTemp(frameBuffer, msg1);
		}

		else if(button->value == -3){
			memset(cmd.args, 0, sizeof(cmd.args));
			sprintf(cmd.args[0], "%d", button->data);

			memset(msg1, 0, sizeof(msg1));
			sprintf(msg1, "Set-> %s", cmd.args[0]);
			LCD_UpdateTypedTemp(frameBuffer, msg1);
			button->data = 0;
		}
		else if(button->value == -4){
			memset(cmd.command, 0, sizeof(cmd.command));
			sprintf(cmd.command, "RUN");

			sprintf(cmd.cmd_all, "Origin: STM32F469IIT6 | Protocol: UART | Command: %s | Arg_0: %s", cmd.command, cmd.args[0]);
			HAL_UART_Transmit(&huart5, (uint8_t *)msg1, 11, 1000);
		}
		else if(button->value == -5){
			memset(cmd.command, 0, sizeof(cmd.command));
			sprintf(cmd.command, "STOP");
		}
	}
	HAL_Delay(500);
}

