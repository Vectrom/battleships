#pragma once

#include "field.h"

#define BOARD_SIZE 10
#define MARGIN 20

typedef struct t_Board
{
	Field fields[BOARD_SIZE][BOARD_SIZE];
} Board;

Board createBoard()
{
	Board b;

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			b.fields[i][j] = createField(MARGIN + i * FIELD_SIZE, MARGIN + j * FIELD_SIZE, LCDBlueSea);
		}
	}

	return b;
}

void drawBoard(const Board *board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			drawField(&(board->fields[i][j]));
		}
	}
}

int win(const Board* b) 
{
	int count = 4 * 1 + 3 * 2 + 2 * 3 + 1 * 4;
		for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			count -= b->fields[i][j].color == LCDRed;
		}
	}
	
	return count;
}

int lose(const Board* b){
	int count = 0;
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			count += b->fields[i][j].color == LCDBlack;
		}
	}
	
	return count;
}
