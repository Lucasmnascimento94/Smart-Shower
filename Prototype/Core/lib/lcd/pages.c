#include"pages.h"
/* PAGES STRUCTS */

#define wifi_status_x 600
#define wifi_status_y 100
#define wifi_status_r 10
#define wifi_color_connected RGB565_Green
#define wifi_color_connecting RGB565_Yellow
#define wifi_color_disconnected RGB565_Red

#define box_1_START_X 650
#define box_1_START_Y 200
#define box_1_HEIGHT_GAP 10
#define box_1_WIDTH_GAP 5
#define box_1_WIDTH 50
#define box_1_HEIGHT 40

#define arrow_HEIGHT 30
#define arrow_WIDHT 30

#define box_2_START_X 100
#define box_2_START_Y 400
#define box_2_WIDTH_GAP 20
#define box_2_HEIGHT_GAP 10
#define box_2_WIDTH 100
#define box_2_HEIGHT 60

#define statistic_box_START_X 10
#define statistic_box_WIDTH 550
#define statistic_box_HEIGHT 350
#define statistic_box_START_Y 50



PAGE_HOME hpage;
// void LCD_Fill(uint32_t frameBuffer, u32 sx,u32 sy,u32 ex,u32 ey,u16 color)

void PAGE_HOME_INIT_VAR(void){
	// Statistic Panel Var
	hpage.statisticPanel_x = statistic_box_START_X;
	hpage.statisticPanel_xe = statistic_box_START_X + statistic_box_WIDTH;
	hpage.statisticPanel_y = statistic_box_START_Y;
	hpage.statisticPanel_ye = statistic_box_START_Y + statistic_box_HEIGHT;
	hpage.statisticalPanel_backgroundColor = RGB565_Violet_blue;
	hpage.statisticalPanel_hotTempColor = RGB565_Red;
	hpage.statisticalPanel_outTempColor = RGB565_Orange;
	hpage.statisticalPanel_wordColor = RGB565_White;

	// Left Arrow
	hpage.arrowLeft_x = box_1_START_X - box_1_WIDTH_GAP - arrow_WIDHT;
	hpage.arrowLeft_xe = box_1_START_X - box_1_WIDTH_GAP;
	hpage.arrowLeft_y = box_1_START_Y + arrow_HEIGHT/4;
	hpage.arrowLeft_ye = hpage.arrowLeft_y + arrow_HEIGHT;
	// Right Arrow
	hpage.arrowRight_x = box_1_START_X + box_1_WIDTH + box_1_WIDTH_GAP;
	hpage.arrowRight_xe = hpage.arrowRight_x + arrow_WIDHT;
	hpage.arrowRight_y = hpage.arrowLeft_y;
	hpage.arrowRight_ye = hpage.arrowLeft_ye;
	// Arrows Colors
	hpage.arrow_backgroundColor = RGB565_Celadon_blue;
	hpage.arrow_arrowColor = RGB565_Moonstone;

	// Box 1 - SETTINGS
	hpage.setBox_x = box_1_START_X;
	hpage.setBox_xe = box_1_START_X + box_1_WIDTH;
	hpage.setBox_y = box_1_START_Y;
	hpage.setBox_ye = box_1_START_Y + box_1_HEIGHT;
	// Home Box
	hpage.homeBox_x = hpage.setBox_x;
	hpage.homeBox_xe = hpage.setBox_xe;
	hpage.homeBox_y = hpage.setBox_ye + box_1_HEIGHT_GAP;
	hpage.homeBox_ye = hpage.homeBox_y + box_1_HEIGHT;
	// Conf Box
	hpage.confBox_x = hpage.setBox_x;
	hpage.confBox_xe = hpage.setBox_xe;
	hpage.confBox_y = hpage.homeBox_ye + box_1_HEIGHT_GAP;
	hpage.confBox_ye = hpage.confBox_y + box_1_HEIGHT;
	//Box 1 Colors
	hpage.setBox_backgroundColor = RGB565_Blue_pantone;
	hpage.setBox_wordColor = RGB565_White;

	//Box 2 - READINGS
	hpage.setTemperature_x = box_2_START_X;
	hpage.setTemperature_xe = box_2_START_X + box_2_WIDTH;
	hpage.setTemperature_y = hpage.statisticPanel_ye + box_2_HEIGHT_GAP;
	hpage.setTemperature_ye = hpage.setTemperature_y + box_2_HEIGHT;
	//Out Temp
	hpage.outTemperature_x = hpage.setTemperature_xe + box_2_WIDTH_GAP;
	hpage.outTemperature_xe = hpage.outTemperature_x + box_2_WIDTH;
	hpage.outTemperature_y = hpage.setTemperature_y;
	hpage.outTemperature_ye = hpage.setTemperature_ye;
	//Hot Temp
	hpage.hotTemperature_x = hpage.outTemperature_xe + box_2_WIDTH_GAP;
	hpage.hotTemperature_xe = hpage.hotTemperature_x  + box_2_WIDTH;
	hpage.hotTemperature_y = hpage.setTemperature_y;
	hpage.hotTemperature_ye = hpage.setTemperature_ye;
	//Box 2 Colors
	hpage.setTemperature_backgroundColor = RGB565_Dodger_blue;
	hpage.setTemperature_wordColor = RGB565_White;

}
void PAGE_HOME_Draw(uint32_t frameBuffer){
	draw_futuristic_background(frameBuffer);
	draw_statistic_panel(frameBuffer);
	draw_box_1(frameBuffer);
	draw_box_2(frameBuffer);
}

void PAGE_CONF_Draw(uint32_t frameBuffer){
}

void draw_wifi_status(uint32_t frameBuffer){
	draw_circle_filled(frameBuffer, wifi_status_x, wifi_status_y, wifi_status_r, wifi_color_connected);
	LCD_ShowString(frameBuffer, wifi_status_x + wifi_status_r + 10, wifi_status_y - wifi_status_r, 24, "WiFi Status",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);
}

void draw_statistic_panel(uint32_t frameBuffer){
	LCD_Fill(frameBuffer, hpage.statisticPanel_x, hpage.statisticPanel_y,
			hpage.statisticPanel_xe, hpage.statisticPanel_ye, hpage.statisticalPanel_backgroundColor);
}
void plot_statistic_panel(uint32_t frameBuffer){

}

void draw_box_1(uint32_t frameBuffer){
	//RGB565_Green
	draw_wifi_status(frameBuffer);
	//LCD_Fill(frameBuffer, hpage.arrowLeft_x, hpage.arrowLeft_y, hpage.arrowLeft_xe, hpage.arrowLeft_ye, hpage.arrow_backgroundColor);
	draw_arrow_left(frameBuffer, 4);
	//LCD_Fill(frameBuffer, hpage.arrowRight_x, hpage.arrowRight_y, hpage.arrowRight_xe, hpage.arrowRight_ye, hpage.arrow_backgroundColor, 4);
	draw_arrow_right(frameBuffer, 4);
	//LCD_Fill(frameBuffer, hpage.setBox_x, hpage.setBox_y, hpage.setBox_xe, hpage.setBox_ye, hpage.setBox_backgroundColor);
	LCD_ShowString(frameBuffer, hpage.setBox_x + box_1_WIDTH/4, hpage.setBox_y + box_1_HEIGHT/4, 24, "25",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);

	//LCD_Fill(frameBuffer, hpage.homeBox_x, hpage.homeBox_y, hpage.homeBox_xe, hpage.homeBox_ye, hpage.setBox_backgroundColor);
	LCD_ShowString(frameBuffer, hpage.homeBox_x + box_1_WIDTH/4, hpage.homeBox_y + box_1_HEIGHT/4, 24, "SET",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);
	//LCD_Fill(frameBuffer, hpage.confBox_x, hpage.confBox_y, hpage.confBox_xe, hpage.confBox_ye, hpage.setBox_backgroundColor);
	LCD_ShowString(frameBuffer, hpage.confBox_x + box_1_WIDTH/4, hpage.confBox_y + box_1_HEIGHT/4, 24, "STOP",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);
}

// LCD_ShowChar(uint32_t frameBuffer, u32 x,u32 y, u8 num,u8 size, u16 color)
void draw_box_1_words(uint32_t frameBuffer){

}

void update_setBox(uint32_t frameBuffer, int newTemp){
	char temp[20];
	sprintf(temp, "%d", newTemp);
	LCD_ShowString(frameBuffer, hpage.setBox_x + box_1_WIDTH/4, hpage.setBox_y + box_1_HEIGHT/4, 24, temp,
	hpage.setBox_wordColor, hpage.setBox_backgroundColor, true);
}
void animate_setBox(uint32_t frameBuffer, int newTemp){
}
void animate_homeBox(uint32_t frameBuffer){

}
void update_confBox(uint32_t frameBuffer){

}

void draw_box_2(uint32_t frameBuffer){
	LCD_ShowString(frameBuffer, hpage.setTemperature_x + box_1_WIDTH/4, hpage.setTemperature_y + box_1_HEIGHT/4, 24, "Calling",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);

	LCD_ShowString(frameBuffer, hpage.outTemperature_x + box_1_WIDTH/4, hpage.outTemperature_y + box_1_HEIGHT/4, 24, "Shower",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);

	LCD_ShowString(frameBuffer, hpage.hotTemperature_x + box_1_WIDTH/4, hpage.hotTemperature_y + box_1_HEIGHT/4, 24, "Hot Water",
			hpage.setBox_wordColor, hpage.setBox_backgroundColor, false);
}
void update_setTemp(uint32_t frameBuffer, char *temp){
	LCD_ShowString(frameBuffer, hpage.setTemperature_x + box_1_WIDTH/4, hpage.setTemperature_y + box_1_HEIGHT, 24, temp,
	hpage.setBox_wordColor, hpage.setBox_backgroundColor, true);
}
void update_outTemp(uint32_t frameBuffer, char *temp){
	LCD_ShowString(frameBuffer, hpage.outTemperature_x + box_1_WIDTH/4, hpage.outTemperature_y + box_1_HEIGHT, 24, temp,
	hpage.setBox_wordColor, hpage.setBox_backgroundColor, true);
}
void update_hotTemp(uint32_t frameBuffer, char *temp){
	LCD_ShowString(frameBuffer, hpage.hotTemperature_x + box_1_WIDTH/4, hpage.hotTemperature_y + box_1_HEIGHT, 24, temp,
	hpage.setBox_wordColor, hpage.setBox_backgroundColor, true);
}

void draw_arrow_left(uint32_t frameBuffer, int thickness){

    const int W = arrow_WIDHT;
    const int H = arrow_HEIGHT;
    int midY = hpage.arrowLeft_y + H/2;

    // upper arm: from top-right down to midpoint
    drawThickLine(frameBuffer,
    		hpage.arrowLeft_x + (W - 1), hpage.arrowLeft_y,
			hpage.arrowLeft_x,             midY,
				  hpage.arrow_arrowColor, thickness);

    // lower arm: from bottom-right up to midpoint
    drawThickLine(frameBuffer,
    		hpage.arrowLeft_x + (W - 1), hpage.arrowLeft_y + (H - 1),
			hpage.arrowLeft_x,             midY,
				  hpage.arrow_arrowColor, thickness);
}
void animate_arrow_left(uint32_t frameBuffer, int thickness){
	draw_arrow_left(frameBuffer, 6);
	HAL_Delay(500);
	draw_arrow_left(frameBuffer, 4);

}

void draw_arrow_right(uint32_t frameBuffer, int thickness){

    const int W = arrow_WIDHT;
    const int H = arrow_HEIGHT;
    int x = hpage.arrowRight_x;
    int y = hpage.arrowRight_y;
    int midY = y + H/2;

    // top-left to center-right
    drawThickLine(frameBuffer,
        x,         y,
        x + W - 1, midY,
        hpage.arrow_arrowColor, thickness);

    // bottom-left to center-right
    drawThickLine(frameBuffer,
        x,         y + H - 1,
        x + W - 1, midY,
        hpage.arrow_arrowColor, thickness);
}
void animate_arrow_right(uint32_t frameBuffer, int thickness){

}

