/*
 * shared.h
 *
 *  Created on: Mar 31, 2013
 *      Author: ftheo
 */

#ifndef SHARED_H_
#define SHARED_H_

// Bluetooth slave address
#define BL_WAADR 0x90
#define BL_RAADR (BL_WAADR | 0x01)

//Accelerometer slave address
#define AC_WAADR 0x3A
#define AC_RAADR (AC_WAADR | 0x01)

// Fuel gauge slave address
#define FG_WAADR 0x6C
#define FG_RAADR (FG_WAADR | 0x01)

#define DIVL 0x00
#define DIVM 0x08

// I2C to uart globals

#define EFR_ENABLE_CTS 1 << 7
#define EFR_ENABLE_RTS 1 << 6
#define EFR_ENABLE_ENHANCED_FUNCTIONS 1 << 4

#define THR        0x00 << 3
#define RHR        0x00 << 3
#define IER        0x01 << 3
#define FCR        0x02 << 3
#define IIR        0x02 << 3
#define LCR        0x03 << 3
#define MCR        0x04 << 3
#define LSR        0x05 << 3
#define MSR        0x06 << 3
#define SPR        0x07 << 3
#define TXLVL      0x08 << 3
#define RXLVL      0x09 << 3
#define DLAB       0x80 << 3
#define IODIR      0x0A << 3
#define IOSTATE    0x0B << 3
#define IOINTMSK   0x0C << 3
#define IOCTRL     0x0E << 3
#define EFCR       0x0F << 3

#define DLL        0x00 << 3
#define DLH        0x01 << 3
#define EFR        0x02 << 3
#define XON1       0x04 << 3
#define XON2       0x05 << 3
#define XOFF1      0x06 << 3
#define XOFF2      0x07 << 3


#define DISABLE_IRQ (IER_THRE | IER_RLS)
#define ENABLE_IRQ (IER_THRE | IER_RLS | IER_RBR)

// Bluetooth opcodes
#define AUTHENTICATE 0




#endif /* SHARED_H_ */
