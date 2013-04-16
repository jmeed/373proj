/*
 * joystick.c
 *
 *  Created on: Apr 15, 2013
 *      Author: ftheo
 */

#include "joystick.h"

enum Joystick_dir getJoyDirection(){
	enum Joystick_dir current_dir = joystick_dir;
	joystick_dir = NONE;
	return current_dir;
}
