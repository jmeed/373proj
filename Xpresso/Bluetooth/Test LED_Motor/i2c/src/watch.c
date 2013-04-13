/*
 * watch.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "watch.h"
#include "globals.h"

// Private functions
static void start_watch();
static void run_watch();
static void stop_watch();

void main_watch() {
	switch (run_state) {
	case START:
		start_watch();
		break;
	case RUN:
		run_watch();
		break;
	case STOP:
		stop_watch();
		break;
	}
}

static void start_watch() {

}

static void run_watch() {

}

static void stop_watch() {

}
