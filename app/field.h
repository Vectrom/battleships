#pragma once

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "LCD_Utils.h"

#define FIELD_SIZE 20

typedef struct t_Field
{
	uint16_t x;
	uint16_t y;
	uint16_t color;
} Field;

Field createField(uint16_t x, uint16_t y, uint16_t color)
{
	Field f;
	f.x = x;
	f.y = y;
	f.color = color;
	return f;
}

void drawField(const Field *field)
{
	drawRectangle(field->x, field->y, FIELD_SIZE, FIELD_SIZE, field->color);
	drawLine(field->x, field->y, field->x + FIELD_SIZE, field->y, LCDGrey);							  // |
	drawLine(field->x, field->y, field->x, field->y + FIELD_SIZE, LCDGrey);							  // -
	drawLine(field->x + FIELD_SIZE, field->y, field->x + FIELD_SIZE, field->y + FIELD_SIZE, LCDGrey); // _
	drawLine(field->x, field->y + FIELD_SIZE, field->x + FIELD_SIZE, field->y + FIELD_SIZE, LCDGrey); // |
}
