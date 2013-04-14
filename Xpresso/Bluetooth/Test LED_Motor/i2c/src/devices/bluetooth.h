/*
 * bluetooth.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include "i2c.h"

// Bluetooth slave address
#define BL_WAADR 0x90
#define BL_RAADR (BL_WAADR | 0x01)

#define BL_EFR_ENABLE_CTS 1 << 7
#define BL_EFR_ENABLE_RTS 1 << 6
#define BL_EFR_ENABLE_ENHANCED_FUNCTIONS 1 << 4

#define BL_THR        0x00 << 3
#define BL_RHR        0x00 << 3
#define BL_IER        0x01 << 3
#define BL_FCR        0x02 << 3
#define BL_IIR        0x02 << 3
#define BL_LCR        0x03 << 3
#define BL_MCR        0x04 << 3
#define BL_LSR        0x05 << 3
#define BL_MSR        0x06 << 3
#define BL_SPR        0x07 << 3
#define BL_TXLVL      0x08 << 3
#define BL_RXLVL      0x09 << 3
#define BL_DLAB       0x80 << 3
#define BL_IODIR      0x0A << 3
#define BL_IOSTATE    0x0B << 3
#define BL_IOINTMSK   0x0C << 3
#define BL_IOCTRL     0x0E << 3
#define BL_EFCR       0x0F << 3

#define BL_DLL        0x00 << 3
#define BL_DLH        0x01 << 3
#define BL_EFR        0x02 << 3
#define BL_XON1       0x04 << 3
#define BL_XON2       0x05 << 3
#define BL_XOFF1      0x06 << 3
#define BL_XOFF2      0x07 << 3

#define BL_BUFFER (BUFSIZE -2) // Do not change this

// Buffers
uint8_t *bl_send; // Do not change the pointer !!!
volatile char bl_receive[BUFSIZE];

void init_bl();

// To send Bluetooth message put the message in the bl_send
// The max length is BL_BUFFER
void send_bl_message();

// Return 1 if there is a bluetooth message 0 otherwise
uint8_t is_bl_message_available();

// Call this function when you want to get a bluetooth message
// The message will return once a complete bluetooth message has been read
// A complete message is when the Bluetooth has received the '\0' character
// If no message is available it will return 0
// This is a blocking function
// It will block untill the message is received
// The data is return to the bl_send array
// The max received data is BUFSIZE
// It return the length of the data it read
uint8_t receive_bl_message();

// Wait untill you get a full bluetooth message. This functions is exactly as the receive_bl_message
// except that it waits untill it gets a message
uint8_t wait_bl_and_receive();

#endif /* BLUETOOTH_H_ */
