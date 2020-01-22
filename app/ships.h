#pragma once

#include <stdlib.h>

#include "board.h"

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void placeOneElement(Board *board, int x, int y)
{
	board->fields[x][y].color = LCDBlack;
	drawField(&board->fields[x][y]);
}

int placeNElementShip(Board *board, int startX, int startY, int endX, int endY, int length)
{
	if (!((startX == endX && abs(startY - endY) == length - 1) || (startY == endY && abs(startX - endX) == length - 1)))
	{
		return 1;
	}

	if (startX > endX)
	{
		swap(&startX, &endX);
	}

	if (startY > endY)
	{
		swap(&startY, &endY);
	}

	int flag = 0;
	for (int i = startX - 1; i <= endX + 1; i++)
	{
		for (int j = startY - 1; j <= endY + 1; j++)
		{
			if (i < 0 || i > BOARD_SIZE - 1 || j < 0 || j > BOARD_SIZE - 1)
			{
				continue;
			}
			if (board->fields[i][j].color != LCDBlueSea)
			{
				flag = 1;
				break;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
	if (flag == 0)
	{
		for (int i = startX; i <= endX; i++)
		{
			for (int j = startY; j <= endY; j++)
			{
				placeOneElement(board, i, j);
			}
		}
	}
	return flag;
}

void drawShip(int size)
{
	for (int i = 0; i < size; i++)
	{
		Field temp = createField(20 + i * FIELD_SIZE, 230 + FIELD_SIZE, LCDRed);
		drawField(&temp);
	}
}

int correctFieldIndex(int *x, int *y)
{
	*x -= MARGIN;
	*x /= FIELD_SIZE;
	*y -= MARGIN;
	*y /= FIELD_SIZE;
	if (*x < 0 || *x >= BOARD_SIZE || *y < 0 || *y >= BOARD_SIZE)
	{
		return -1;
	}
	return 0;
}
