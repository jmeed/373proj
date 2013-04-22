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
#include "../devices/joystick.h"
#include "../devices/accelerometer.h"

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
	front->x = rand_lim(1, 124); //random number between 0
	front->y = rand_lim(1, 124); //random number between 62
	front->f = NULL;
	back = malloc(sizeof(struct dll));
	back->x = front->x + 3;
	back->y = front->y;
	back->f = front;
	draw_snake_square(front);
	draw_snake_square(back);
	s_dir = S_L;
	s_tick = 0;
}

static void run_snake() {
	if (s_tick) {
		// Check accelerometer
		enum Acc_direction acc_dir = get_current_orientation();

		switch (acc_dir){ //get the current direction
		case AC_NORMAL:
			break;
		case AC_LEFT:
			s_dir = S_U;
			break;
		case AC_FRONT:
			s_dir = S_R;
			break;
		case AC_RIGHT:
			s_dir = S_D;
			break;
		case AC_BACK:
			s_dir = S_L;
			break;
		}



		// Update the square based on the direction
		switch (s_dir){ //get the current direction
		case S_U:
			add_snake_square(0, -3);
			break;
		case S_R:
			add_snake_square(3, 0);
			break;
		case S_D:
			add_snake_square(0, 3);
			break;
		case S_L:
			add_snake_square(-3, 0);
			break;
		}
		s_tick = 0;
	}
}

static void stop_snake() {
	while(front) {
		struct dll * tmp = front->f;
		free(front);
		front = tmp;
	}
}


static void draw_snake_square(struct dll *square) {
	drawFillSquare(square->x, square->y, square->x+3, square->y+3, 0xffff);
}

static void create_food() {

}

void add_snake_square(int8_t x, int8_t y) {
	x = front->x + x;
	if (x == 0) {
		x = x;
	}
	y = front->y + y;
	if (get_pixel(x,y) == 0xCAFF) { //ran into wall or snake tail;
		textSize(2,2);
		moveCursor(3,1);
		writeString("You lost\n :(");
		uint64_t start_s = unixtime;
		while(unixtime - start_s < 3) {

		}
		next_state = MAIN_WATCH;
		return;
	}
	struct dll * newHead;
	newHead = malloc(sizeof(struct dll));
	newHead->x = x;
	newHead->y = y;
	newHead->f = NULL;
	front->f = newHead;
	front = newHead;
	draw_snake_square(newHead);
	uint16_t f = get_pixel(x,y);
	f = f;
	if (get_pixel(x,y) == 0xCA00) { //not ran into food
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
	drawFillSquare(square->x, square->y, square->x+3, square->y+3, 0x0000); //draw a 2x2 square starting at x,y
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
