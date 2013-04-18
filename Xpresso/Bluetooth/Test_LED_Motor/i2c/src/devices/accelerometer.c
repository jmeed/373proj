/*
 * accelerometer.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "accelerometer.h"
#include "i2c_com.h"
#include "../globals.h"
#include "../devices/i2c_com.h"
#include "i2c.h"
#include <stdio.h>

static uint8_t is_about(uint8_t des_axes, uint8_t cur_axes);

void init_accel() {
	x_g = 0;
	y_g = 0;
	z_g = 0;
	write_i2c_register(AC_WAADR, 0x2D, 0);
	write_i2c_register(AC_WAADR, 0x2D, 16);
	write_i2c_register(AC_WAADR, 0x2D, 8);
//	write_i2c_register(AC_WAADR, 0x2A, 0x01); // Take the accelerometer out of sleep mode
}

int16_t get_acc_direction(uint8_t reg) {
	return (read_i2c_register(AC_RAADR, AC_WAADR, reg) + (read_i2c_register(AC_RAADR, AC_WAADR, (reg + 1)) << 8));
}

void update_acc_data() {
	x_g = get_acc_direction(0x00);
	x_g = get_acc_direction(0x32);
	y_g = get_acc_direction(0x34);
	z_g = get_acc_direction(0x36);
}

enum Acc_direction get_current_orientation() {
	update_acc_data();
	printf("x: %d\ty: %d\tz: %d\n", x_g, y_g, z_g);


	// Check x
	if(x_g < -100) {
		return AC_LEFT;
	}

	if(x_g > 125) {
		return AC_RIGHT;
	}

	if(y_g < -125) {
		return AC_BACK;
	}

	if(y_g > 125) {
		return AC_FRONT;
	}

//	if(is_about(0, x_g) && is_about(0, y_g) && is_about(0, z_g)) {
////		printf("AC_NORMAL\n");
//	} else {
////		printf("=======NOT NORMAL====\n");
//	}


	return AC_NORMAL;
}


static uint8_t is_about(uint8_t des_axes, uint8_t cur_axes) {
	uint8_t wrap = 0;
	uint8_t lower = cur_axes - ABOUT;
	uint8_t upper = cur_axes + ABOUT;
//	printf("lower: %d\t upper: %d\n", lower, upper);
	if((upper <  cur_axes) || lower >  cur_axes) {
		lower = cur_axes + ABOUT;
		upper = cur_axes - ABOUT;
		wrap = 1;
	}

	if(wrap) {
		return !(des_axes >= lower && des_axes <= upper);
	} else {
		return (des_axes >= lower && des_axes <= upper);
	}
}







