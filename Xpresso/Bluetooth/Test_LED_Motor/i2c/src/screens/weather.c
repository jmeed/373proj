/*
 * weather.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "weather.h"
#include "../devices/joystick.h"
#include "../devices/bluetooth.h"
#include "../devices/screen.h"
#include "string.h"
#include "i2c.h"
#include "gpio.h"
#include <stdio.h>

// Private functions
static void start_weather();
static void run_weather();
static void stop_weather();
static void get_weather(uint8_t opcode);

void main_weather() {
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
	get_weather(10);
}

static void run_weather() {
	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case RIGHT:
		get_weather(12);
		break;
	case LEFT:
		get_weather(10);
		break;
	case IN:
		next_state = MAIN_WATCH;
		break;
	default:
		break;
	}
}

static void stop_weather() {

}

static void get_weather(uint8_t opcode) {
	char * opcode_char;
	char * title;
	char * navigation;
	if(opcode == 10) {
		opcode_char = "10";
		title = "Current Weather\n";
		navigation = "-> Forecast";
	} else {
		opcode_char = "12";
		title = "Forecast\n";
		navigation = "<- Current Weather";
	}


	strcpy((char *) bl_send, opcode_char);
	send_bl_message();
	get_bl_msg_and_process(opcode);
	enum WEATHER_TYPE weather = bl_receive[2] - '0';
	weatherScreen(weather, title);
	char * weather_explanation = (char *) bl_receive + 3;
	writeString(weather_explanation);
	moveCursor(15, 0);
	writeString(navigation);
}
