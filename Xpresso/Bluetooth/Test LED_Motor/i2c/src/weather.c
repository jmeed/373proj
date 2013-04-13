/*
 * weather.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "weather.h"
#include "globals.h"

// Private functions
static void start_weather();
static void run_weather();
static void stop_weather();

void main_weather() {
	switch (run_state) {
	case START:
		start_weather();
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

}

static void run_weather() {

}

static void stop_weather() {

}
