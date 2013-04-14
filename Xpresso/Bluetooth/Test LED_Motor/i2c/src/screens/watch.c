/*
 * watch.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "watch.h"
#include <assert.h>
#include <stdio.h>
#include "../globals.h"
#include "watch.h"
#include "../devices/screen.h"
#include "gpio.h"
#include "i2c.h"
#include "../devices/bluetooth.h"
#include "../devices/accelerometer.h"
#include "../devices/fuel_gauge.h"
#include "debug.h"

// Private functions
static void start_watch();
static void run_watch();
static void stop_watch();

void main_watch() {
	enum joystick_dir curJoy ;
	curJoy = getJoyDirection();
	switch (curJoy) {
	case LEFT:
		next_state = DEBUGSC;
		run_state = START;
		break;
	case RIGHT:
		next_state = WEATHER;
		run_state = START;
	default:
		break;
	}


	switch (run_state) {
	case START:
		start_watch();
		break;
	case RUN:
		run_watch();
		break;
	case STOP:
		stop_watch();
		break;
	}
}

static void start_watch() {
	timeScreen();
	run_state = RUN;
}

static void run_watch() {

}

static void stop_watch() {

}
