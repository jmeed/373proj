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

#include "screen.h"
#include <string.h>
#include "uart.h"
#include <stdio.h>

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZEUART];	//This may need to be larger

int time_image = 0;

void initScreen() {
	setBaudRate();
	UARTInit(UART_BAUD_FAST);
	clearScreen();
	disableScrolling();
	mediaInit();
	welcomeScreen();
}

void disableScrolling() {
	UARTBuffer[0] = 0x00;
	UARTBuffer[1] = 0x0E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x04;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to disable scrolling\n");
	UARTCount = 0;
}

void setBaudRate()
{
	//set to 115200
	UARTBuffer[0] = 0x00;
	UARTBuffer[1] = 0x0B;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x19;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set baud rate\n");
	UARTCount = 0;
}

void clearScreen() {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xD7;
	UARTCount = 2;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK

	wait();	//wait for screen to ACK

	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to clear screen\n");

	UARTCount = 0;
}

void mediaInit() {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xB1;
	UARTCount = 2;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK

	wait();	//wait for screen to ACK

//	if (!(UARTBuffer[0] == 0x00 && UARTBuffer[1] == 0x00
//			&& UARTBuffer[2] == 0x04))
//		printf("DANGER WILL ROBINSON, Failed to initialize SD Card\n");

	UARTCount = 0;
}

void media_setSector(uint16_t hi, uint16_t lo) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xB8;
	UARTBuffer[2] = hi >> 8;
	UARTBuffer[3] = hi;
	UARTBuffer[4] = lo >> 8;
	UARTBuffer[5] = lo;
	UARTCount = 6;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set media sector\n");
	UARTCount = 0;
}

void media_display() {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xB3;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTBuffer[4] = 0x00;
	UARTBuffer[5] = 0x00;
	UARTCount = 6;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (!(UARTBuffer[0] == 0x00 && UARTBuffer[1] == 0x00
			//&& UARTBuffer[2] == 0x04))
		//printf("DANGER WILL ROBINSON, Failed to display media\n");
	UARTCount = 0;
}

void wait() {
	int i, j;
	for (i = 1; i < 1000; i++) {
		for (j = 1; j < 1000; j++) {
			;	//do nothing
		}
	}
}

int gotACK() {
	if (UARTBuffer[0] == 0x06)
		return 1;
	else
		return 0;
}

void moveCursor(uint8_t x, uint8_t y) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xE4;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = x;	//These could be incorrect syntax. Need board to test.
	UARTBuffer[4] = 0x00;
	UARTBuffer[5] = y;
	UARTCount = 6;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0;

	wait();
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to move cursor\n");

	UARTCount = 0;
}

void writeString(char *str) {
	UARTBuffer[0] = 0x00;
	UARTBuffer[1] = 0x06;

	unsigned int i;
	for (i = 0; i < strlen(str); i++) {
		UARTBuffer[2 + i] = str[i];
	}
	UARTBuffer[i + 2] = '\0';
	UARTCount = strlen(str) + 3;	//This may also be wrong syntax
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0;
	wait();
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to write string\n");
	UARTCount = 0;
}

void drawSquare(int x, int y, int size, int color) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xCE;
	UARTBuffer[2] = x;
	UARTBuffer[4] = y;
	UARTBuffer[6] = x + 4;
	UARTBuffer[8] = y + 4;
	UARTBuffer[10] = color;
	UARTCount = 12;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0;
	wait();
	if (gotACK() == 0)
		printf("DANGER WILL ROBINSON, Failed to draw rectangle\n");
	UARTCount = 0;
}
//Screen Types
void welcomeScreen() {
	clearScreen();
	//Make text color white
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0xFF;
	UARTBuffer[3] = 0xFF;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text color white\n");
	UARTCount = 0;

	//Make text background color black
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text background color black\n");
	UARTCount = 0;

	//Background
	media_setSector(0x0000, BLOCKM);	//Block M
	//media_setSector(0x0000, BLACKM); //Black M
	media_display();

	char temporary[BUFSIZEUART];

	strcpy(temporary, "Mwatch\n");
	writeString(temporary);
	moveCursor(2, 2);
//	writeString(temporary);	//Need to check this system
//	moveCursor(4,0);
	strcpy(temporary, "David Jackson\n");
	writeString(temporary);
//	moveCursor(5,0);
	strcpy(temporary, "Jon Meed\n");
	writeString(temporary);
	moveCursor(13, 0);
	strcpy(temporary, "Filip Theodorakis\n");
	writeString(temporary);
	moveCursor(14, 2);
	strcpy(temporary, "Tony Lucchesi\n");
	writeString(temporary);
}

void weatherScreen(enum WEATHER_TYPE w, char * weather_title) {
	clearScreen();
	//Make text color white
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0xFF;
	UARTBuffer[3] = 0xFF;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text color white\n");
	UARTCount = 0;

	//Make text background color black
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text background color black\n");
	UARTCount = 0;

	//Background
	switch (w) //_CLOUD, _RAIN, _SNOW, _STORM, _SUNNY
	{
	case _CLOUD:
		media_setSector(0x0000, CLOUDY);
		break;
	case _RAIN:
		media_setSector(0x0000, RAIN);
		break;
	case _SNOW:
		media_setSector(0x0000, SNOW);
		break;
	case _STORM:
		media_setSector(0x0000, STORM);
		break;
	case _SUNNY:
		media_setSector(0x0000, SUNNY);
		break;
	default:
		//display black screen
		clearScreen();
		break;
	}
	media_display();

	char temporary[BUFSIZEUART];

	strcpy(temporary, weather_title);
	writeString(temporary);
	moveCursor(2, 0);
}

void newsScreen() {
	clearScreen();
	media_setSector(0x0000, NEWS);
	media_display();
	moveCursor(0, 0);

	//Make text color white
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0xFF;
	UARTBuffer[3] = 0xFF;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text color white\n");
	UARTCount = 0;

	//Make text background color black
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text background color black\n");
	UARTCount = 0;

	char temporary[BUFSIZEUART];
	strcpy(temporary, "Headlines\n");
	writeString(temporary);
}
void debugScreen() {
	clearScreen();
	//Make text color green
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0x25;
	UARTBuffer[3] = 0x74;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text color green\n");
	UARTCount = 0;

	//Make text background color black
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7E;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = 0x00;
	UARTCount = 4;
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text background color black\n");
	UARTCount = 0;

	moveCursor(0, 0);
	char temporary[BUFSIZEUART];

	strcpy(temporary, "Debug\n");
	writeString(temporary);
}
void snakeScreen() {
	//Get basic black screen for snake-ing
	clearScreen();
}

void timeScreen() {
	clearScreen();
	switch (time_image) {
	case 0:
		media_setSector(0x0000, TIME1);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xF6;
		UARTBuffer[3] = 0xD6;
		UARTCount = 4;
		UARTSend((uint8_t *) UARTBuffer, UARTCount);
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(2, 5);
		time_image++;
		break;
	case 1:
		media_setSector(0x0000, TIME2);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xB7;
		UARTBuffer[3] = 0x9F;
		UARTCount = 4;
		UARTSend((uint8_t *) UARTBuffer, UARTCount);
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(2, 2);
		time_image++;
		break;
	case 2:
		media_setSector(0x0000, TIME3);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xE6;
		UARTBuffer[3] = 0x1E;
		UARTCount = 4;
		UARTSend((uint8_t *) UARTBuffer, UARTCount);
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(2, 2);
		time_image++;
		break;
	case 3:
		media_setSector(0x0000, TIME4);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xCE;
		UARTBuffer[3] = 0xBC;
		UARTCount = 4;
		UARTSend((uint8_t *) UARTBuffer, UARTCount);
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(2, 2);
		time_image = 0;
		break;
	default:
		clearScreen();
		moveCursor(0, 0);
		break;
	}
	media_display();
}

