/*
 * globals.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdint.h>

enum watch_state {
	MAIN_WATCH, WEATHER, SNAKE, HEADLINES
};
enum run_state {
	START, RUN, STOP
};

enum joystick_dir {
	UP, DOWN, LEFT, RIGHT, IN
};



uint64_t unixtime;
enum watch_state current_state;
enum watch_state next_state;
enum run_state run_state;

// Joystick definitions
#define UPPORT 1
#define UPPOS 1
#define DOWNPORT 1
#define	DOWNPOS 2
#define LEFTPORT 0
#define LEFTPOS 11
#define RIGHTPORT 3
#define RIGHTPOS 0
#define INPORT 1
#define INPOS 0

void enableJoystick();

#endif /* GLOBALS_H_ */