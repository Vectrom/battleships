#pragma once

#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "asciiLib.h"
#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "TP_Open1768.h"

void LCDconfig()
{
	lcdConfiguration();
	init_ILI9325();
	touchpanelInit();
}

void drawRectangle(int x, int y, int width, int height, uint16_t col)
{
    lcdWriteReg(HADRPOS_RAM_START, x);
    lcdWriteReg(HADRPOS_RAM_END, x + width);
    lcdWriteReg(VADRPOS_RAM_START, y);
    lcdWriteReg(VADRPOS_RAM_END, y + height);
    lcdWriteReg(ADRX_RAM, x + 10);
    lcdWriteReg(ADRY_RAM, y + 10);
    lcdWriteReg(DATA_RAM, col);
    for (int i = 0; i <= width; i++)
    {
        for (int j = 0; j <= height; j++)
        {
            lcdWriteData(col);
        }
    }
}

void backGround(uint16_t col)
{
    drawRectangle(0, 0, LCD_MAX_X, LCD_MAX_Y, col);
}

void clearInfoPanel(uint16_t col)
{
    drawRectangle(0, 220, LCD_MAX_X, LCD_MAX_Y - 220, col);
}

void drawLine(const int x1, const int y1, const int x2, const int y2, uint16_t col)
{
    int d, dx, dy, ai, bi, xi, yi;
    int x = x1, y = y1;
    if (x1 < x2)
    {
        xi = 1;
        dx = x2 - x1;
    }
    else
    {
        xi = -1;
        dx = x1 - x2;
    }
    if (y1 < y2)
    {
        yi = 1;
        dy = y2 - y1;
    }
    else
    {
        yi = -1;
        dy = y1 - y2;
    }
    lcdWriteReg(ADRX_RAM, x);
    lcdWriteReg(ADRY_RAM, y);
    lcdWriteReg(DATA_RAM, col);

    if (dx > dy)
    {
        ai = (dy - dx) * 2;
        bi = dy * 2;
        d = bi - dx;
        while (x != x2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                x += xi;
            }
            lcdWriteReg(ADRX_RAM, x);
            lcdWriteReg(ADRY_RAM, y);
            lcdWriteReg(DATA_RAM, col);
        }
    }
    else
    {
        ai = (dx - dy) * 2;
        bi = dx * 2;
        d = bi - dy;
        while (y != y2)
        {
            if (d >= 0)
            {
                x += xi;
                y += yi;
                d += ai;
            }
            else
            {
                d += bi;
                y += yi;
            }
            lcdWriteReg(ADRX_RAM, x);
            lcdWriteReg(ADRY_RAM, y);
            lcdWriteReg(DATA_RAM, col);
        }
    }
}

void printString(const char *str, uint16_t x_pos, uint16_t y_pos)
{
    unsigned char buff[16];
    int line = x_pos;
    int column = y_pos;
    for (int i = 0; i < strlen(str); i++, column++)
    {
        char c = str[i];
        GetASCIICode(1, buff, c);
        if (c == '\n')
        {
            line += 1;
            column = -1;
            continue;
        }
        for (int y = line * 16; y < (line + 1) * 16; y++)
        {
            unsigned char temp = buff[y % 16];
            for (int x = column * 8; x < (column + 1) * 8; x++)
            {
                lcdWriteReg(ADRX_RAM, x);
                lcdWriteReg(ADRY_RAM, y);
                if (temp & (1 << (8 - (x % 8))))
                {
                    lcdWriteReg(DATA_RAM, LCDBlack);
                }
                else
                {
                    //lcdWriteReg(DATA_RAM, LCDBlue);
                }
            }
        }
    }
}
