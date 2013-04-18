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
#define AC_WAADR 0xA6
#define AC_RAADR (AC_WAADR | 0x01)

#define ABOUT 50

enum Acc_direction {AC_NORMAL, AC_LEFT, AC_FRONT, AC_RIGHT, AC_BACK};

void init_accel();
int16_t get_acc_direction(uint8_t reg);
void update_acc_data();
enum Acc_direction get_current_orientation();



#endif /* ACCELEROMETER_H_ */
