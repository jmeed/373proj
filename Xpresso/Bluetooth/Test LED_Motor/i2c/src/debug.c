/*
 * watch.c
 *
 *  Created on: Apr 13, 2013
 *      Author: not ftheo
 */

#include <assert.h>
#include <stdio.h>
#include "globals.h"
#include "watch.h"
#include "screen.h"
#include "gpio.h"
#include "i2c.h"
#include "bluetooth.h"
#include "accelerometer.h"
#include "fuel_gauge.h"
#include "debug.h"

// Private functions

void main_debug() {
	if (run_state == START){
		debugScreen();
		run_state = RUN;
	}

	enum joystick_dir curJoy  = getJoyDirection();
	switch (curJoy) {
	case IN:
		next_state = MAIN_WATCH;
		break;
	default:
		break;
	}

	char text[20];
	sprintf(text ,"Joystick Enum %d\n", joystick_dir);
	writeString(text);
	update_acc_data();
	sprintf(text, "Accel x: %u      \n", x_g);
	writeString(text);
	sprintf(text, "Accel y: %u     \n", y_g);
	writeString(text);
	sprintf(text, "Accel z: %u     \n", z_g);
	writeString(text);
	if (is_running_on_battery())
	{
		sprintf(text, "Battery: %u%%\n", get_power_remaining());
		writeString(text);
	}
	else
	{
		sprintf(text, "Watch plugged in\n", get_power_remaining());
		writeString(text);
	}
	moveCursor(1,0);

}
