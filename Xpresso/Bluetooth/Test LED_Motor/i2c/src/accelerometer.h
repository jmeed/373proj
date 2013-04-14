/*
 * accelerometer.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include <stdint.h>

//Accelerometer slave address
#define AC_WAADR 0x3A
#define AC_RAADR (AC_WAADR | 0x01)

uint16_t x_g;
uint16_t y_g;
uint16_t z_g;

void init_accel();
uint16_t get_acc_direction(uint8_t reg);
void update_acc_data();

#endif /* ACCELEROMETER_H_ */
