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
#include <assert.h>
#include "timer32.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZEUART];	//This may need to be larger

int time_image = 0;
int music_image = 0;

static void send();

static void send() {
	UARTSend((uint8_t *) UARTBuffer, UARTCount);
}

void initScreen() {
	//setBaudRate();
//	UARTInit(UART_BAUD_FAST);
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
	send();
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
	send();
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	//wait();	//wait for screen to ACK
	int i, j;
	for (i = 1; i < 1000; i++) {
		for (j = 1; j < 1000; j++) {
			;	//do nothing
		}
	}
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set baud rate\n");
	UARTCount = 0;
}

void clearScreen() {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xD7;
	UARTCount = 2;
	send();
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
	send();
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
	send();
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
	send();
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (!(UARTBuffer[0] == 0x00 && UARTBuffer[1] == 0x00
			//&& UARTBuffer[2] == 0x04))
		//printf("DANGER WILL ROBINSON, Failed to display media\n");
	UARTCount = 0;
}

void wait() {
	uint64_t start = unixtime;
	while (UARTCount == 0 && unixtime - start < SCREEN_TIMEOUT);// Wait for an interupt

	if(unixtime - start >= SCREEN_TIMEOUT) {
//		printf("Didn't get screen ACK. Screen must have frozen. Restart the screen!\n");
		assert(0);
	}

	UARTCount = 0;
}

int gotACK() {
	if (UARTBuffer[0] == 0x06)
		return 1;
	else
		return 0;
}

void textSize(uint16_t width, uint16_t height) {
	//set text width
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7C;
	UARTBuffer[2] = width >> 8;
	UARTBuffer[3] = width;
	UARTCount = 4;
	send();
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text width\n");
	UARTCount = 0;

	//set text height
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7B;
	UARTBuffer[2] = height >> 8;
	UARTBuffer[3] = height;
	UARTCount = 4;
	send();
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to set text height\n");
	UARTCount = 0;
}

void moveCursor(uint8_t x, uint8_t y) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xE4;
	UARTBuffer[2] = 0x00;
	UARTBuffer[3] = x;	//These could be incorrect syntax. Need board to test.
	UARTBuffer[4] = 0x00;
	UARTBuffer[5] = y;
	UARTCount = 6;
	send();
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
	send();
	UARTCount = 0;
	wait();
	//if (gotACK() == 0)
		//printf("DANGER WILL ROBINSON, Failed to write string\n");
	UARTCount = 0;
}

void drawSquare(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xCF;
	UARTBuffer[2] = x1 >> 8;
	UARTBuffer[3] = x1;
	UARTBuffer[4] = y1 >> 8;
	UARTBuffer[5] = y1;
	UARTBuffer[6] = x2 >> 8;
	UARTBuffer[7] = x2;
	UARTBuffer[8] = y2 >> 8;
	UARTBuffer[9] = y2;
	UARTBuffer[10] = color >> 8;
	UARTBuffer[11] = color;
	UARTCount = 12;
	send();
	UARTCount = 0;
	wait();
//	if (gotACK() == 0)
//		printf("DANGER WILL ROBINSON, Failed to draw rectangle\n");
	UARTCount = 0;
}


void drawFillSquare(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xCE;
	UARTBuffer[2] = x1 >> 8;
	UARTBuffer[3] = x1;
	UARTBuffer[4] = y1 >> 8;
	UARTBuffer[5] = y1;
	UARTBuffer[6] = x2 >> 8;
	UARTBuffer[7] = x2;
	UARTBuffer[8] = y2 >> 8;
	UARTBuffer[9] = y2;
	UARTBuffer[10] = color >> 8;
	UARTBuffer[11] = color;
	UARTCount = 12;
	send();
	UARTCount = 0;
	wait();
//	if (gotACK() == 0)
//		printf("DANGER WILL ROBINSON, Failed to draw rectangle\n");
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
	send();
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
	send();
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
	volatile int wait_c = 4800000;
	int i = 0;
	for (wait_c; wait_c >= 0; wait_c--) {
		i++;
	}
}

void weatherScreen(enum WEATHER_TYPE w, char * weather_title) {
	clearScreen();
	//Make text color white
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0x7F;
	UARTBuffer[2] = 0xFF;
	UARTBuffer[3] = 0xFF;
	UARTCount = 4;
	send();
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
	send();
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
	send();
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
	send();
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
	send();
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
	send();
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
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;
		moveCursor(0, 0);
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
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(0, 2);
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
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(0, 2);
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
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(0, 2);
		time_image = 0;
		break;
	default:
		clearScreen();
		moveCursor(0, 0);
		break;
	}
	media_display();
}

void musicScreen() {
	clearScreen();

	switch (music_image) {
	case 0:
		media_setSector(0x0000, MUSIC1);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xF6;
		UARTBuffer[3] = 0xD6;
		UARTCount = 4;
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;
		moveCursor(0, 0);
		music_image++;
		break;
	case 1:
		media_setSector(0x0000, MUSIC2);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xB7;
		UARTBuffer[3] = 0x9F;
		UARTCount = 4;
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(0, 2);
		music_image++;
		break;
	case 2:
		media_setSector(0x0000, MUSIC3);
		//Set text Color
		UARTBuffer[0] = 0xFF;
		UARTBuffer[1] = 0x7F;
		UARTBuffer[2] = 0xE6;
		UARTBuffer[3] = 0x1E;
		UARTCount = 4;
		send();
		UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
		wait();	//wait for screen to ACK
		//if (gotACK() == 0)
			//printf("DANGER WILL ROBINSON, Failed to set text color\n");
		UARTCount = 0;

		moveCursor(0, 2);
		music_image = 0;
		break;
	default:
		clearScreen();
		moveCursor(0, 0);
		break;
	}
	media_display();
}

uint16_t get_pixel(uint16_t x, uint16_t y) {
	//set text width
	UARTBuffer[0] = 0xFF;
	UARTBuffer[1] = 0xCA;
	UARTBuffer[2] = x >> 8;
	UARTBuffer[3] = x;
	UARTBuffer[4] = y >> 8;
	UARTBuffer[5] = y;
	UARTCount = 6;
	send();
	UARTCount = 0; //reset counter, this assumes this is faster than screen can ACK
	wait();	//wait for screen to ACK
	UARTCount = 0;
	return ((UARTBuffer[1] << 8) + UARTBuffer[2]);
}

