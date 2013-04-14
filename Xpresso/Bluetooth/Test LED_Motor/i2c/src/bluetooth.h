/*
 * bluetooth.h
 *
 *  Created on: Apr 13, 2013
 *      Author: ftheo
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>

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

void init_bl();

void send_bl_message();
uint8_t is_bl_message_available();
uint8_t receive_bl_message();

#endif /* BLUETOOTH_H_ */
