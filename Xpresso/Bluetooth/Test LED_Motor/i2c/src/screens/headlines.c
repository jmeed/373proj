/*
 * headlines.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "headlines.h"
#include "../globals.h"

// Private functions
static void start_headlines();
static void run_headlines();
static void stop_headlines();

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

}

static void run_headlines() {

}

static void stop_headlines() {

}

