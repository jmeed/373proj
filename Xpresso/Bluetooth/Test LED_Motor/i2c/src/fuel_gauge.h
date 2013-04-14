/*
 * fuel_gauge.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef FUEL_GAUGE_H_
#define FUEL_GAUGE_H_

#include <stdint.h>

// Fuel gauge slave address
#define FG_WAADR 0x6C
#define FG_RAADR (FG_WAADR | 0x01)

uint8_t is_running_on_battery();
uint8_t get_power_remaining();
uint16_t get_current_voltage();

#endif /* FUEL_GAUGE_H_ */
