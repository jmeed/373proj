/*
 * weather.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "weather.h"
#include "globals.h"
#include "bluetooth.h"
#include "string.h"

// Private functions
static void start_weather();
static void run_weather();
static void stop_weather();

void main_weather() {
//	if (run_state == START){
//		weatherScreen();
//		run_state = RUN;
//	}
//
//	enum joystick_dir curJoy ;
//	curJoy = getJoyDirection();
//	switch (curJoy) {
//	case IN:
//		next_state = MAIN_WATCH;
//		break;
//	default:
//		break;
//	}

	switch (run_state) {
	case START:
		start_weather();
		run_state = RUN;
		break;
	case RUN:
		run_weather();
		break;
	case STOP:
		stop_weather();
		break;
	}
}

static void start_weather() {
	strcpy((char *) bl_send, "30");
	send_bl_message();

}

static void run_weather() {

}

static void stop_weather() {

}
