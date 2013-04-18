/*
 * vibrator.h
 *
 *  Created on: Apr 17, 2013
 *      Author: ftheo
 */

#ifndef VIBRATOR_H_
#define VIBRATOR_H_

#include <stdint.h>
#include "../globals.h"

#define VIB_F_PORT 2
#define VIB_F_POS 3
#define VIB_S_PORT 3
#define VIB_S_POS 1

#define VIB_ON 1
#define VIB_OFF 0

int64_t vib_start_time;
uint8_t vib_duration;

// Vibrate for x seconds
// It will return immidietly
void vibrate(uint8_t _duration);

#endif /* VIBRATOR_H_ */
