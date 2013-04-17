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

void init_accel() {
	x_g = 0;
	y_g = 0;
	z_g = 0;
	write_i2c_register(AC_WAADR, 0x2D, 0);
	write_i2c_register(AC_WAADR, 0x2D, 16);
	write_i2c_register(AC_WAADR, 0x2D, 8);
//	write_i2c_register(AC_WAADR, 0x2A, 0x01); // Take the accelerometer out of sleep mode
}

uint8_t get_acc_direction(uint8_t reg) {
	return (read_i2c_register(AC_RAADR, AC_WAADR, reg));
}

void update_acc_data() {
	x_g = get_acc_direction(0x00);
	x_g = get_acc_direction(0x32);
	y_g = get_acc_direction(0x34);
	z_g = get_acc_direction(0x36);
}
