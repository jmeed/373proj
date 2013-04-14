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
#include "screens/watch.h"
#include "devices/screen.h"
#include "gpio.h"
#include "i2c.h"
#include "devices/bluetooth.h"
#include "devices/accelerometer.h"
#include "devices/fuel_gauge.h"
#include "screens/debug.h"
#include "screens/weather.h"

static void init_mwatch();
static void run_mwatch();


static void init_timer();
static void init_gpio();
static void init_i2c();

volatile uint32_t TimeTick = 0;

int main(void) {
	// Initialization
	init_mwatch();

	// Main run loop
	run_mwatch();

	return 0;
}

void SysTick_Handler(void)
{
  TimeTick++;
}

void delaySysTick(uint32_t tick)
{
  uint32_t timetick;

  timetick = TimeTick;
  while ((TimeTick - timetick) < tick)
	  __WFI();
}

void init_mwatch() {
	// Init timer
	init_timer();
	SysTick_Config( SysTick->CALIB + 1 );

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
				printf("Invalid run state %d\n", run_state);
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
			break;
		case HEADLINES:
			break;
		case DEBUGSC:
			main_debug();
			break;
		default:
			printf("Invalid current state state %d\n", current_state);
			assert(0);
		}
	}
}


void init_timer() {

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

	joystick_dir = NONE;
}



void init_i2c() {
	uint32_t i2c_result = I2CInit((uint32_t) I2CMASTER);
	assert(i2c_result);

	init_bl();
	init_accel();
}


























// Previous TO DELETE

#include "driver_config.h"
#include "target_config.h"

#include "type.h"
#include "i2c.h"
#include "shared.h"

extern volatile uint32_t I2CCount;
extern volatile uint8_t I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;


char msg_g[BUFSIZE];
uint32_t msg_count_g;
uint8_t is_running_on_battery_g;
//uint16_t x_g;
//uint16_t y_g;
//uint16_t z_g;

uint32_t write_byte_to_register(uint8_t addr, uint8_t reg, uint8_t value);
uint32_t send_i2c_msg(uint8_t addr, uint8_t reg);
uint8_t read_byte_from_register(uint8_t r_addr, uint8_t w_addr, uint8_t reg);
uint32_t uartConnected();
void process_bl_msg();
void send_bl_msg();
void configure_i2c_devices();
void set_up_i2c();
uint8_t is_running_on_battery();
uint16_t get_current_voltage();
//uint16_t get_acc_direction(uint8_t reg);
//void update_acc_data();

/*******************************************************************************
 **   Main Function  main()
 *******************************************************************************/
//int main(void) {
//	// Initialization
//	init_mwatch();
//
//	// Main run loop
//	run_mwatch();
//
//	printf("I2C to UART for Bluetooth started \n");
//
//	set_up_i2c();
//	while (1) {
//		// Check the Bluetooth
//		while (read_byte_from_register(BL_RAADR, BL_WAADR, LSR) & 0x01) {
//			msg_g[0] = (char) read_byte_from_register(BL_RAADR, BL_WAADR, RHR);
//			printf("%s", msg_g);
//			fflush(stdout);
//
//			msg_g[0] = (char) read_byte_from_register(BL_RAADR, BL_WAADR, RHR);
//			printf("%s", msg_g);
//			fflush(stdout);
//
//			msg_g[0] = (char) read_byte_from_register(BL_RAADR, BL_WAADR, RHR);
//			printf("%s", msg_g);
//			fflush(stdout);
//
//			/*msg_g[msg_count_g] = (char) read_byte_from_register(BL_RAADR, BL_WAADR, RHR);
//			 msg_count_g++;
//			 if(msg_g[msg_count_g - 1] == '\0') {
//			 printf("%s\n", msg_g);
//			 fflush(stdout);
//			 process_bl_msg();
//			 msg_count_g = 0;
//			 }*/
//		}
//
//		// Check the accelerometer
//					update_acc_data();
//					printf("x: %d\ty: %d\tz: %d\n", x_g, y_g, z_g);
//
//					// Check the fuel gauge
//					if(is_running_on_battery()) {
//						printf("%d%%\n", is_running_on_battery_g);
//					} else {
//						printf("voltage: %d\n", get_current_voltage());
//					}
//
//					fflush(stdout);
//					// Sleep
//					//int i;
//					//for ( i = 0; i < 0x200000; i++ );
//				}
//	return 0;
//}






















// Previous


//uint32_t send_i2c_msg(uint8_t addr, uint8_t reg) {
//	I2CWriteLength = 2 + msg_count_g;
//	I2CReadLength = 0;
//	I2CMasterBuffer[0] = addr;
//	I2CMasterBuffer[1] = reg;
//	int i;
//	for (i = 0; i < msg_count_g; i++) {
//		I2CMasterBuffer[i + 2] = msg_g[i];
//	}
//	I2CEngine();
//
//	// Assert if the I2C is not in ok mode
//	assert(I2CMasterState == I2C_OK);
//	for (i = 0; i < 0x200000; i++)
//		;
//	return I2CMasterState;
//}

uint32_t write_byte_to_register(uint8_t addr, uint8_t reg, uint8_t value) {
	msg_count_g = 1;
	msg_g[0] = value;
	send_i2c_msg(addr, reg);
	msg_count_g = 0;
	return I2CMasterState;
}

uint8_t read_byte_from_register(uint8_t r_addr, uint8_t w_addr, uint8_t reg) {
	I2CWriteLength = 2;
	I2CReadLength = 1;
	I2CMasterBuffer[0] = w_addr;
	I2CMasterBuffer[1] = reg;
	I2CMasterBuffer[2] = r_addr;
	I2CEngine();

	// Assert if the I2C is not in ok mode
	assert(I2CMasterState == I2C_OK);
	return I2CSlaveBuffer[0];
}

void process_bl_msg() {
	assert(msg_count_g >= 3);
	// Ensure that the length is at least 3/enough to cover the opcode "00\0"
	// The first two characters are actually the bluetooth code
	int opcode = 10 * (msg_g[0] - '0');
	opcode = opcode + (msg_g[1] - '0');
	printf("BL received. opcode: %d\n", opcode);
	switch (opcode) {
	case AUTHENTICATE:
//		send_i2c_msg(BLs_WAADR, THR);
		break;
	default:
		printf("ERR: opcode [%d] from bluetooth receive not valid\n", opcode);
		assert(0);
		break;
	}
}

void set_up_i2c() {
	// Initialize I2C

	if (I2CInit((uint32_t) I2CMASTER) == FALSE) /* initialize I2c */
	{
		while (1)
			; /* Fatal error */
	}

	// Check if running on power or battery
	if (is_running_on_battery())
		printf("Device is running on battery\n");
	else
		printf("Device is running on power\n");

//	configure_i2c_devices();
	printf("I2C configured\n");
}

//void configure_i2c_devices() {
	// Set up Bluetooth
//	write_byte_to_register(BL_WAADR, LCR, 0x80); // 0x80 to program baudrate
//	write_byte_to_register(BL_WAADR, DLH, 0x00); // ([14.7456 * 10 ^ 6] / 1) / (115200 * 16) = 8 => 0x0008
//	write_byte_to_register(BL_WAADR, DLL, 0x08); // The desired baud rate is 115200
//	write_byte_to_register(BL_WAADR, LCR, 0x03); // 8 data bit, 1 stop bit, no parity
//	write_byte_to_register(BL_WAADR, FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
//	write_byte_to_register(BL_WAADR, FCR, 0x01); // enable FIFO mode
//	write_byte_to_register(BL_WAADR, IER, 0x01); // enable RHR interrupt
//
//	// Set up the Accelerometer
//	write_byte_to_register(AC_WAADR, 0x2A, 0x01); // Take the accelerometer out of sleep mode
//}

// Return 0 if not running on battery. Else return the % of battery left
//uint8_t is_running_on_battery() {
//	is_running_on_battery_g = read_byte_from_register(FG_RAADR, FG_WAADR, 0x04);
//	return is_running_on_battery_g == 0 ? 0 : 1;
//}
//
//uint16_t get_current_voltage() {
//	return (read_byte_from_register(FG_RAADR, FG_WAADR, 0x02) << 8)
//			+ (read_byte_from_register(FG_RAADR, FG_WAADR, 0x03) << 4);
//}

//uint16_t get_acc_direction(uint8_t reg) {
//	return (read_byte_from_register(AC_RAADR, AC_WAADR, reg) << 4)
//			+ (read_byte_from_register(AC_RAADR, AC_WAADR, (reg + 0x01)) >> 4);
//}
//
//void update_acc_data() {
//	x_g = get_acc_direction(0x01);
//	y_g = get_acc_direction(0x03);
//	z_g = get_acc_direction(0x05);
//}

uint32_t uartConnected() {
	/*

	 Check that UART is connected and operational.

	 */
	// Perform read/write test to check if UART is working
//	const char TEST_CHARACTER = 'H';

//	write_register(SPR, TEST_CHARACTER);

//	return (read_register(SPR) == TEST_CHARACTER);
	return 0;
}


/******************************************************************************
 **                            End Of File
 ******************************************************************************/
