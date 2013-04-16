/*
 * joystick.h
 *
 *  Created on: Apr 15, 2013
 *      Author: ftheo
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

// Joystick definitions
#define UPPORT 1
#define UPPOS 1
#define DOWNPORT 1
#define	DOWNPOS 2
#define LEFTPORT 0
#define LEFTPOS 11
#define RIGHTPORT 3
#define RIGHTPOS 0
#define INPORT 1
#define INPOS 0

enum Joystick_dir {
	UP, DOWN, LEFT, RIGHT, IN, NONE
};

enum Joystick_dir joystick_dir;

enum Joystick_dir getJoyDirection();

#endif /* JOYSTICK_H_ */
