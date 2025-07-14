#include "screen.h"
#include "serial_printing.h"
#include <math.h>

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi4;

void scan(SPI_HandleTypeDef *spi, Scan *scan_reading){
	uint8_t a;
	uint8_t data_X[2] = {0};
	uint8_t data_Y[2] = {0};


	/* X-POSITION*/
	a = 0x00;
	a = START_BIT | X_POSITION;
	//a = START_BIT | X_POSITION | MODE_8BIT;

	T_CS_RESET; // @suppress("Statement has no effect")
	//T_IRQ_RESET;
	HAL_SPI_Transmit(&hspi4, &a, 1, 1000); // @suppress("Statement has no effect")
	//T_IRQ_SET;
	//T_IRQ_RESET;
	HAL_SPI_Receive(&hspi4, data_X, 2, 1000);
	T_CS_SET;
	//T_IRQ_SET;

	osDelay(10);
	/* Y-POSITION*/
	a = 0x01;
	a = START_BIT | Y_POSITION;
	//a = START_BIT | Y_POSITION | MODE_8BIT;
	T_CS_RESET;
	//T_IRQ_RESET;
	HAL_SPI_Transmit(&hspi4, &a, 1, 1000);
	//T_IRQ_SET;
	//T_IRQ_RESET;
	HAL_SPI_Receive(&hspi4, data_Y, 2, 1000);
	T_CS_SET;
	//T_IRQ_SET;

	uint16_t r_X = data_X[0] << 4 | (data_X[1] >> 3);
	uint16_t r_Y = data_Y[0] << 4 | (data_Y[1] >> 3);
	scan_reading->x = r_X;
	scan_reading->y = r_Y;
}

void read_col(Pressed *pressed, uint16_t *x){
	pressed->X_ = 0x00;
	if(*x > (T_X_C1 - ERROR_X) && *x < (T_X_C1 + ERROR_X)){
		pressed->X_ |= (0x04);
	}
	else if(*x > (T_X_C2 - ERROR_X) && *x < (T_X_C2 + ERROR_X)){
		pressed->X_ |= (0x02);
	}
	else if(*x > (T_X_C3 - ERROR_X) && *x < (T_X_C3 + ERROR_X)){
		pressed->X_ |= (0x01);
	}
}

void read_row(Pressed *pressed, uint16_t *y){
	pressed->Y_ = 0x00;
	if(*y > (T_Y_R1 - ERROR_Y) && *y < (T_Y_R1 + ERROR_Y)){
		pressed->Y_ |= (0x80);
	}
	else if(*y > (T_Y_R2 - ERROR_Y) && *y < (T_Y_R2 + ERROR_Y)){
		pressed->Y_ |= (0x40);
	}
	else if(*y > (T_Y_R3 - ERROR_Y) && *y < (T_Y_R3 + ERROR_Y)){
		pressed->Y_ |= (0x20);
	}
	else if(*y > (T_Y_R4 - ERROR_Y) && *y < (T_Y_R4 + ERROR_Y)){
		pressed->Y_ |= (0x10);
	}
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
			button->value = 1;
			break;
		case 0x82:
			button->value = 2;
			break;
		case 0x81:
			button->value = 3;
			break;
		case 0x44:
			button->value = 4;
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




/*
 void scan(SPI_HandleTypeDef *spi, Scan *scan_reading){
	uint8_t a;
	uint8_t data_X[2] = {0};
	uint8_t data_Y[2] = {0};



	a = 0x00;
	a = START_BIT | X_POSITION;
	//a = START_BIT | X_POSITION | MODE_8BIT;
	memset(buffer_X, 0, sizeof(buffer_X));
	memset(buffer_Y, 0, sizeof(buffer_Y));
	memset(data_X, 0, sizeof(data_X));
	memset(data_Y, 0, sizeof(data_Y));
	memset(buffer, 0, sizeof(buffer));
	memset(buff, 0, sizeof(buff));
	memset(temp1, 0, sizeof(temp1));
	memset(temp2, 0, sizeof(temp2));

	T_CS_RESET; // @suppress("Statement has no effect")
	//T_IRQ_RESET;
	HAL_SPI_Transmit(&hspi4, &a, 1, 1000); // @suppress("Statement has no effect")
	//T_IRQ_SET;
	//T_IRQ_RESET;
	HAL_SPI_Receive(&hspi4, data_X, 2, 1000);
	T_CS_SET;
	//T_IRQ_SET;

	osDelay(100);

	a = 0x01;
	a = START_BIT | Y_POSITION;
	//a = START_BIT | Y_POSITION | MODE_8BIT;
	T_CS_RESET;
	//T_IRQ_RESET;
	HAL_SPI_Transmit(&hspi4, &a, 1, 1000);
	//T_IRQ_SET;
	//T_IRQ_RESET;
	HAL_SPI_Receive(&hspi4, data_Y, 2, 1000);
	T_CS_SET;
	//T_IRQ_SET;

	uint16_t r_X = data_X[0] << 4 | (data_X[1] >> 3);
	uint16_t r_Y = data_Y[0] << 4 | (data_Y[1] >> 3);
	read_col(pressed, &r_X);
	read_row(pressed, &r_Y);

	get_button(button, pressed);
	int n = button->value;
	if(n != -1){
		if(n > -1){
			button->valid = true;
		}

		else{
			button->valid = true;
			button->rst = true;
			if(n == -3){button->set = true;}
		}
	}

	if(print && n != -1){
		sprintf(buffer_X, "X_-> %X | X_ANL -> %d ", pressed->X_, r_X);
		print_hex_f_array(uart, (uint8_t *)buffer_X, 1000, false, false, false, true, false);

		sprintf(buffer_Y, "Y_ -> %X | Y_ANL -> %d", pressed->Y_, r_Y);
		print_hex_f_array(uart, (uint8_t *)buffer_Y, 1000, false, false, false, true, false);
	}
}
  */
