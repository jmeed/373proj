/*
 * music.c
 *
 *  Created on: Apr 22, 2013
 *      Author: ftheo
 */

#include "music.h"
#include "../devices/joystick.h"
#include "../devices/bluetooth.h"
#include "../devices/screen.h"
#include "string.h"
#include "i2c.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include "../devices/accelerometer.h"

// Private functions
static void start_music();
static void run_music();
static void stop_music();


void main_music() {
	switch (run_state) {
	case START:
		start_music();
		run_state = RUN;
		break;
	case RUN:
		run_music();
		break;
	case STOP:
		stop_music();
		break;
	}
}

static void start_music() {
	//clearScreen();
	musicScreen();
	itoa(40, (char *) bl_send, 10);
	send_bl_message();
	get_bl_msg_and_process(40);
	music_tick = 0;

	// Track name
	textSize(1, 1);
	moveCursor(2, 1);
	writeString((char *) &bl_receive[2]);

	// Play
	textSize(1, 1);
	moveCursor(8, 3);
	char * play = ">";
	textSize(2, 2);
	writeString((char *) play);

	// Pause
	textSize(1, 1);
	moveCursor(8, 12);
	char * pause = "||";
	textSize(2, 2);
	writeString((char *) pause);

	// Next
	textSize(1, 1);
	moveCursor(5, 7);
	char * next = ">|";
	textSize(2, 2);
	writeString((char *) next);

	// Prev
	textSize(1, 1);
	moveCursor(11, 7);
	char * prev = "|<";
	textSize(2, 2);
	writeString((char *) prev);
}

static void run_music() {
	// Check the accelerometer

	if(music_tick == 1) {
		enum Acc_direction acc_dir = get_current_orientation();

		switch (acc_dir) { //get the current direction
		case AC_NORMAL:
			break;
		case AC_LEFT:
			itoa(46, (char *) bl_send, 10);
			send_bl_message();
			break;
		case AC_FRONT:
			break;
		case AC_RIGHT:
			itoa(47, (char *) bl_send, 10);
			send_bl_message();
			break;
		case AC_BACK:
			break;
		default:
			break;
		}
		music_tick = 0;
	}

	enum Joystick_dir curJoy = getJoyDirection();
	switch (curJoy) {
	case RIGHT: // Previous
		itoa(44, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(44);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);

		// TONY
		itoa(45, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(45);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);
		break;
	case LEFT: // Next
		itoa(43, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(43);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);

		// TONY
		itoa(45, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(45);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);
		break;
	case UP: // Play
		itoa(41, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(41);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);
		break;
	case DOWN: // Pause
		itoa(42, (char *) bl_send, 10);
		send_bl_message();
		get_bl_msg_and_process(42);

		// Track name
		textSize(1, 1);
		moveCursor(2, 1);
		writeString((char *) &bl_receive[2]);
		break;
	case IN:
		next_state = MAIN_WATCH;
		break;
	default:
		break;
	}
}

static void stop_music() {

}
