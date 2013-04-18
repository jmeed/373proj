/*
 * snake.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "snake.h"
#include <stdlib.h>
#include <time.h>
#include "../globals.h"
#include "../devices/screen.h"

// Private functions
static void start_snake();
static void run_snake();
static void stop_snake();

struct dll{
	uint8_t x,y;
	struct dll * f;
};

struct dll * front, * back;

static void draw_snake_square(struct dll *square);
static void create_food();
static void clear_snake_square(struct dll *square);
static uint8_t rand_lim(uint8_t lower,uint8_t upper);

void main_snake() {
	switch (run_state) {
	case START:
		start_snake();
		run_state = RUN;
		break;
	case RUN:
		run_snake();
		break;
	case STOP:
		stop_snake();
		break;
	}
}

static void start_snake() {
	clearScreen();
	drawSquare(0, 0, 127, 127, 0xffff);
	front = malloc(sizeof(struct dll));
	front->x = rand_lim(1, 126); //random number between 0
	front->y = rand_lim(1, 126); //random number between 62
	front->f = NULL;
	back = malloc(sizeof(struct dll));
	back->x = front->x + 1;
	back->y = front->y;
	back->f = front;
	draw_snake_square(front);
	draw_snake_square(back);
}

static void run_snake() {

}

static void stop_snake() {

}


static void draw_snake_square(struct dll *square) {
	drawSquare(square->x, square->y, square->x+1, square->y+1, 0xffff);
}

static void create_food() {

}

void add_snake_square(int8_t x, int8_t y) {
	x = front->x + x;
	y = front->y + y;
	if (get_pixel(x,y) == 0xFFFF) { //ran into wall or snake tail;
		// Die
		return;
	}
	struct dll * newHead;
	newHead = malloc(sizeof(struct dll));
	newHead->x = x; //random number between 0
	newHead->y = y;//random number between 62
	newHead->f = NULL;
	front->f = newHead;
	front = newHead;
	draw_snake_square(newHead);
	if (get_pixel(x,y) == 0x0000) { //not ran into food
		struct dll * tempBack;
		tempBack = back;
		back = back->f;
		clear_snake_square(tempBack);
		return;
	}

	// Run into food
	create_food();
	return;
}

static void clear_snake_square(struct dll *square) {
	drawSquare(square->x, square->y, square->x+1, square->y+1, 0xFFFF); //draw a 2x2 square starting at x,y
	free(square);
}

static uint8_t rand_lim(uint8_t lower,uint8_t upper) {
/* return a random number between 0 and limit inclusive.
 */

    int divisor = RAND_MAX/(upper+1);
    uint8_t retval;

    do {
        retval = rand() / divisor;
    } while (retval > upper || retval < lower);

    return retval;
}
