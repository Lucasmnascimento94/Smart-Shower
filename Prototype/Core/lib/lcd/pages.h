#ifndef PAGES_H
#define PAGES_H

#include "types.h"
#include "GUI.h"

typedef struct {
	/*Statistic display graph panel*/
	uint32_t statisticPanel_x;
	uint32_t statisticPanel_xe;
	uint32_t statisticPanel_y;
	uint32_t statisticPanel_ye;
	uint16_t statisticalPanel_backgroundColor;
	uint16_t statisticalPanel_hotTempColor;
	uint16_t statisticalPanel_outTempColor;
	uint16_t statisticalPanel_wordColor;

	/*Logo Positioning*/
	uint32_t LOGO_x;
	uint32_t LOGO_y;
	uint16_t LOGO_wordColor;

	/*Set Temperature Box panel*/
	uint32_t setBox_x;
	uint32_t setBox_xe;
	uint32_t setBox_y;
	uint32_t setBox_ye;
	uint16_t setBox_backgroundColor;
	uint16_t setBox_wordColor;

	/*Arrow Left*/
	uint32_t arrowLeft_x;
	uint32_t arrowLeft_xe;
	uint32_t arrowLeft_y;
	uint32_t arrowLeft_ye;
	uint16_t arrow_backgroundColor;
	uint16_t arrow_arrowColor;

	/*Arrow Right*/
	uint32_t arrowRight_x;
	uint32_t arrowRight_xe;
	uint32_t arrowRight_y;
	uint32_t arrowRight_ye;

	/*Home Box*/
	uint32_t homeBox_x;
	uint32_t homeBox_xe;
	uint32_t homeBox_y;
	uint32_t homeBox_ye;
	uint16_t homeBox_backgroundColor;
	uint16_t homeBox_wordColor;

	/*Conf Box*/
	uint32_t confBox_x;
	uint32_t confBox_xe;
	uint32_t confBox_y;
	uint32_t confBox_ye;
	uint16_t confBox_backgroundColor;
	uint16_t confBox_wordColor;

	/*Set Temperature Box panel*/
	uint32_t setTemperature_x;
	uint32_t setTemperature_xe;
	uint32_t setTemperature_y;
	uint32_t setTemperature_ye;
	uint16_t setTemperature_backgroundColor;
	uint16_t setTemperature_wordColor;

	/*current Temperature Box panel*/
	uint32_t outTemperature_x;
	uint32_t outTemperature_xe;
	uint32_t outTemperature_y;
	uint32_t outTemperature_ye;
	uint16_t outTemperature_backgroundColor;
	uint16_t outTemperature_wordColor;

	/*Hot Temperature Box panel*/
	uint32_t hotTemperature_x;
	uint32_t hotTemperature_xe;
	uint32_t hotTemperature_y;
	uint32_t hotTemperature_ye;
	uint16_t hotTemperature_backgroundColor;
	uint16_t hotTemperature_wordColor;

}PAGE_HOME;

void PAGE_HOME_INIT_VAR(void);
void PAGE_HOME_Draw(uint32_t frameBuffer);
void PAGE_CONF_Draw(uint32_t frameBuffer);

void draw_statistic_panel(uint32_t frameBuffer);
void plot_statistic_panel(uint32_t frameBuffer);

void draw_box_1(uint32_t frameBuffer);
void update_setBox(uint32_t frameBuffer, int newTemp);
void update_homeBox(uint32_t frameBuffer);
void update_confBox(uint32_t frameBuffer);

void draw_box_2(uint32_t frameBuffer);
void update_setTemp(uint32_t frameBuffer, char *temp);
void update_outTemp(uint32_t frameBuffer, char *temp);
void update_hotTemp(uint32_t frameBuffer, char *temp);

void draw_arrow_left(uint32_t frameBuffer, int thickness);
void animate_arrow_left(uint32_t frameBuffer, int thickness);

void draw_arrow_right(uint32_t frameBuffer, int thickness);
void animate_arrow_right(uint32_t frameBuffer, int thickness);
#endif
















