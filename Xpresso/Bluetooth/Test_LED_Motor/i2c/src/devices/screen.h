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
#include "../globals.h"


//Screen image #defines. Set as y value.
#define BLOCKM 0x0000
#define BLACKM 0x0041
#define CLOUDY 0x0082
#define RAIN 0x00C3
#define SNOW 0x0104
#define STORM 0x0145
#define SUNNY 0x0186
#define NEWS 0x01C7
#define TIME1 0x0208
#define TIME2 0x0249
#define TIME3 0x028A
#define TIME4 0x02CB

#define SCREEN_TIMEOUT 3

void initScreen();
void setBaudRate();
void disableScrolling();
void clearScreen();
void mediaInit();
void media_setSector(uint16_t hi, uint16_t lo);
void wait();
int gotACK();

/*
 * Set test size of watch
 * width is in [1, 16] (default 1)
 * height is in [1,16] (default 1)
 * text size is a multiple of the default;
 * that is, 5 means text is 5x size of default.
 */
void textSize(uint16_t width, uint16_t height);

/*
 * Moves cursor (line, column)
 * x between 0 and 15 inclusive.
 * y between 0 and 17 inclusive.
 * X AND Y ARE SWITCHED!!!!!!!!!!!!!!
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
void drawSquare(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void drawFillSquare(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);


//Screen Types
/*
 * This screen has the block M and M watch and our names
 */
void welcomeScreen();

/*
 * this screen displays the appropriate weather background
 * for the enum passed in WEATHER_TYPE
 */
void weatherScreen(enum WEATHER_TYPE w, char * weather_title);

/*
 * this screen displays the news background and sets cursor
 * for news placement
 */
void newsScreen();

/*
 * This screen is black with green text. Cursor set for debug data.
 */
void debugScreen();

/*
 * Clears the screen to black for snake play
 */
void snakeScreen();

/*
 * Sets time screen. Places cursor ready for time data.
 */
void timeScreen();

uint16_t get_pixel(uint16_t x, uint16_t y);


extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZEUART];	//This may need to be larger

#endif /* SCREEN_H_ */
