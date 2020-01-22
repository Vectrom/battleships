#pragma once

#include "ships.h"
#include "TP_Open1768.h"
volatile int touched = 0;

typedef struct t_Coords
{
	int x;
	int y;
} Coords;

volatile Coords curr_tpad;

void saveCurrentCoords(int *x, int *y)
{
	*x = curr_tpad.x;
	*y = curr_tpad.y;
}

typedef struct t_Matrix
{
	int32_t An, Bn, Cn, Dn, En, Fn, Den;
} Matrix;

volatile Matrix matrix;

void recalculateCoords()
{
	int tempX = curr_tpad.x;
	curr_tpad.x = (curr_tpad.x * matrix.An + curr_tpad.y * matrix.Bn + matrix.Cn) / matrix.Den;
	curr_tpad.y = (tempX * matrix.Dn + curr_tpad.y * matrix.En + matrix.Fn) / matrix.Den;
}

int SetCalMatrix(const Coords *CalCoords, Coords *TPsample,volatile Matrix *MtxPtr)
{
	MtxPtr->Den = ((TPsample[0].x - TPsample[2].x) * (TPsample[1].y - TPsample[2].y)) - ((TPsample[1].x - TPsample[2].x) * (TPsample[0].y - TPsample[2].y));
	if (MtxPtr->Den == 0)
		return 0;
	MtxPtr->An = ((CalCoords[0].x - CalCoords[2].x) * (TPsample[1].y - TPsample[2].y)) -
				 ((CalCoords[1].x - CalCoords[2].x) * (TPsample[0].y - TPsample[2].y));
	MtxPtr->Bn = ((TPsample[0].x - TPsample[2].x) * (CalCoords[1].x - CalCoords[2].x)) -
				 ((CalCoords[0].x - CalCoords[2].x) * (TPsample[1].x - TPsample[2].x));
	MtxPtr->Cn = (TPsample[2].x * CalCoords[1].x - TPsample[1].x * CalCoords[2].x) * TPsample[0].y +
				 (TPsample[0].x * CalCoords[2].x - TPsample[2].x * CalCoords[0].x) * TPsample[1].y +
				 (TPsample[1].x * CalCoords[0].x - TPsample[0].x * CalCoords[1].x) * TPsample[2].y;
	MtxPtr->Dn = ((CalCoords[0].y - CalCoords[2].y) * (TPsample[1].y - TPsample[2].y)) -
				 ((CalCoords[1].y - CalCoords[2].y) * (TPsample[0].y - TPsample[2].y));
	MtxPtr->En = ((TPsample[0].x - TPsample[2].x) * (CalCoords[1].y - CalCoords[2].y)) -
				 ((CalCoords[0].y - CalCoords[2].y) * (TPsample[1].x - TPsample[2].x));
	MtxPtr->Fn = (TPsample[2].x * CalCoords[1].y - TPsample[1].x * CalCoords[2].y) * TPsample[0].y +
				 (TPsample[0].x * CalCoords[2].y - TPsample[2].x * CalCoords[0].y) * TPsample[1].y +
				 (TPsample[1].x * CalCoords[0].y - TPsample[0].x * CalCoords[1].y) * TPsample[2].y;
	return 1;
}

void EINT3_IRQHandler()
{
	touched = !touched;

	int count = 5;
	int sumX = 0;
	int sumY = 0;
	int x = 0;
	int y = 0;
	int i = 0;
	while (i < count)
	{
		touchpanelGetXY(&y, &x);
		if (x == 0xFFF)
			continue;
		sumX += x;
		sumY += y;
		i++;
	}
	curr_tpad.y = sumX / count;
	curr_tpad.x = sumY / count;

	LPC_GPIOINT->IO0IntClr = 1 << 19;
}

void tp_wait4touch(void)
{
	while (touched == 0)
		;
}

void tp_wait4release(void)
{
	while (touched == 1)
		;
}

static void DrawCross(uint16_t Xpos, uint16_t Ypos)
{
	drawLine(Xpos - 15, Ypos, Xpos - 2, Ypos, 0xffff);
	drawLine(Xpos + 2, Ypos, Xpos + 15, Ypos, 0xffff);
	drawLine(Xpos, Ypos - 15, Xpos, Ypos - 2, 0xffff);
	drawLine(Xpos, Ypos + 2, Xpos, Ypos + 15, 0xffff);
}

#define CM_OFF 45
static const Coords CalPoints[3] = {
	{CM_OFF, CM_OFF},
	{CM_OFF, LCD_MAX_Y - CM_OFF},
	{LCD_MAX_X - CM_OFF, LCD_MAX_Y - CM_OFF}};

void tp_calib(void)
{
	Coords TPsamp[3];

	do
	{
		for (uint32_t i = 0; i < 3; i++)
		{
			backGround(LCDRed);
			DrawCross(CalPoints[i].x, CalPoints[i].y);
			tp_wait4touch();
			TPsamp[i] = curr_tpad;
			tp_wait4release();
		}
	} while (!SetCalMatrix(CalPoints, TPsamp, &matrix));
}

void touch()
{
	tp_wait4touch();
	recalculateCoords();
	tp_wait4release();
}
