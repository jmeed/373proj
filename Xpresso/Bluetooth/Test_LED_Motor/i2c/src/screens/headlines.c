/*
 * headlines.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "headlines.h"
#include "../globals.h"
#include <string.h>
#include "../devices/bluetooth.h"
#include "../devices/screen.h"
#include "../devices/joystick.h"

// Private functions
static void start_headlines();
static void run_headlines();
static void stop_headlines();
static void get_headline(uint8_t opcode);

void main_headlines() {
	switch (run_state) {
	case START:
		start_headlines();
		run_state = RUN;
		break;
	case RUN:
		run_headlines();
		break;
	case STOP:
		stop_headlines();
		break;
	}
}

static void start_headlines() {
	get_headline(B_HEAD_INIT);
}

static void run_headlines() {
	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case RIGHT:
		get_headline(B_HEAD_NEXT);
		break;
	case LEFT:
		get_headline(B_HEAD_PREV);
		break;
	case IN:
		next_state = MAIN_WATCH;
	default:
		break;
	}
}

static void stop_headlines() {

}

static void get_headline(uint8_t opcode) {
	set_bl_opcode(opcode);
	send_bl_message();
	get_bl_msg_and_process(opcode);
	newsScreen();
	char * headline = (char *) bl_receive + 2;
	writeString(headline);
}

