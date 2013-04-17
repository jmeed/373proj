/*
 * bluetooth.c
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#include "bluetooth.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "i2c_com.h"
#include "../globals.h"

// bl_send should point to the first available byte where the message will begin
uint8_t * const bl_send = (uint8_t *) I2CMasterBuffer + 2;

void init_bl() {
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
	printf("BL msg send: [%s]\n", (char *) bl_send);
	send_i2c_msg(BL_WAADR, BL_THR, (sizeof((char *)I2CMasterBuffer) - 1));
}

uint8_t is_bl_message_available() {
	return read_i2c_register(BL_RAADR, BL_WAADR, BL_LSR) & 0x01;
}

// Returns the number of characters it read from the Bluetooth
// Data is available in the I2CSlaveBuffer
uint8_t receive_bl_message() {
	uint8_t index = 0;
	uint64_t start = unixtime;
	while(unixtime - start <= BL_TIMEOUT) {
		if(is_bl_message_available()) {
			bl_receive[index] = (char) read_i2c_register(BL_RAADR, BL_WAADR, BL_RHR);
			index++;
			if(bl_receive[index - 1] == '\0') { // Got the entire message
				printf("BL msg rcv size: [%d]\n", index);
				break;
			}
			// If we have gotten 64 characters but not null than we want to send an ack and read the rest of them again
			if(index % 64 == 0) {
				// Send an ack
				strcpy((char *) bl_send, "33");
				send_bl_message();
			}
		}
	}
	if(unixtime - start >= 3) index = -1;
	return index;
}

uint8_t get_bl_msg_and_process(uint8_t opcode_requested) {
	uint8_t result = receive_bl_message();

	if(result == -1) {
		printf("BL timeout for opcode %d\n", opcode_requested);
		assert(0);
	}

	// Ensure we got the message we asked for
	uint8_t opcode_received = 10 * (bl_receive[0] - '0');
	opcode_received = opcode_received + (bl_receive[1] - '0');

	if(opcode_received == opcode_requested) return result;

	/*
	// If we get phone call working
	switch(opcode_received) {
	default:
		assert(0);
		break;
	}
	*/

	return result;
}
