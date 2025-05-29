#include "GUI.h"
#include "main.h"
#include "FONT.h"
#include "types.h"

extern SDRAM_HandleTypeDef hsdram1;
extern UART_HandleTypeDef huart8;
PAD pad ={
		.color = RGB565_Granite_gray,
		.height = 50,
		.w_gap = 10,
		.width = 100,
		.h_gap = 10,
		.start_x = 430,
		.start_y = 110,
		.word_color = RGB565_Black,
		.stop_color = RGB565_Red_orange
};

TERMINAL terminal = {
		.color = RGB565_Granite_gray,
		.column_count = 0,
		.first_line = NULL,
		.height = 230,
		.last_line = NULL,
		.line_count = 0,
		.width = 200,
		.word_color = RGB565_Green_lizard,
		.x_start = 0,
		.y_start = 10,
		.header_gap = 30
};


/*******************************************************************
 * @name       :void GUI_DrawPoint(u16 x,u16 y,u16 color)
 * @date       :2018-08-09
 * @function   :draw a point in LCD screen
 * @parameters :x:the x coordinate of the point
                y:the y coordinate of the point
								color:the color value of the point
 * @retvalue   :None
********************************************************************/
/*
int y_ = 0;
uint16_t yellow_rgb565 = RGB565_Redwood;
for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
      	  uint32_t index = y * FRAMEBUFFER_WIDTH + x; // 1D index
      	  uint32_t address = (uint32_t)BUFFER_LAYER_2_ADDRESS + index * BYTES_PER_PIXEL; // SDRAM address
            HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)address, &yellow_rgb565, 1);
            address += BYTES_PER_PIXEL; // Increment by 2 bytes for each pixel
        }
    }
    */
void GUI_DrawPoint(uint32_t frameBuffer, u32 x,u32 y,u16 color)
{
	//u32 index = (x + y*FRAMEBUFFER_WIDTH)*BYTES_PER_PIXEL;
	u32 address = (frameBuffer + (x + y*(FRAMEBUFFER_WIDTH))*BYTES_PER_PIXEL);

	//char msg[200];
	//sprintf(msg, "Values-> x: %d, y: %d, index: %d, address: %X\n", (int)x, (int)y, (int)index, (int)address);
	//HAL_UART_Transmit(&huart8, (uint8_t *)msg, strlen(msg), 1000);

	HAL_SDRAM_Write_16b(&hsdram1, (uint32_t *)address, &color, 1);
	//HAL_Delay(100);
	//frameBuffer[x + y*(FRAMEBUFFER_WIDTH)] = color;//���ù��λ��
}

/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(uint32_t frameBuffer, u32 sx,u32 sy,u32 ex,u32 ey,u16 color)
{
	u32 i,j;
	u32 width=ex-sx+1;
	u32 height=ey-sy+1;
	for(i=0;i<width;i++){
		for(j=0;j<height;j++){
			GUI_DrawPoint(frameBuffer, sx+i, sy+j, color);
		}
	}
}

/*****************************************************************************
 * @name       :void LCD_ShowChar(u16 x,u16 y,u16 fc, u16 bc, u8 num,u8 size,u8 mode)
 * @date       :2018-08-09
 * @function   :Display a single English character
 * @parameters :x:the bebinning x coordinate of the Character display position
                y:the bebinning y coordinate of the Character display position
								fc:the color value of display character
								bc:the background color of display character
								num:the ascii code of display character(0~94)
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowChar(uint32_t frameBuffer, u32 x,u32 y, u8 num,u8 size, u16 color)
{
    u32 temp;
    u8 pos,t;

	num=num-' ';//�õ�ƫ�ƺ��ֵ

	if(size < 20){
		for(pos=0;pos<16;pos++)
		{
			temp=asc2_1608[num][pos];		 //����1608����
			for(t=0;t<8;t++)
			{
				if(temp&0x01)GUI_DrawPoint(frameBuffer, x+t,y+pos, color);//��һ����
				temp>>=1;
			}
		}
	}

	else{
		for(pos=0;pos<24;pos++)
		{
			temp=Font24[num][pos*3] << 16 | Font24[num][pos*3+1] << 8 | Font24[num][pos*3 + 2];		 //����1608����
			for(t=0;t<24;t++)
			{
				if(temp&0x800000)GUI_DrawPoint(frameBuffer, x+t,y+pos, color);//��һ����
				temp<<=1;
			}
		}
	}
}

/*****************************************************************************
 * @name       :void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
 * @date       :2018-08-09
 * @function   :Display English string
 * @parameters :x:the bebinning x coordinate of the English string
                y:the bebinning y coordinate of the English string
								p:the start address of the English string
								size:the size of display character
								mode:0-no overlying,1-overlying
 * @retvalue   :None
******************************************************************************/
void LCD_ShowString(uint32_t frameBuffer, u32 x,u32 y,u8 displaySize,char *p,u16 color_front, u16 color_back, bool background)
{
	// Clear Background pixels with back color
	uint32_t xe = strlen(p)*8 + x;
	uint32_t ye = 16 + y;
	if(displaySize == 24){
		xe = strlen(p)*12 + x;
		ye = 20 + y;
	}
	if(background) LCD_Fill(frameBuffer, x, y, xe, ye, color_back);

	for(int i=0; i<strlen(p); i++)
	{
		if(x>(WIDTH - offsetX)||y>(HEIGHT-offsetY - 1))
		return;
		LCD_ShowChar(frameBuffer, x,y,p[i],displaySize,color_front);
		x+=displaySize/2;
	}
}

void keypad(uint32_t frameBuffer){

	uint32_t x_ = pad.start_x;
	uint32_t y_ = pad.start_y;
	uint32_t x_string;
	uint32_t y_string ;
	char padNumbers[4][3][6] = {{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", "9"}, {"CLR", "0", "SET"}};
	char stop[5] = "STOP";
	char run[5] = "RUN";
	LCD_Fill(frameBuffer, x_ + 2*(pad.width + pad.w_gap), y_ - (pad.height + pad.h_gap), x_ + 2*(pad.width + pad.w_gap)
			+ pad.width , y_ - pad.h_gap, pad.stop_color);
	LCD_Fill(frameBuffer, x_ + (pad.width + pad.w_gap), y_ - (pad.height + pad.h_gap), x_ + (pad.width + pad.w_gap)
			+ pad.width , y_ - pad.h_gap, pad.stop_color);

	x_string = (int)(x_ + 2*(pad.width + pad.w_gap) + (pad.width - strlen(stop)*word_width)/2);
	y_string = (int)(y_ - (pad.height + pad.h_gap) + (pad.height - word_height)/2);
	LCD_ShowString(frameBuffer, x_string, y_string, 16, stop, pad.word_color, pad.stop_color, false);

	x_string = (int)(x_ + (pad.width + pad.w_gap) + (pad.width - strlen(stop)*word_width)/2);
	y_string = (int)(y_ - (pad.height + pad.h_gap) + (pad.height - word_height)/2);
	LCD_ShowString(frameBuffer, x_string, y_string, 16, run, pad.word_color, pad.stop_color, false);



	for(int i=0; i< 4; i++){
		for(int j = 0; j< 3; j++){
			uint32_t x = x_ + j*(pad.width + pad.w_gap);
			uint32_t xe = x + pad.width;

			uint32_t y = y_ + i*(pad.height + pad.h_gap);
			uint32_t ye = y + pad.height;
			LCD_Fill(frameBuffer, x, y, xe, ye, pad.color);

			x_string = (int)(x + (pad.width - strlen(padNumbers[i][j])*word_width)/2);
			y_string = (int)(y + (pad.height - word_height)/2);
			LCD_ShowString(frameBuffer, x_string, y_string, 16, padNumbers[i][j], pad.word_color, pad.color, false);
		}
	}

}

void draw_terminal(uint32_t frameBuffer){
	char header[10] = "Terminal";

	uint32_t x = terminal.x_start;
	uint32_t xe = x + terminal.width;

	uint32_t y = terminal.y_start;
	uint32_t ye = y + terminal.height;
	char msg[100];
	sprintf(msg, "Values-> x: %d, xe: %d, y: %d, ye: %d\n", (int)x, (int)xe, (int)y, (int)ye);
	HAL_UART_Transmit(&huart8, (uint8_t *)msg, strlen(msg), 1000);
	LCD_ShowString(frameBuffer, x, y + terminal.header_gap , 16, header, RGB565_Black, terminal.word_color, false);
	LCD_Fill(frameBuffer, x, y, xe, ye, terminal.color);
	draw_var(frameBuffer);


}

void draw_var(uint32_t frameBuffer){
	char set[10] = "SET-> ";
	char hot[10] = "HOT-> ";
	char out[10] = "OUT-> ";
	char cmd[10] = "CMD-> ";
	char stp[10] = "STP-> ";
	int i = 0, space = 30;
	LCD_ShowString(frameBuffer, terminal.width + 20, terminal.y_start + space*i++, 16, set, terminal.word_color, RGB565_Black, false);
	LCD_ShowString(frameBuffer, terminal.width + 20, terminal.y_start + space*i++, 16, hot, terminal.word_color, RGB565_Black, false);
	LCD_ShowString(frameBuffer, terminal.width + 20, terminal.y_start + space*i++, 16, out, terminal.word_color, RGB565_Black, false);
	LCD_ShowString(frameBuffer, terminal.width + 20, terminal.y_start + space*i++, 16, cmd, terminal.word_color, RGB565_Black, false);
	LCD_ShowString(frameBuffer, terminal.width + 20, terminal.y_start + space*i++, 16, stp, terminal.word_color, RGB565_Black, false);

}

void printf_(uint32_t frameBuffer, char *msg){
	if(msg == NULL) return;

	char p1[18];
	LINE *temp, *temp1;


	if(strlen(msg) < 23){
		addLine();
		strcpy(p1, msg);
		strcpy(terminal.last_line->line_msg, msg);
		if(terminal.line_count > 13){
			terminal.line_count = 0;
			temp1 = terminal.first_line;
			terminal.first_line = terminal.first_line->next;
			free(temp1);
			draw_terminal(frameBuffer);

			for(temp = terminal.first_line; temp != NULL; temp = temp->next){
				LCD_ShowString(frameBuffer, terminal.x_start, terminal.y_start+(terminal.line_count++)*word_height,
						16, temp->line_msg, terminal.word_color, terminal.color, false);
				//HAL_Delay(1);
			}
		}
		else{
			temp = terminal.last_line;
			LCD_ShowString(frameBuffer, terminal.x_start, terminal.y_start+(terminal.line_count++)*word_height/2,
					    16, temp->line_msg, terminal.word_color, terminal.color, false);
			//HAL_Delay(1);
		}
	}
}

void printf_g(uint32_t frameBuffer, int x, int y){
	GUI_DrawPoint(frameBuffer, (int)x, (int)y, terminal.word_color);
}

// TERMINAL
void addLine(){
	LINE *newLine = malloc(sizeof(LINE));
	newLine->next = NULL;
	memset(newLine->line_msg, 0, sizeof(newLine->line_msg)) ;
    if(terminal.line_count == 0){
    	terminal.last_line = newLine;
    	terminal.first_line = newLine;
    }
    else{
    	terminal.last_line->next = newLine;
    	terminal.last_line = newLine;
    }
    terminal.line_count ++;
    return;
}


void LCD_UpdateTypedTemp(uint32_t frameBuffer, char *c){
	char msg[30] = {0};
	sprintf(msg, "-> ");
	strcat(msg, c);
	LCD_Fill(frameBuffer, 400, 90, 530, 90+word_height, color_index);
	LCD_ShowString(frameBuffer, 400, 90, 16, msg, terminal.word_color, color_index, false);
}


void draw_futuristic_background(uint32_t frameBuffer) {
    for (uint16_t y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (uint16_t x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            // Vertical gradient from blue to black
            uint8_t brightness = 255 - (y * 255 / FRAMEBUFFER_HEIGHT);
            uint16_t baseColor = RGB565(0, brightness / 2, brightness);

            // Add horizontal scanlines every 4 pixels
            if ((y % 4) == 0)
                baseColor = RGB565(0, ((brightness / 2) + 30), brightness);

            // Add vertical grid lines every 50 pixels
            if (x % 50 == 0)
                baseColor = RGB565(brightness, brightness / 3, brightness / 3);

            // Add glowing diagonal streak
            if ((x + y) % 200 < 2)
                baseColor = RGB565(0, 255, 180); // cyan glow

            GUI_DrawPoint(frameBuffer, x, y, baseColor);
        }
    }
}

void drawLine(uint32_t frameBuffer, int x0, int y0, int x1, int y1, uint32_t color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        GUI_DrawPoint(frameBuffer, x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2*err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void stampSquare(uint32_t frameBuffer, int cx, int cy, int half, uint32_t color) {
    for(int dy = -half; dy <= half; dy++) {
        for(int dx = -half; dx <= half; dx++) {
            GUI_DrawPoint(frameBuffer, cx + dx, cy + dy, color);
        }
    }
}

void drawThickLine(uint32_t frameBuffer,
                          int x0, int y0,
                          int x1, int y1,
                          uint32_t color,
                          int thickness)
{
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    int half = thickness/2;
    while (1) {
        stampSquare(frameBuffer, x0, y0, half, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2*err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_circle(uint32_t frameBuffer,int cx, int cy, int radius, uint32_t color)
{
    int x = radius;
    int y = 0;
    int err = 1 - radius;

    while (x >= y) {
        // eight octants
        GUI_DrawPoint(frameBuffer, cx + x, cy + y, color);
        GUI_DrawPoint(frameBuffer, cx - x, cy + y, color);
        GUI_DrawPoint(frameBuffer, cx + x, cy - y, color);
        GUI_DrawPoint(frameBuffer, cx - x, cy - y, color);
        GUI_DrawPoint(frameBuffer, cx + y, cy + x, color);
        GUI_DrawPoint(frameBuffer, cx - y, cy + x, color);
        GUI_DrawPoint(frameBuffer, cx + y, cy - x, color);
        GUI_DrawPoint(frameBuffer, cx - y, cy - x, color);

        y++;
        if (err < 0) {
            err += 2*y + 1;
        } else {
            x--;
            err += 2*(y - x) + 1;
        }
    }
}

void draw_circle_filled(uint32_t frameBuffer, int cx, int cy, int radius, uint32_t color)
{
    for (int dy = -radius; dy <= radius; dy++) {
        // for this scan-line, how far out do we go?
        int dx = (int)(sqrt((double)(radius*radius - dy*dy)) + 0.5);
        for (int x = -dx; x <= dx; x++) {
            GUI_DrawPoint(frameBuffer, cx + x, cy + dy, color);
        }
    }
}
