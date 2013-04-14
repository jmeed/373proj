/*
 * bluetooth.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "bluetooth.h"
#include <stdio.h>
#include "i2c_com.h"
#include "i2c.h"

void send_bl_message() {
	send_i2c_msg(BL_WAADR, BL_THR, sizeof(I2CMasterBuffer));
}

uint8_t is_bl_message_available() {
	return read_i2c_register(BL_RAADR, BL_WAADR, BL_LSR) & 0x01;
}

// Returns the number of characters it read from the Bluetooth
// Data is available in the I2CSlaveBuffer
uint8_t receive_bl_message() {
	uint8_t length = 0;
	while(is_bl_message_available()) {
		read_i2c_register(BL_RAADR, BL_WAADR, BL_RHR);
		length++;
	}
	printf("BL message: [%s]\n", (char *) I2CSlaveBuffer);
	return length;
}
