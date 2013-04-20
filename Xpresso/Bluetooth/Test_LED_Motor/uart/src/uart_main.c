/****************************************************************************
 *   $Id:: uart_main.c 4824 2010-09-07 18:47:51Z nxp21346                   $
 *   Project: NXP LPC11xx UART example
 *
 *   Description:
 *     This file contains UART test modules, main entry, to test UART APIs.
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
#include "driver_config.h"
#include "target_config.h"

#include "uart.h"
#include "shared.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];

void process_bl_msg();

int main(void) {
	// Initialize
	UARTInit(UART_BAUD);
	// Main loop code
	while(1) {
		// Check bluetooth status
		if (LPC_UART ->IER == DISABLE_IRQ) { // There is a message to read
			process_bl_msg();
		}
	}




	/* Basic chip initialization is taken care of in SystemInit() called
	 * from the startup code. SystemInit() and chip settings are defined
	 * in the CMSIS system_<part family>.c file.
	 */

	/* NVIC is installed inside UARTInit file. */
#if MODEM_TEST
	ModemInit();
#endif

	while (1) { /* Loop forever */
		if (UARTCount != 0) {
			LPC_UART ->IER = IER_THRE | IER_RLS; /* Disable RBR */
			UARTSend((uint8_t *) UARTBuffer, UARTCount);
			UARTCount = 0;
			LPC_UART ->IER = IER_THRE | IER_RLS | IER_RBR; /* Re-enable RBR */
		}
	}
}

void process_bl_msg() {
	assert(UARTCount >= 3); // Ensure that the length is at least 3/enough to cover the opcode "00\0"
	// The first two characters are actually the bluetooth code
	int opcode = 10 * (UARTBuffer[0] - '0');
	opcode = opcode + (UARTBuffer[1] - '0');
//	printf("BL received. opcode: %d\n", opcode);
	switch(opcode) {
	case AUTHENTICATE:
		UARTSend((uint8_t *) UARTBuffer, UARTCount);
		UARTCount = 0;
		LPC_UART ->IER = ENABLE_IRQ; /* Re-enable RBR */
		break;
	default:
//		printf("ERR: opcode [%d] from bluetooth receive not valid\n", opcode);
		assert(0);
		break;
	}
}
