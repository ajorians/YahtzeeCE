#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>

#include "YahtzeeGame.h"

#define FONT_HEIGHT 8

void main(void) {
	/* Seed the random numbers */
	srand(rtc_Time());

	/* Initialize the 8bpp graphics */
	gfx_Begin(gfx_8bpp);

	/* Fill the screen black */
	gfx_FillScreen(gfx_black);

	/* Set the text color */
	gfx_SetTextFGColor(gfx_orange);

	/* Print the message on the screen */
	gfx_PrintStringXY("             +------------+", 0, 0);
	gfx_PrintStringXY("             |  nYahtzee  |", 0, 10);
	gfx_PrintStringXY("             +------------+", 0, 20);

	gfx_PrintStringXY("---------------------------------------", 0, 40);
	gfx_PrintStringXY("Controls:", 0, 50);
	gfx_PrintStringXY("   - Move around    : Arrow keys", 0, 60);
	gfx_PrintStringXY("   - Hold Dice/Release Dice ", 0, 70);
	gfx_PrintStringXY("       or choose score  : 2nd or Enter", 0, 80);

	gfx_PrintStringXY("Game play:", 0, 100);
	gfx_PrintStringXY("Roll various dice up to three times", 0, 110);
	gfx_PrintStringXY("and choose how to score them for the", 0, 120);
	gfx_PrintStringXY("best score!", 0, 130);
	gfx_PrintStringXY("2nd/Enter to Start !", 0, 140);
	gfx_PrintStringXY("                      Or Clear to quit...", 0, 150);

	/* Wait for a key to be pressed */
	while (1)
	{
		sk_key_t key;
		key = os_GetCSC();

		if (key == sk_Enter || key == sk_2nd )
		{
			InitYahtzeeGame();
			Loop();
		}
		else if( key == sk_Clear || key == sk_Power )
		{
			break;
		}
	}

	/* Close the graphics */
	gfx_End();
}
