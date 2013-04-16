/*
 * watch.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "watch.h"
#include <assert.h>
#include <stdio.h>
#include "../devices/joystick.h"
#include "watch.h"
#include "../devices/screen.h"
#include "gpio.h"
#include "i2c.h"
#include "../devices/bluetooth.h"
#include "../devices/accelerometer.h"
#include "../devices/fuel_gauge.h"
#include "debug.h"
#include <string.h>

// Private functions
static void start_watch();
static void run_watch();
static void stop_watch();

void main_watch() {
	switch (run_state) {
	case START:
		start_watch();
		run_state = RUN;
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
	strcpy((char *) bl_send, "20");
	send_bl_message();
	wait_bl_and_receive(20);

}

static void run_watch() {
	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case LEFT:
		next_state = DEBUGSC;
		break;
	case DOWN:
		next_state = WEATHER;
	default:
		break;
	}
}

static void stop_watch() {

}
