/*
 * screen.c
 *
 *  Created on: Apr 10, 2013
 *      Author: David Jackson
 *
 *  Purpose: Create a set of functions we can use easily with our screen to interface with the
 *  smart watch (SMWATCH)
 */
#include "driver_config.h"
#include "target_config.h"
#include "uart.h"
#include "screen.h"
#include "string.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];	//This may need to be larger

void initScreen()
{
	clearScreen();
	mediaInit();
	diagnosticScreen();
}

void clearScreen()
{
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xD7;
	UARTCount = 2;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0; 	//reset counter, this assumes this is faster than screen can ACK

	wait();	//wait for screen to ACK

	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to clear screen");

	UARTCount = 0;
}

void mediaInit()
{
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xB1;
	UARTCount = 2;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0; 	//reset counter, this assumes this is faster than screen can ACK

	wait();	//wait for screen to ACK

	if (!(UARTBuffer[0] == 0x00 && UARTBuffer[1] == 0x00 && UARTBuffer[2] == 0x04))
		printf("DANGER WILL ROBINSON, Failed to initialize SD Card");

	UARTCount = 0;
}

void wait()
{
	int i, j;
	for (i=1; i < 100; i++)
	{
		for(j=1; j < 100; j++)
		{
			;	//do nothing
		}
	}
}

int gotACK()
{
	if (UARTBuffer[0] == 0x06)
		return 1;
	else
		return 0;
}

void moveCursor(uint32_t x, uint32_t y)
{
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xE4;
	UARTBuffer[2] = x;	//These could be incorrect syntax. Need board to test.
	UARTBuffer[4] = y;
	UARTCount = 6;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0;

	wait();
	if (gotACK() == 0)
			printf("DANGER WILL ROBINSON, Failed to move cursor");

	UARTCount = 0;
}

void writeString(char *str)
{
	UARTBuffer[0] = 0x00;
	UARTBuffer[1] = 0x06;
	UARTBuffer[2] = *str;
	UARTCount = (sizeof(*str) + 2);	//This may also be wrong syntax
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0;
	wait();
	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to write string");
	UARTCount = 0;
}

void drawSquare(int x, int y, int size, int color)
{
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xCE;
	UARTBuffer[2] = x;
	UARTBuffer[4] = y;
	UARTBuffer[6] = x+4;
	UARTBuffer[8] = y + 4;
	UARTBuffer[10] = color;
	UARTCount = 12;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0;
	wait();
	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to draw rectangle");
	UARTCount = 0;
}
//Screen Types
void diagnosticScreen()
{
	//Make text color white
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0xFF;
	UARTBuffer[3] = 0xFF;
	UARTCount = 4;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0; 	//reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to set text color white");
	UARTCount = 0;

	//Make text background color black
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTCount = 4;
	UARTSend( (uint8_t *)UARTBuffer, UARTCount );
	UARTCount = 0; 	//reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to set text background color black");
	UARTCount = 0;

	char temporary[BUFSIZE];
	strcpy(temporary, "SMWATCH PROJECT");
	moveCursor(2,2);
	writeString(temporary);	//Need to check this system
	moveCursor(4,0);
	strcpy(temporary, "David Jackson");
	writeString(temporary);
	moveCursor(5,0);
	strcpy(temporary, "Jon Meed");
	writeString(temporary);
	moveCursor(6,0);
	strcpy(temporary, "Filip Theodorakis");
	writeString(temporary);
	moveCursor(7,0);
	strcpy(temporary, "Tony Lucchesi");
	writeString(temporary);
	}


