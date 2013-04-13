/*
 * globals.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */
#include "globals.h"
#include "gpio.h"

void enableJoystick(){
	GPIOInit();
	GPIOSetDir( UPPORT, UPPOS, 0 );
	GPIOSetInterrupt( UPPORT, UPPOS, 1, 1, 1 );
	GPIOSetDir( DOWNPORT, DOWNPOS, 1 );
	GPIOSetInterrupt( DOWNPORT, DOWNPOS, 1, 1, 1 );
	GPIOSetDir( LEFTPORT, LEFTPOS, 1 );
	GPIOSetInterrupt( LEFTPORT, LEFTPOS, 1, 1, 1 );
	GPIOSetDir( RIGHTPORT, RIGHTPOS, 1 );
	GPIOSetInterrupt( RIGHTPORT, RIGHTPOS, 1, 1, 1 );
	GPIOSetDir( INPORT, INPOS, 1 );
	GPIOSetInterrupt( INPORT, INPOS, 1, 1, 1 );
	GPIOIntEnable( 0, 0 );
	GPIOIntEnable( 1, 0 );
	GPIOIntEnable( 3, 0 );

}
