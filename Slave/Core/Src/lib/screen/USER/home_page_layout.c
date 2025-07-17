/*
 * gui_layouts.c
 *
 *  Created on: Jul 14, 2025
 *      Author: lucas
 */


#include "home_page_layout.h"
#include "FONT.h"
#include "lcd.h"
#include "screen.h"
#include "serial_printing.h"
#include "string.h"
#include "GUI.h"
int LinkScreenNodes();
void linkCanvasToCanvas(struct _canvas** canvases);
void linkCanvasToText(struct _canvas** canvases);
void linkPagesToCanvas(struct _canvas** canvases);
int LinkCanvas();
struct _canvas* popCanvasNode(CanvasStack* stack);
void pushCanvasNode(CanvasStack* stack, struct _canvas* canvas);
void GUI_VAR_INIT();
void PROCESS_VAR_PARENT(struct _canvas* canvas);
void canvasDrawHandle(struct _canvas* canvas);
void PROCESS_VAR_SIBLING(struct _canvas* canvas);


// --- Color Definitions (Example RGB565) ---
#define DARK_BG         0x18E3
#define STATUS_GREEN    0x3E20
#define STATUS_ORANGE   0xFD00
#define STATUS_BLUE     0x1D9F
#define STATUS_GRAY     0x7BEF
#define OUTPUT_BLUE     0x0C9F
#define TARGET_ORANGE   0xE2E4
#define HOT_H2O_RED     0xC166
#define VALVE_GREEN     0x156A
#define CALC_VLV_PURPLE 0x815C
#define ARROW_RED       0xD104
#define ESTOP_RED       0xF800
#define FOOTER_GRAY     0x8410
#define WHITE           0xFFFF
#define BLACK           0x0000
#define GREEN           0x07E0
#define RED             0xF800
#define BROWN           0xA145


// --- Page & Screen Management Globals ---
char* PageList[] = {
    "home_page",
	"hist_page"
};

struct _pageIndex pageIndex = {
    .Page_IDs = PageList,
    .pageCount = 2
};

Screen_t screen = {
    .count = 0,
    .home_page = NULL,
    .last_page = NULL
};

Page_t home_page = {
    .next_page = NULL,
};


/*#######################################################################################################
#                                         BACKGROUND                                                  #
#######################################################################################################*/
struct CanvasConf_t home_canvas_conf = {
    .Canvas_ID = "home_canvas", .Parent_ID = NULL, .Page_ID = "home_page",
    .canvasBackgroundColor = DARK_BG,
    .canvasWidth = screenWidth, .canvasHeight = screenHeight,
    .canvasStart_x = 0, .canvasStart_y = 0,
    .offset_top = 0, .offset_left = 0,
    .display = "vertical"
};

/*#######################################################################################################
#                                             HEADER                                                    #
#######################################################################################################*/
struct CanvasConf_t header_container_conf = {
    .Canvas_ID = "header_container", .Parent_ID = "home_canvas", .Page_ID = "home_page",
    .offset_top = 10, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 28,
    .display = "vertical"
};
struct _textConf_t header_text_conf = {
    .Text_ID = "header_text", .Canvas_ID = "header_container", .text = "VALVE CONTROLLER",
    .text_color = STATUS_GRAY, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct _textConf_t header_subtext_conf = {
    .Text_ID = "header_subtext", .Canvas_ID = "header_container", .text = "Shower Temperature Control",
    .text_color = STATUS_GRAY, .font_size = 12, .offset_top = 14, .offset_left = 0
};

/*#######################################################################################################
#                                          STATUS BAR                                                   #
#######################################################################################################*/
struct CanvasConf_t status_container_conf = {
    .Canvas_ID = "status_container", .Parent_ID = "home_canvas", .Page_ID = "home_page",
    .offset_top = 10, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 25,
    .display = "horizontal"
};
struct CanvasConf_t power_button_container_conf = {
    .Canvas_ID = "power_button_container", .Parent_ID = "status_container",
    .canvasBackgroundColor = STATUS_GREEN, .canvasWidth = 60, .canvasHeight = 25,
    .offset_top = 0, .offset_left = 0
};
struct _textConf_t power_button_text_conf = {
    .Text_ID = "power_button_text", .Canvas_ID = "power_button_container", .text = "ON",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t temperature_status_container_conf = {
    .Canvas_ID = "temperature_status_container", .Parent_ID = "status_container",
    .canvasBackgroundColor = STATUS_ORANGE, .canvasWidth = 90, .canvasHeight = 25,
    .offset_top = 0, .offset_left = 5
};
struct _textConf_t temperature_status_text_conf = {
    .Text_ID = "temperature_status_text", .Canvas_ID = "temperature_status_container", .text = "ADJUSTING",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t wifi_status_container_conf = {
    .Canvas_ID = "wifi_status_container", .Parent_ID = "status_container",
    .canvasBackgroundColor = STATUS_BLUE, .canvasWidth = 50, .canvasHeight = 25,
    .offset_top = 0, .offset_left = 5
};
struct _textConf_t wifi_status_text_conf = {
    .Text_ID = "wifi_status_text", .Canvas_ID = "wifi_status_container", .text = "WIFI",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t history_icon_container_conf = {
    .Canvas_ID = "history_icon_container", .Parent_ID = "status_container",
    .canvasBackgroundColor = STATUS_GRAY, .canvasWidth = 25, .canvasHeight = 25,
    .offset_top = 0, .offset_left = 5
};
struct _textConf_t history_icon_text_conf = {
    .Text_ID = "history_icon_text", .Canvas_ID = "history_icon_container", .text = "H",
    .text_color = BLACK, .font_size = 12, .offset_top = 0, .offset_left = 0
};

/*#######################################################################################################
#                                     TEMPERATURE DISPLAYS                                              #
#######################################################################################################*/
struct CanvasConf_t OUT_TARG_container_conf = {
    .Canvas_ID = "OUT_TARG_container", .Parent_ID = "home_canvas",
    .offset_top = 10, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 55, .display = "horizontal"
};
struct CanvasConf_t OUT_container_conf = {
    .Canvas_ID = "OUT_container", .Parent_ID = "OUT_TARG_container",
    .canvasBackgroundColor = OUTPUT_BLUE, .canvasWidth = 105, .canvasHeight = 55,
    .offset_top = 0, .offset_left = 0, .display = "vertical"
};
struct _textConf_t OUT_text_label_conf = {
    .Text_ID = "out_label", .Canvas_ID = "OUT_container", .text = "OUTPUT",
    .text_color = WHITE, .font_size = 12, .offset_top = 8, .offset_left = 0
};
struct _textConf_t OUT_text_value_conf = {
    .Text_ID = "out_value", .Canvas_ID = "OUT_container", .text = "96.3",
    .text_color = WHITE, .font_size = 16, .offset_top = 28, .offset_left = 0
};
struct CanvasConf_t TARGET_container_conf = {
    .Canvas_ID = "TARGET_container", .Parent_ID = "OUT_TARG_container",
    .canvasBackgroundColor = TARGET_ORANGE, .canvasWidth = 105, .canvasHeight = 55,
    .offset_top = 0, .offset_left = 10, .display = "vertical"
};
struct _textConf_t TARGET_text_label_conf = {
    .Text_ID = "target_label", .Canvas_ID = "TARGET_container", .text = "TARGET",
    .text_color = WHITE, .font_size = 12, .offset_top = 8, .offset_left = 0
};
struct _textConf_t TARGET_text_value_conf = {
    .Text_ID = "target_value", .Canvas_ID = "TARGET_container", .text = "101",
    .text_color = WHITE, .font_size = 16, .offset_top = 28, .offset_left = 0
};
struct CanvasConf_t hot_h2o_container_conf = {
    .Canvas_ID = "hot_h2o_container", .Parent_ID = "home_canvas",
    .offset_top = 5, .offset_left = 0,
    .canvasBackgroundColor = HOT_H2O_RED, .canvasWidth = 220, .canvasHeight = 45, .display = "vertical"
};
struct _textConf_t hoth2o_label_conf = {
    .Text_ID = "hoth2o_label", .Canvas_ID = "hot_h2o_container", .text = "HOT H2O",
    .text_color = WHITE, .font_size = 12, .offset_top = 5, .offset_left = 0
};
struct _textConf_t hoth2o_value_conf = {
    .Text_ID = "hoth2o_value", .Canvas_ID = "hot_h2o_container", .text = "142.2",
    .text_color = WHITE, .font_size = 16, .offset_top = 22, .offset_left = 0
};

/*#######################################################################################################
#                                       VALVE DISPLAYS                                                  #
#######################################################################################################*/
struct CanvasConf_t valve_container_conf = {
    .Canvas_ID = "valve_container", .Parent_ID = "home_canvas",
    .offset_top = 5, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 45, .display = "horizontal"
};
struct CanvasConf_t valve_pos_conf = {
    .Canvas_ID = "valve_pos", .Parent_ID = "valve_container",
    .canvasBackgroundColor = VALVE_GREEN, .canvasWidth = 105, .canvasHeight = 45,
    .offset_top = 0, .offset_left = 0, .display = "vertical"
};
struct _textConf_t valve_label_conf = {
    .Text_ID = "valve_label", .Canvas_ID = "valve_pos", .text = "VALVE",
    .text_color = WHITE, .font_size = 12, .offset_top = 5, .offset_left = 0
};
struct _textConf_t valve_value_conf = {
    .Text_ID = "valve_value", .Canvas_ID = "valve_pos", .text = "2047 | 64%",
    .text_color = WHITE, .font_size = 12, .offset_top = 25, .offset_left = 0
};
struct CanvasConf_t calc_valve_pos_conf = {
    .Canvas_ID = "calc_valve_pos", .Parent_ID = "valve_container",
    .canvasBackgroundColor = CALC_VLV_PURPLE, .canvasWidth = 105, .canvasHeight = 45,
    .offset_top = 0, .offset_left = 10, .display = "vertical"
};
struct _textConf_t calc_valve_label_conf = {
    .Text_ID = "calc_valve_label", .Canvas_ID = "calc_valve_pos", .text = "CALC VALVE",
    .text_color = WHITE, .font_size = 12, .offset_top = 5, .offset_left = 0
};
struct _textConf_t calc_valve_value_conf = {
    .Text_ID = "calc_valve_value", .Canvas_ID = "calc_valve_pos", .text = "1596 | 50%",
    .text_color = WHITE, .font_size = 12, .offset_top = 25, .offset_left = 0
};

/*#######################################################################################################
#                                       TEMP CONTROLS & FOOTER                                          #
#######################################################################################################*/
struct CanvasConf_t temp_ctrl_container_conf = {
    .Canvas_ID = "temp_ctrl_container", .Parent_ID = "home_canvas",
    .offset_top = 5, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 30, .display = "horizontal"
};
struct CanvasConf_t temp_ctrl_label_canvas = {
    .Canvas_ID = "temp_ctrl_label_canvas", .Parent_ID = "temp_ctrl_container",
    .canvasWidth = 150, .canvasHeight = 30, .offset_top = 0, .offset_left = 0,
};
struct _textConf_t temp_ctrl_label_conf = {
    .Text_ID = "temp_ctrl_label", .Canvas_ID = "temp_ctrl_label_canvas", .text = "Temperature Control",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t temp_down_btn_conf = {
    .Canvas_ID = "temp_down_btn", .Parent_ID = "temp_ctrl_container", .canvasBackgroundColor = ARROW_RED,
    .canvasWidth = 30, .canvasHeight = 30, .offset_top = 0, .offset_left = 5
};
struct CanvasConf_t temp_up_btn_conf = {
    .Canvas_ID = "temp_up_btn", .Parent_ID = "temp_ctrl_container", .canvasBackgroundColor = ARROW_RED,
    .canvasWidth = 30, .canvasHeight = 30, .offset_top = 0, .offset_left = 5
};
struct _textConf_t temp_down_text_conf = {
    .Text_ID = "temp_down_text", .Canvas_ID = "temp_down_btn", .text = "<",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct _textConf_t temp_up_text_conf = {
    .Text_ID = "temp_up_text", .Canvas_ID = "temp_up_btn", .text = ">",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t estop_btn_conf = {
    .Canvas_ID = "estop_btn", .Parent_ID = "home_canvas",
    .offset_top = 5, .offset_left = 0,
    .canvasBackgroundColor = ESTOP_RED, .canvasWidth = 220, .canvasHeight = 25,
};
struct _textConf_t estop_text_conf = {
    .Text_ID = "estop_text", .Canvas_ID = "estop_btn", .text = "EMERGENCY STOP",
    .text_color = WHITE, .font_size = 12, .offset_top = 0, .offset_left = 0
};
struct CanvasConf_t footer_text_canvas = {
    .Canvas_ID = "footer_canvas", .Parent_ID = "home_canvas",
    .offset_top = 5, .offset_left = 0,
    .canvasWidth = 220, .canvasHeight = 20
};
struct _textConf_t footer_text_conf = {
    .Text_ID = "footer_text", .Canvas_ID = "footer_canvas", .text = "Range: 80F-120F | Steps: 0-3200",
    .text_color = FOOTER_GRAY, .font_size = 12, .offset_top = 0, .offset_left = 0
};


/*#######################################################################################################
#                                       MANIFEST ARRAYS                                               #
#######################################################################################################*/
struct CanvasConf_t* page_conf[] = {
    &home_canvas_conf,
    &header_container_conf,
    &status_container_conf,
    &power_button_container_conf,
    &temperature_status_container_conf,
    &wifi_status_container_conf,
    &history_icon_container_conf,
    &OUT_TARG_container_conf,
    &OUT_container_conf,
    &TARGET_container_conf,
    &hot_h2o_container_conf,
    &valve_container_conf,
    &valve_pos_conf,
    &calc_valve_pos_conf,
    &temp_ctrl_container_conf,
    &temp_ctrl_label_canvas,
    &temp_down_btn_conf,
    &temp_up_btn_conf,
    &estop_btn_conf,
    &footer_text_canvas
};

struct _textConf_t* text_conf[] = {
    &header_text_conf,
    &header_subtext_conf,
    &power_button_text_conf,
    &temperature_status_text_conf,
    &wifi_status_text_conf,
    &history_icon_text_conf,
    &OUT_text_label_conf,
    &OUT_text_value_conf,
    &TARGET_text_label_conf,
    &TARGET_text_value_conf,
    &hoth2o_label_conf,
    &hoth2o_value_conf,
    &valve_label_conf,
    &valve_value_conf,
    &calc_valve_label_conf,
    &calc_valve_value_conf,
    &temp_ctrl_label_conf,
    &temp_down_text_conf,
    &temp_up_text_conf,
    &estop_text_conf,
    &footer_text_conf
};

int LinkScreenNodes(){
	for(int i=0; i< pageIndex.pageCount; i++){
		SerialPrint("LINK NODES\n\n");
		osDelay(1000);
		Page_t*  page = osMemoryPoolAlloc(PAGE_POOLHandle, osWaitForever);
		if(page == NULL){
			SerialPrint("Failed to Allocate Page\n");
			return -1;
		}

		page->index = i;
		page->Page_ID = pageIndex.Page_IDs[i];
		if(screen.count == 0){
			screen.home_page = page;
			screen.last_page = page;
		}
		else{
			screen.last_page->next_page = page;
			screen.last_page = page;
		}
		screen.count++;
	}

	if(LinkCanvas() != 0) return -1;

	return 0;
}



int LinkCanvas(){
	//SerialPrint("LINK CANVAS\n\n");
	//osDelay(2000);
	int size_canvas = sizeof(page_conf)/sizeof(struct CanvasConf_t*);
	int size_text = sizeof(text_conf)/sizeof(struct _textConf_t*);
	static struct _canvas* canvases[nCanvas] = {0};
	int i;

	static char num[40];
	// Allocate canvases settings
	for(i=0; i<size_canvas; i++){
		SerialPrint("LOOP 1\n");
		osDelay(500);
		canvases[i] = osMemoryPoolAlloc(CANVAS_POOLHandle, osWaitForever);
		if(canvases[i] == NULL){
			SerialPrint("Failed to Allocate Canvas\n");
			osDelay(2000);
			return -1;
		}
		canvases[i]->child_canvas = NULL;
		canvases[i]->index = 0;
		canvases[i]->next_canvas = NULL;
		canvases[i]->parent_canvas = NULL;
		canvases[i]->text = NULL;
		canvases[i]->settings = page_conf[i];
	}

	// Allocate Text settings
	for(int j =0; j<size_text; j++, i++){
		canvases[i] = osMemoryPoolAlloc(CANVAS_POOLHandle, osWaitForever);
		if(canvases[i] == NULL){
			SerialPrint("Failed to Allocate Canvas\n");
			osDelay(2000);
			return -1;
		}
		canvases[i]->child_canvas = NULL;
		canvases[i]->index = 0;
		canvases[i]->next_canvas = NULL;
		canvases[i]->parent_canvas = NULL;
		canvases[i]->text = text_conf[j];
		canvases[i]->settings = NULL;
	}
	// Link canvas->canvas
	linkCanvasToCanvas(canvases);

	// Link canvas->Text
	linkCanvasToText(canvases);

	// Link home_canvas to pages
	linkPagesToCanvas(canvases);

	return 0;
}

void linkPagesToCanvas(struct _canvas** canvases){
	int size = sizeof(page_conf)/sizeof(struct CanvasConf_t*);
	Page_t* page;
	SerialPrint("LINK PAGES TO CANVAS\n\n");
	osDelay(1000);

	for(page = screen.home_page; page!= NULL; page = page->next_page){
		for(int i = 0; i<size; i++){
			if((strcmp(canvases[i]->settings->Page_ID, page->Page_ID) == 0) && (canvases[i]->index == 0)){
				page->home_canvas = canvases[i];
			}
		}
	}
}


void linkCanvasToCanvas(struct _canvas** canvases){
	SerialPrint("LINK CANVAS TO CANVAS\n\n");
	osDelay(1000);
	struct _canvas* canvas_i = NULL;
	struct _canvas* canvas_j = NULL;
	struct _canvas* last;
	int index_child;
	int size = sizeof(page_conf)/sizeof(struct CanvasConf_t*) + sizeof(text_conf)/sizeof(struct _textConf_t*);

	for(int i=0; i<size; i++){
		index_child = 1;
		last = NULL;
		canvas_i = canvases[i];
		for(int j=0; j<size; j++){
			if(j==i) continue;

			canvas_j = canvases[j];
			// Search for Child Canvases
			if(canvas_j->settings == NULL || canvas_i->settings == NULL) continue;
			if(strcmp(canvas_j->settings->Parent_ID, canvas_i->settings->Canvas_ID) == 0){

				if(last==NULL){
					canvas_i->child_canvas = canvas_j;
					canvas_j->parent_canvas = canvas_i;
					last = canvas_j;
					last->index = index_child++;
				}
				else{
					canvas_j->parent_canvas = canvas_i;
					last->next_canvas = canvas_j;
					last = canvas_j;
					last->index = index_child++;
				}
			}
		}
	}
}

void linkCanvasToText(Canvas_t** canvases){
	struct _canvas*     canvas;
	struct _textConf_t* text;
	int size = sizeof(text_conf)/sizeof(struct _textConf_t*);
	int size_containter = sizeof(page_conf)/sizeof(struct CanvasConf_t*) + size;

	for(int i=0; i<size_containter; i++){
		static char n[40];
		canvas = canvases[i];
		for(int j=0; j<size; j++){
			text = text_conf[j];
			// Search for Child Canvases
			if(canvas->settings == NULL) continue;
			if(strcmp(text->Canvas_ID, canvas->settings->Canvas_ID) == 0){
				canvas->text = text;
			}

		}
	}
}


int SCREEN_INIT(){
	if(LinkScreenNodes() != 0){
		return -1;
	}
	GUI_VAR_INIT();
	return 0;
}

void GUI_VAR_INIT(){
	struct _Page* current_page;
	struct _canvas* current_canvas;
	CanvasStack stack = {
		.count = 0,
		.head = NULL
	};

	/*Iterate through pages*/
	for(current_page = screen.home_page; current_page != NULL; current_page = current_page->next_page){
		/*DFS through canvas within the page*/
		if(current_page->home_canvas == NULL) continue;

		pushCanvasNode(&stack, current_page->home_canvas);
		while(stack.count != 0){
			current_canvas = popCanvasNode(&stack);
			PROCESS_VAR_PARENT(current_canvas);
			if(current_canvas->next_canvas != NULL) pushCanvasNode(&stack, current_canvas->next_canvas);
			if(current_canvas->child_canvas != NULL) pushCanvasNode(&stack, current_canvas->child_canvas);
		}
	}

	/*Iterate through pages*/
	for(current_page = screen.home_page; current_page != NULL; current_page = current_page->next_page){
		/*DFS through canvas within the page*/
		if(current_page->home_canvas == NULL) continue;

		pushCanvasNode(&stack, current_page->home_canvas);
		while(stack.count != 0){
			current_canvas = popCanvasNode(&stack);
			PROCESS_VAR_SIBLING(current_canvas);

			if(current_canvas->text != NULL){
				current_canvas->text->textStart_x = current_canvas->text->offset_left + current_canvas->settings->canvasStart_x;
				current_canvas->text->textStart_y = current_canvas->text->offset_top + current_canvas->settings->canvasStart_y;
			}
			if(current_canvas->next_canvas != NULL) pushCanvasNode(&stack, current_canvas->next_canvas);
			if(current_canvas->child_canvas != NULL) pushCanvasNode(&stack, current_canvas->child_canvas);
		}
	}

}

void PROCESS_VAR_PARENT(struct _canvas* canvas){

	struct _canvas* current;

	for(current = canvas->parent_canvas; current != NULL; current = current->parent_canvas){
		canvas->settings->canvasStart_x += current->settings->offset_left;
		canvas->settings->canvasStart_y += current->settings->offset_top;
	}

	canvas->settings->canvasStart_x += canvas->settings->offset_left;
	canvas->settings->canvasStart_y += canvas->settings->offset_top;

}

void PROCESS_VAR_SIBLING(struct _canvas* canvas){
	if(canvas == NULL) return;

	if(canvas->next_canvas == NULL) return;

	if(canvas->parent_canvas->settings->display != NULL && (strcmp(canvas->parent_canvas->settings->display, "horizontal") == 0)){
		if(canvas->parent_canvas!=NULL) canvas->settings->canvasStart_y = canvas->parent_canvas->settings->canvasStart_y + canvas->settings->offset_top;

		canvas->next_canvas->settings->canvasStart_x =
				canvas->settings->canvasStart_x + canvas->settings->canvasWidth + canvas->next_canvas->settings->offset_left;

		canvas->next_canvas->settings->canvasStart_y = canvas->settings->canvasStart_y + canvas->next_canvas->settings->offset_top;
	}

	else{
		if(canvas->parent_canvas!=NULL) canvas->settings->canvasStart_x = canvas->parent_canvas->settings->canvasStart_x + canvas->settings->offset_left;
		canvas->next_canvas->settings->canvasStart_y =
				canvas->settings->canvasStart_y + canvas->settings->canvasHeight + canvas->next_canvas->settings->offset_top;

		canvas->next_canvas->settings->canvasStart_x = canvas->settings->canvasStart_x + canvas->next_canvas->settings->offset_left;
	}

}

void GUI_HANDLE(){
	struct _Page* current_page;
	struct _canvas* current_canvas;
	CanvasStack stack = {
		.count = 0,
		.head = NULL
	};
	/*Iterate through pages*/
	for(current_page = screen.home_page; current_page != NULL; current_page = current_page->next_page){
		/*DFS through canvas within the page*/
		if(current_page->home_canvas == NULL) continue;

		pushCanvasNode(&stack, current_page->home_canvas);
		while(stack.count != 0 && current_canvas != NULL){
			current_canvas = popCanvasNode(&stack);
			canvasDrawHandle(current_canvas);
			if(current_canvas->next_canvas != NULL) pushCanvasNode(&stack, current_canvas->next_canvas);
			if(current_canvas->child_canvas != NULL) pushCanvasNode(&stack, current_canvas->child_canvas);
		}
	}
}

void canvasDrawHandle(struct _canvas* canvas){

	if(canvas->settings != NULL){
		LCD_Fill(
				canvas->settings->canvasStart_x,
				canvas->settings->canvasStart_y,
				canvas->settings->canvasWidth  + canvas->settings->canvasStart_x,
				canvas->settings->canvasHeight + canvas->settings->canvasStart_y,
				canvas->settings->canvasBackgroundColor);
	}

	if(canvas->text != NULL){
		Show_Str(
				canvas->text->textStart_x,
				canvas->text->textStart_y,
				canvas->text->text_color,
				canvas->settings->canvasBackgroundColor,
				canvas->text->text,
				12,
				1);
	}
}

void pushCanvasNode(CanvasStack* stack, struct _canvas* canvas){
	struct _nodeCanvas* newNode = osMemoryPoolAlloc(NODE_POOLHandle, osWaitForever);
	if(newNode == NULL){
		SerialPrint("Failed To Allocate Node\n");
		return;
	}

	newNode->val = canvas;
	newNode->next = stack->head;
	stack->head = newNode;
	stack->count++;
}

struct _canvas* popCanvasNode(CanvasStack* stack){
	osStatus_t status;
	if(stack->count == 0){
		SerialPrint("Empty Canvas Stack\n");
		return NULL;
	}
	struct _canvas* val;
	struct _nodeCanvas* temp;
	val = stack->head->val;
	temp = stack->head;
	stack->head = stack->head->next;

	status = osMemoryPoolFree(NODE_POOLHandle, temp);
	if(status != osOK){
		SerialPrint("Failed to Free Node -> ");
		print_OS_Status(status);
	}

	stack->count --;
	return val;
}










































