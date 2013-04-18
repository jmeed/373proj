/*
 * vibrator.c
 *
 *  Created on: Apr 17, 2013
 *      Author: ftheo
 */

#include "vibrator.h"
#include "gpio.h"

void init_vibrator() {
	vib_start_time = -1;
}

void vibrate(uint8_t _duration) {
	// Start the GPIO for the vibrator
	GPIOSetValue( VIB_F_PORT, VIB_F_POS, VIB_ON );
	GPIOSetValue( VIB_S_PORT, VIB_S_POS, VIB_ON );

	vib_start_time = unixtime;
	vib_duration = _duration;

}
