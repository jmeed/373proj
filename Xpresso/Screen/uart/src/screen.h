/*
 * screen.h
 *
 *  Created on: Apr 10, 2013
 *      Author: David Jackson
 *
 *	Purpose: Create a set of functions we can use easily with our screen to interface with the
 *	smart watch (SMWATCH)
 */

#ifndef SCREEN_H_
#define SCREEN_H_

#include "driver_config.h"
#include "target_config.h"
#include "uart.h"

//Screen image #defines. Set as y value.
#define BLOCKM 0x0000
#define BLACKM 0x0041
#define CLOUDY 0x0082
#define RAIN 0x00C3
#define SNOW 0x0104
#define STORM 0x0145
#define SUNNY 0x0186

void initScreen();
void disableScrolling();
void clearScreen();
void mediaInit();
void media_setSector(uint16_t hi, uint16_t lo);
void wait();
int gotACK();

/*
 * Moves cursor (line, column)
 * x between 0 and 15 inclusive.
 * y between 0 and 17 inclusive.
 */
void moveCursor(uint8_t x, uint8_t y);

/*
 * Call this to place a string of text
 * at the location of the cursor (specifed elsewhere)
 */
void writeString(char *str);

/*
 * Draws a square
 * x between 0 and 124 inclusive.
 * y between 0 and 124 inclusive.
 * color between 0x0000 0xffff
 */
void drawSquare(int x, int y, int size, int color);


//Screen Types
void welcomeScreen();
void weatherScreen();
void newsScreen();
void debugScreen();
void snakeScreen();


extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];	//This may need to be larger

#endif /* SCREEN_H_ */
