/*
 * watch.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "watch.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
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
#include "../globals.h"
#include <stdlib.h>

// Private functions
static void start_watch();
static void run_watch();
static void stop_watch();
static void get_time();

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
	get_time();
}

static void run_watch() {
	get_time();
	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case LEFT:
		next_state = DEBUGSC;
		break;
	case DOWN:
		next_state = WEATHER;
		break;
	case UP:
		next_state = HEADLINES;
		break;
	case NONE:
		moveCursor(7,4);
		writeString(unixtime_str);
	default:
		break;
	}
}

static void get_time() {
	strcpy((char *) bl_send, "20");
	send_bl_message();
	wait_bl_and_receive(20);
	unixtime = atol((((char *)bl_receive) + 2));

	uint8_t seconds = unixtime % 60;
	uint8_t minutes = (unixtime / 60) % 60;
	uint8_t hours = (unixtime / (60 * 24)) % 24 - 6;

	uint8_t increment = 0;
	if (hours < 10) {
		unixtime_str[0] = '0';
		increment = 1;
	}
	itoa(hours, (unixtime_str + increment), 10);
	unixtime_str[2] = ':';

	increment = 3;
	if (minutes < 10) {
		unixtime_str[3] = '0';
		increment = 4;
	}

	itoa(minutes, (unixtime_str + increment), 10);
	unixtime_str[5] = ':';

	increment = 6;
	if (seconds < 10) {
		unixtime_str[6] = '0';
		increment = 7;
	}

	itoa(seconds, (unixtime_str + increment), 10);

	moveCursor(7,4);
	writeString(unixtime_str);
}

static void stop_watch() {

}
