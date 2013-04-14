/*
 * bluetooth.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "bluetooth.h"
#include <stdio.h>
#include "i2c_com.h"

void init_bl() {
	bl_send = I2CMasterBuffer + 2;
	// Set up Bluetooth
	write_i2c_register(BL_WAADR, BL_LCR, 0x80); // 0x80 to program baudrate
	write_i2c_register(BL_WAADR, BL_DLH, 0x00); // ([14.7456 * 10 ^ 6] / 1) / (115200 * 16) = 8 => 0x0008
	write_i2c_register(BL_WAADR, BL_DLL, 0x08); // The desired baud rate is 115200
	write_i2c_register(BL_WAADR, BL_LCR, 0x03); // 8 data bit, 1 stop bit, no parity
	write_i2c_register(BL_WAADR, BL_FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	write_i2c_register(BL_WAADR, BL_FCR, 0x01); // enable FIFO mode
	write_i2c_register(BL_WAADR, BL_IER, 0x01); // enable RHR interrupt
}

void send_bl_message() {
	// Make sure there is no NULL in the first bits
	I2CMasterBuffer[0] = 0xFF;
	I2CMasterBuffer[1] = 0xFF;
	send_i2c_msg(BL_WAADR, BL_THR, (sizeof(I2CMasterBuffer) - 2));
}

uint8_t is_bl_message_available() {
	return read_i2c_register(BL_RAADR, BL_WAADR, BL_LSR) & 0x01;
}

// Returns the number of characters it read from the Bluetooth
// Data is available in the I2CSlaveBuffer
uint8_t receive_bl_message() {
	uint8_t index = 0;
	while(is_bl_message_available()) {
		bl_receive[index] = read_i2c_register(BL_RAADR, BL_WAADR, BL_RHR);
		index++;
		if(bl_receive[index - 1] == '\0') { // Got the entire message
			break;
		}
	}
	printf("BL message: [%s]\n", (char *) I2CSlaveBuffer);
	return index;
}
