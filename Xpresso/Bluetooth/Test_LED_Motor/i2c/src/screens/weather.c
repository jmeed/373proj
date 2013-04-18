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
#include <stdlib.h>

// Private functions
static void start_weather();
static void run_weather();
static void stop_weather();
static void get_weather(uint8_t opcode);
static void print_weather(char * title, char * footer);

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
	get_weather(B_WEATHER_CURR);
	print_weather("Current Weather\n", "^ Forecast");
}

static void run_weather() {
	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case RIGHT:
		get_weather(B_WEATHER_CURR);
		print_weather("Current Weather\n", "^ Forecast");
		break;
	case LEFT:
		get_weather(B_WEATHER_FOR);
		print_weather("Forecast\n", "v Current Weather");
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
	itoa(opcode, (char *) bl_send, 10);
	send_bl_message();
	get_bl_msg_and_process(opcode);
}

static void print_weather(char * title, char * footer) {
	enum WEATHER_TYPE weather = bl_receive[2] - '0';
	weatherScreen(weather, title);
	char * weather_explanation = (char *) bl_receive + 3;
	writeString(weather_explanation);
	moveCursor(15, 0);
	writeString(footer);
}
