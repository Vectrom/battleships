#include "ships.h"
#include "calibrationTp.h"
#include "LCD_Utils.h"
#include "game.h"
#include "uart.h"

volatile int player1 = 0;



Board b, enemyB;

int main(void)
{
	LCDconfig();
	UARTconfig();

	LPC_GPIOINT->IO0IntEnF = 1 << 19;
	NVIC_EnableIRQ(EINT3_IRQn);

	tp_calib();
	backGround(0x82C0);
	printString("Umiesc statek:", 14, 2);
	b = createBoard();
	drawBoard(&b);
	setupGame(&b);

	printString("Dotknij, aby kontynuowac...", 14, 2);
	tp_wait4touch();
	tp_wait4release();

	enemyB = createBoard();

	uint8_t temp = 0;
	temp = LPC_UART0->RBR;
	if (temp == CONFIG_READY)
	{
		player1 = 0;
	}
	else
	{
		player1 = 1;
	}

	clearInfoPanel(0x82C0);
	if (player1)
	{
		sendInfo(CONFIG_READY);
		printString("Czekam na przeciwnika...", 14, 2);
		waitForACK();
		clearInfoPanel(0x82C0);
		printString("Strzelaj!", 14, 2);
	}
	else
	{
		sendInfo(CONFIG_READY);
	}
	if (player1)
	{
		drawBoard(&enemyB);
	}

	while (1)
	{
		if (player1)
		{
			printString("Strzelaj!", 14, 2);
			shoot(&enemyB);
			if(win(&enemyB) == 0) {
				clearInfoPanel(0x82C0);
				printString("Wygrales!", 17, 9);
				break;
			} else {
				drawBoard(&b);
				clearInfoPanel(0x82C0);
			}
			
		}
		else
		{
			printString("Uwazaj, jestes pod ostrzalem!", 14, 0);
			receiveShoot(&b);
			if(lose(&b) == 0) {
				clearInfoPanel(0x82C0);
				printString("Przegrales!", 17, 9);
				break;
			} else {
				drawBoard(&enemyB);
				clearInfoPanel(0x82C0);
			}
			
		}
		player1 = !player1;
	}
	
	while(1);
}
