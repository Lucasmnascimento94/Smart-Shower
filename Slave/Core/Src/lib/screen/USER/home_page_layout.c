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
	.view = NULL
};
View_t view = {
	.count = 0,
	.home_canvas = NULL,
	.last_canvas = NULL
};

/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	 		BACKGROUND	                                            #
#########################################################################################################
#########################################################################################################
*/


const struct CanvasConf_t home_canvas_conf = {
	/*Location*/
	.Canvas_ID = "home_canvas",
	.Parent_ID = NULL,
	.Page_ID = "home_page",
	.index = 0,

	/*Color*/
	.canvasBackgroundColor = BLACK,

	/*Dimensions*/
	.canvasHeight = screenHeight,
	.canvasWidth = screenWidth,
	.canvasStart_x = DEFAULT,
	.canvasStart_y = DEFAULT,

	/*Effects*/
	.effects = false

};


/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	 	 	 		HEADDER		                                            #
#########################################################################################################
#########################################################################################################
*/
const struct CanvasConf_t header_container_conf = {
	/*Location*/
	.Canvas_ID = "header_container",
	.Parent_ID = "home_canvas",
	.Page_ID = "home_page",
	.index = 1,

	/*Color*/
	.canvasBackgroundColor = BLACK,

	/*Display*/
	.offset_center_x = true,
	.offset_top = 5,
	.offset_left = 0,
	.offset_right = 0,
	.offset_bottom = 0,

	/*Dimensions*/
	.canvasHeight = 15,
	.canvasWidth = screenWidth,
	.canvasStart_x = DEFAULT,
	.canvasStart_y = DEFAULT,

	/*Effects*/
	.effects = false
};
		const struct _textConf_t header_text_conf = {
			/*Location*/
			.Canvas_ID = "header_text",
			.Parent_ID = "header_container",
			.Page_ID = "home_page",
			.index = 1,



			/*Display*/
			.data_center = true,
			.data_top = false,
			.data_bottom = false,
			.data_left = false,
			.data_right = false,

			/*Color*/
			.text_color = WHITE,

			/*Font*/
			.font_size = 12,

			/*Effects*/
			.effects = true,
		};

/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	      		STATUS CONTAINER		                                        #
#########################################################################################################
#########################################################################################################
*/
const struct CanvasConf_t status_container_conf = {
	/*Location*/
	.Canvas_ID = "status_container",
	.Parent_ID = "home_canvas",
	.Page_ID = "home_page",
	.index = 2,

	/*Color*/
	.canvasBackgroundColor = GRAY,

	/*Display*/
	.offset_center_x = true,
	.offset_top = 30,
	.offset_left = 0,
	.offset_right = 0,
	.offset_bottom = 0,

	/*Dimensions*/
	.canvasHeight = 30,
	.canvasWidth = screenWidth - 20,
	.canvasStart_x = DEFAULT,
	.canvasStart_y = DEFAULT,

	/*Effects*/
	.effects = false
};

		/*******************************
		 * POWER BUTTON CONFIGURATION. *
		 * ****************************/
		const struct CanvasConf_t power_button_container_conf = {
			/*Location*/
			.Canvas_ID = "power_button_container",
			.Parent_ID = "status_container",
			.Page_ID = "home_page",
			.index = 3,

			/*Color*/
			.canvasBackgroundColor = GREEN,

			/*Display*/
			.offset_center_x = true,
			.offset_top = 5,
			.offset_left = 0,
			.offset_right = 0,
			.offset_bottom = 0,

			/*Dimensions*/
			.canvasHeight = 10,
			.canvasWidth = 15,
			.canvasStart_x = DEFAULT,
			.canvasStart_y = DEFAULT,

			/*Effects*/
			.effects = false
		};

				const struct _textConf_t power_button_text_conf = {
					/*Location*/
					.Canvas_ID = "power_button_text",
					.Parent_ID = "power_button_container",
					.Page_ID = "home_page",
					.index = 1,

					/*Display*/
					.data_center = true,
					.data_top = false,
					.data_bottom = false,
					.data_left = false,
					.data_right = false,

					/*Color*/
					.text_color = WHITE,

					/*Font*/
					.font_size = 12,

					/*Effects*/
					.effects = true,
				};


		/*****************************
		 * Final Temperature Status. *
		 * **************************/
		const struct CanvasConf_t temperature_status_container_conf = {
			/*Location*/
			.Canvas_ID = "temperature_status_container",
			.Parent_ID = "status_container",
			.Page_ID = "home_page",
			.index = 3,

			/*Color*/
			.canvasBackgroundColor = BROWN,

			/*Dimensions*/
			.canvasHeight = 10,
			.canvasWidth = 15,
			.canvasStart_x = 10,
			.canvasStart_y = 10,

			/*Effects*/
			.effects = false
		};

				const struct _textConf_t temperature_status_text_conf = {
					/*Location*/
					.Canvas_ID = "temperature_status_text",
					.Parent_ID = "temperature_status_container",
					.Page_ID = "home_page",
					.index = 1,

					/*Display*/
					.data_center = true,
					.data_top = false,
					.data_bottom = false,
					.data_left = false,
					.data_right = false,

					/*Color*/
					.text_color = WHITE,

					/*Font*/
					.font_size = 12,


					/*Effects*/
					.effects = true,
				};


		/***************************
		 * WiFi Connection Status. *
		 * ************************/
		const struct CanvasConf_t wifi_status_container_conf = {
			/*Location*/
			.Canvas_ID = "wifi_status_container",
			.Parent_ID = "status_container",
			.Page_ID = "home_page",
			.index = 3,

			/*Color*/
			.canvasBackgroundColor = BROWN,

			/*Dimensions*/
			.canvasHeight = screenHeight,
			.canvasWidth = screenWidth,
			.canvasStart_x = 0,
			.canvasStart_y = 0,



			/*Effects*/
			.effects = false
		};

				const struct _textConf_t wifi_status_text_conf = {
					/*Location*/
					.Canvas_ID = "wifi_status_text",
					.Parent_ID = "wifi_status_container",
					.Page_ID = "home_page",
					.index = 1,

					/*Display*/
					.data_center = true,
					.data_top = false,
					.data_bottom = false,
					.data_left = false,
					.data_right = false,

					/*Color*/
					.text_color = WHITE,

					/*Font*/
					.font_size = 12,


					/*Effects*/
					.effects = true,
				};


		/********************
		 * New Page Button. *
		 * *****************/
		const struct CanvasConf_t next_page_container_conf = {
			/*Location*/
			.Canvas_ID = "next_page_container",
			.Parent_ID = "status_container",
			.Page_ID = "home_page",
			.index = 3,

			/*Color*/
			.canvasBackgroundColor = BROWN,

			/*Dimensions*/
			.canvasHeight = screenHeight,
			.canvasWidth = screenWidth,
			.canvasStart_x = 0,
			.canvasStart_y = 0,


			/*Effects*/
			.effects = false
		};

				const struct _textConf_t next_page_text_conf = {
					/*Location*/
					.Canvas_ID = "next_page_text",
					.Parent_ID = "next_page_container",
					.Page_ID = "home_page",
					.index = 1,

					/*Display*/
					.data_center = true,
					.data_top = false,
					.data_bottom = false,
					.data_left = false,
					.data_right = false,
					/*Color*/
					.text_color = WHITE,

					/*Font*/
					.font_size = 12,

					/*Effects*/
					.effects = true,
				};


/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	OUTPUT TEMPERATURE CONTAINER                                            #
#########################################################################################################
#########################################################################################################
*/



/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	   TARGET TEMPERATURE CONTAINER                                             #
#########################################################################################################
#########################################################################################################
*/





/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	   HOT TEMPERATURE CONTAINER                                            #
#########################################################################################################
#########################################################################################################
*/




/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	     VALE POSITION CONTAINER                                            #
#########################################################################################################
#########################################################################################################
*/



/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	EXPECTED VALVE POSITION CONTAINER                                       #
#########################################################################################################
#########################################################################################################
*/


/*#######################################################################################################
#########################################################################################################
# 	 	 	 	 	 	 	 	   TEMPERATURE CONTROL CONTAINER                                        #
#########################################################################################################
#########################################################################################################
*/




const struct CanvasConf_t* page_conf[] = {
		&home_canvas_conf,
		&header_container_conf,
		&status_container_conf,
		&power_button_container_conf,
		&temperature_status_container_conf,
		&wifi_status_container_conf,
		&next_page_container_conf,
};

const struct _textConf_t* text_conf[] = {
		&header_text_conf,
		&power_button_text_conf,
		&temperature_status_text_conf,
		&wifi_status_text_conf,
		&next_page_text_conf,
};








Screen_t* LinkScreenNodes(){

	for(int i=0; i< pageIndex.pageCount; i++){
		Page_t*  page = osMemoryPoolAlloc(PAGE_POOLHandle, osWaitForever);
		View_t*  view = osMemoryPoolAlloc(VIEW_POOLHandle, osWaitForever);
		if(page == NULL || view == NULL){
			SerialPrint("Failed to Allocate Page\n");
			return NULL;
		}

		page->view = view;
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
	}

	return &screen;
}





Page_t* LinkPages(Screen_t* screen){

}


Page_t* PAGE_INIT(Screen_t *screen, char* ID){
	Page_t * page = osMemoryPoolAlloc(PAGE_POOLHandle, osWaitForever);
	View_t*  view = osMemoryPoolAlloc(VIEW_POOLHandle, osWaitForever);

	if(page == NULL || view == NULL){
		SerialPrin("Failed to Allocate Page\n");
		return NULL;
	}

	page->Page_ID = ID;
	page->view = view;
	page->next_page = NULL;

	if(screen->count == 0){
			screen->home_page = page;
			screen->last_page = page;
	}
	else{
			screen->last_page->next_page = page;
			screen->last_page = page;
	}
	screen->count++;
	return page;
}


void ADD_CANVAS(){
	Canvas_t *canvas = osMemoryPoolAlloc(CANVAS_POOLHandle, osWaitForever);
	if(canvas == NULL){
		SerialPrint("FAILED TO ALLOCATE MEMORY");
	}

	canvas->settings = settings;

	if(view->count == 0){
		view->home_canvas = canvas;
		view->last_canvas = canvas;
	}
	else{
		view->last_canvas->next_canvas = canvas;
		view->last_canvas = canvas;
	}
	view->count++;
}

void ADD_TEXT(){

}
















































