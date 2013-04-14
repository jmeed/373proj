/*
 * fuel_gauge.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "fuel_gauge.h"
#include "i2c_com.h"

uint8_t is_running_on_battery() {
	return read_i2c_register(FG_RAADR, FG_WAADR, 0x04) == 0 ? 0 : 1;
}

uint8_t get_power_remaining() {
	return read_i2c_register(FG_RAADR, FG_WAADR, 0x04) == 0 ? 0 : 1;
}

uint16_t get_current_voltage() {
	return (read_i2c_register(FG_RAADR, FG_WAADR, 0x02) << 8)
			+ (read_i2c_register(FG_RAADR, FG_WAADR, 0x03) << 4);
}
