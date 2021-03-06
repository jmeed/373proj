/*****************************************************************************
 *   config.h:  config file for blinky example for NXP LPC11xx Family
 *   Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/

/*********************************************************************************
**                            End Of File
*********************************************************************************/

#define UART_BAUD 6830
//#define UART_BAUD_FAST 115200
#define UART_BAUD_FAST 86400

#define LED_PORT 0		// Port for led
#define LED_BIT 7		// Bit on port for led
#define LED_ON 1		// Level to set port to turn on led
#define LED_OFF 0		// Level to set port to turn off led

#define LED_TOGGLE_TICKS 200 // 100 ticks = 1 Hz flash rate
