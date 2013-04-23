/****************************************************************************
 *   $Id:: i2c_main.c 4785 2010-09-03 22:39:27Z nxp21346                    $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains I2C test modules, main entry, to test I2C APIs.
 *
 ****************************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 ****************************************************************************/
#include <assert.h>
#include <stdio.h>
#include "globals.h"
#include "devices/joystick.h"
#include "screens/watch.h"
#include "devices/screen.h"
#include "gpio.h"
#include "i2c.h"
#include "devices/bluetooth.h"
#include "devices/accelerometer.h"
#include "devices/fuel_gauge.h"
#include "screens/debug.h"
#include "screens/weather.h"
#include "timer32.h"
#include <stdlib.h>
#include "screens/headlines.h"
#include "devices/vibrator.h"
#include "screens/snake.h"
#include "screens/music.h"

static void init_mwatch();
static void run_mwatch();
static void vibrator_check();


static void init_timer();
static void init_gpio();
static void init_i2c();
static void check_snake();

//volatile uint32_t TimeTick = 0;

int main(void) {
	volatile int wait_c = 48000000;
	int i = 0;
	for(wait_c; wait_c >= 0; wait_c--) {
		i++;
	}


//	I2CInit((uint32_t) I2CMASTER);
//	init_bl();
//	init_accel();
//	get_current_voltage();
	// Initialization
  	init_mwatch();

	// Main run loop
	run_mwatch();
	return 0;
}

void SysTick_Handler(void)
{
    static uint64_t milli;
    if(milli == 132) {
    	unixtime++;
    	milli = 0;
    	tick = 1;
    } else if(milli % 66) {
    	s_tick = 1;
    	milli++;
    } else {
    	milli++;
    }
}
//
//void delaySysTick(uint32_t tick)
//{
//  uint32_t timetick;
//
//  timetick = TimeTick;
//  while ((TimeTick - timetick) < tick)
//	  __WFI();
//}

void init_mwatch() {
	// Init timer
	init_timer();
//	init_timer32(0 , 0);
//	while(1);
//	unixtime = 0;
	// Init GPIO
	init_gpio();

	// Init I2C
	init_i2c();

	// Get the current time from Bluetooth

	// Initialize the baud rate for uart
	UARTInit(UART_BAUD);
	// Init Screen
	initScreen();
}

void run_mwatch() {

	while (1) {



		// Timer

		// Change run state if transitioning
		if (next_state != current_state) {
			switch (run_state) {
			case START:
				run_state = STOP;
				break;
			case RUN:
				run_state = STOP;
				break;
			case STOP:
				run_state = START;
				current_state = next_state;
				break;
			default:
//				printf("Invalid run state %d\n", run_state);
				assert(0);
			}
		}

		// Do the work for the state
		switch (current_state) {
		case MAIN_WATCH:
			main_watch();
			break;
		case WEATHER:
			main_weather();
			break;
		case SNAKE:
			main_snake();
			break;
		case HEADLINES:
			main_headlines();
			break;
		case DEBUGSC:
			main_debug();
		case MUSIC:
			main_music();
			break;
		default:
//			printf("Invalid current state state %d\n", current_state);
			assert(0);
		}
		// Do tick checking stuff
		if(tick) {
			// Check the vibrator
			vibrator_check();

			// Tick of the snake
			check_snake();

			tick = 0;
		}
	}
}


void init_timer() {
	SysTick_Config( SysTick->CALIB + 1 );
	unixtime = 0;
}

void init_dir(uint32_t port, uint32_t bit){
	GPIOSetDir( port, bit, 0 );
	GPIOSetInterrupt( port, bit, 1, 1, 0 );
	GPIOIntEnable( port, bit );
}
void init_gpio() {
	GPIOInit();
	init_dir(UPPORT, UPPOS);
	init_dir(DOWNPORT, DOWNPOS);
	init_dir(LEFTPORT, LEFTPOS);
	init_dir(RIGHTPORT, RIGHTPOS);
	init_dir(INPORT, INPOS);

	// Vibrator
	GPIOSetDir( VIB_F_PORT, VIB_F_POS, 1 );
	GPIOSetDir( VIB_S_PORT, VIB_S_POS, 1 );

	joystick_dir = NONE;
}



void init_i2c() {
	uint32_t i2c_result = I2CInit((uint32_t) I2CMASTER);
	assert(i2c_result);

//	get_current_voltage();

	init_bl();
	init_accel();

//	while(1) {
//		update_acc_data();
//		printf("x: %d\ty: %d\tz: %d\n", x_g, y_g, z_g);
//		int i = 0;
//		for(; i < 1000000; i++) {
//
//		}
//	}
}

void vibrator_check() {
	if(vib_start_time != -1) {
		if(unixtime - vib_start_time > vib_duration) {
			GPIOSetValue( VIB_F_PORT, VIB_F_POS, VIB_OFF );
			GPIOSetValue( VIB_S_PORT, VIB_S_POS, VIB_OFF );
		}
	}
}

static void check_snake() {
}
