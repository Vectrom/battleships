#pragma once

#include "ships.h"
#include "calibrationTp.h"
#include "uart.h"

void putNElementShip(Board *board, int length)
{
	int startX;
	int startY;
	int endX;
	int endY;

	printString("Umiesc statek:", 14, 2);
	drawShip(length);
	do
	{
		do
		{
			tp_wait4touch();
			recalculateCoords();
			saveCurrentCoords(&startX, &startY);
			tp_wait4release();
		} while (correctFieldIndex(&startX, &startY) != 0);

		do
		{
			tp_wait4touch();
			recalculateCoords();
			saveCurrentCoords(&endX, &endY);
			tp_wait4release();
		} while (correctFieldIndex(&endX, &endY) != 0);
	} while (placeNElementShip(board, startX, startY, endX, endY, length) != 0);
	clearInfoPanel(0x82C0);
}

void setupGame(Board *board)
{
	putNElementShip(board, 4);
	for (int i = 0; i < 2; i++)
		putNElementShip(board, 3);

	for (int i = 0; i < 3; i++)
		putNElementShip(board, 2);

	for (int i = 0; i < 4; i++)
	{
		int x;
		int y;

		printString("Umiesc statek:", 14, 2);
		drawShip(1);
		do
		{
			do
			{
				tp_wait4touch();
				recalculateCoords();
				saveCurrentCoords(&x, &y);
				tp_wait4release();
			} while (correctFieldIndex(&x, &y) != 0);
		} while (placeNElementShip(board, x, y, x, y, 1) != 0);
		clearInfoPanel(0x82C0);
	}
}

int checkShoot(Board *enemy_board, int x, int y)
{
	uint8_t ans = 0;
	waitForACK();
	readInfo(&ans);
	if (ans == GOOD_SHOOT)
	{
		enemy_board->fields[x][y].color = LCDRed;
		drawField(&(enemy_board->fields[x][y]));
		return 0;
	}
	else
	{
		enemy_board->fields[x][y].color = LCDGrey;
	}
	return 1;
}

void shoot(Board *enemy_board)
{
	int x;
	int y;
	do
	{
		do
		{
			tp_wait4touch();
			recalculateCoords();
			saveCurrentCoords(&x, &y);
			tp_wait4release();
		} while (correctFieldIndex(&x, &y) != 0);
		sendInfo(CONFIG_READY);
		sendInfo(x + 1);
		sendInfo(y + 1);
	} while (checkShoot(enemy_board, x, y) == 0);
}

void receiveShoot(Board *my_board)
{
	waitForACK();
	uint8_t x;
	uint8_t y;
	readInfo(&x);
	readInfo(&y);
	x -= 1;
	y -= 1;
	sendInfo(CONFIG_READY);
	if (my_board->fields[x][y].color == LCDBlack || my_board->fields[x][y].color == LCDRed)
	{ 
		sendInfo(GOOD_SHOOT);
		my_board->fields[x][y].color = LCDRed;
		drawField(&(my_board->fields[x][y]));
		receiveShoot(my_board);
	}
	else
	{
		sendInfo(BAD_SHOOT);
		my_board->fields[x][y].color = LCDGrey;
		drawField(&(my_board->fields[x][y]));
	}
}
