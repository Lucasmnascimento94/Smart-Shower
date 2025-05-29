
#include "main.h"
#include "FONT.h"
#ifndef __GUI_H__
#define __GUI_H__


void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void Draw_Circle(u16 x0,u16 y0,u16 fc,u8 r);
void Draw_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void Fill_Triangel(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len,u8 size,u8 mode);
void GUI_DrawFont16(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont24(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void GUI_DrawFont32(u16 x, u16 y, u16 fc, u16 bc, u8 *s,u8 mode);
void Show_Str(u16 x, u16 y, u16 fc, u16 bc, char *str,u8 size,u8 mode);
void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p); //��ʾ40*40 QQͼƬ
void gui_circle(int xc, int yc,u16 c,int r, int fill);
void Gui_StrCenter(u16 x, u16 y, u16 fc, u16 bc, char *str,u8 size,u8 mode);
void LCD_DrawFillRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawLogoPage(void);
void LCD_DrawPage1(void);
void LCD_DrawKeyPad(void);
void LCD_DrawPage2(void);
void LCD_DrawPage3(void);
void LCD_DrawIntro(void);
void LCD_UpdateTempSet(float temp);
void LCD_UpdateColdWater(float temp);
void LCD_UpdateHotWater(float temp);
void LCD_UpdateTerminalWater(float temp);
void LCD_UpdateOpening(int steps);
void LCD_ClearWindow(u16 xs, u16 xe, u16 ys, u16 ye, u16 bc);
void LCD_UpdateTypedTemp(uint32_t frameBuffer, char *c);


void LCD_ShowString(uint32_t frameBuffer, u32 x,u32 y,u8 displaySize,char *p,u16 color, u16 color_back, bool background);
void LCD_ShowChar(uint32_t frameBuffer, u32 x,u32 y, u8 num,u8 size, u16 color);
void GUI_DrawPoint(uint32_t frameBuffer, u32 x,u32 y,u16 color);
void LCD_Fill(uint32_t frameBuffer, u32 sx,u32 sy,u32 ex,u32 ey,u16 color);
void keypad(uint32_t frameBuffer);
void page_1(uint32_t buffer);
void draw_terminal(uint32_t frameBuffer);
void draw_var(uint32_t frameBuffer);
void printf_(uint32_t frameBuffer, char *msg);
void printf_g(uint32_t frameBuffer, int x, int y);
void scan_g(uint32_t frameBuffer);
void addLine();
void load_background_image_to_sdram(void);
void drawLine(uint32_t frameBuffer, int x0, int y0, int x1, int y1, uint32_t color);
void drawThickLine(uint32_t frameBuffer, int x0, int y0, int x1, int y1, uint32_t color, int thickness);
void draw_circle(uint32_t frameBuffer,int cx, int cy, int radius, uint32_t color);
void draw_circle_filled(uint32_t frameBuffer, int cx, int cy, int radius, uint32_t color);
#endif





















