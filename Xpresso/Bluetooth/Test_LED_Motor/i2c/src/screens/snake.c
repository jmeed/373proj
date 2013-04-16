/*
 * snake.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "snake.h"
#include "../globals.h"

// Private functions
static void start_snake();
static void run_snake();
static void stop_snake();

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

}

static void run_snake() {

}

static void stop_snake() {

}

