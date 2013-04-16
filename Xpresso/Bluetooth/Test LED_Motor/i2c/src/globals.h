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
	MAIN_WATCH, WEATHER, SNAKE, HEADLINES, DEBUGSC
};
enum run_state {
	START, RUN, STOP
};


uint64_t unixtime;
enum watch_state current_state;
enum watch_state next_state;
enum run_state run_state;
enum WEATHER_TYPE {_CLOUD, _RAIN, _SNOW, _STORM, _SUNNY};

//Accelerometer variables
uint8_t x_g;
uint8_t y_g;
uint8_t z_g;

#endif /* GLOBALS_H_ */
