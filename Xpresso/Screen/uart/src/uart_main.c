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
#include "driver_config.h"
#include "target_config.h"

#include "uart.h"
#include "screen.h"

extern volatile uint32_t UARTCount;
extern volatile uint8_t UARTBuffer[BUFSIZE];	//This may need to be larger

int main (void) {
	  /* Basic chip initialization is taken care of in SystemInit() called
	   * from the startup code. SystemInit() and chip settings are defined
	   * in the CMSIS system_<part family>.c file.
	   */

  /* NVIC is installed inside UARTInit file. */
  UARTInit(UART_BAUD);

#if MODEM_TEST
  ModemInit();
#endif

  //Disable Interrupts
  //LPC_UART->IER = IER_THRE | IER_RLS;

  UARTBuffer[0]=0xFF;
  UARTBuffer[1]=0xB1;
  UARTCount = 2;
  UARTSend( (uint8_t *)UARTBuffer, UARTCount );

  UARTBuffer[0]=0xFF;
    UARTBuffer[1]=0xD7;
    UARTCount = 2;
    UARTSend( (uint8_t *)UARTBuffer, UARTCount );
  //LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;
  while(1);


  //Move cursor to start
//  	UARTBuffer[0] = 0xFF;
//    UARTBuffer[1] = 0xE4;
//    UARTBuffer[2] = 0x00;
//    UARTBuffer[3] = 0x05;
//    UARTBuffer[4] = 0x00;
//    UARTBuffer[5] = 0x03;
//    UARTCount = 6;
//    UARTSend( (uint8_t *)UARTBuffer, UARTCount );
//    UARTCount = 0;
//    //Reenable Interrupts
//    LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;
//
//    while(1)
//    {
//
//    }

//  UARTBuffer[0] = 0xFF;
//  UARTBuffer[1] = 0xCF;
//  UARTSend( (uint8_t *)UARTBuffer, UARTCount );
//  UARTCount = 0;
//  LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;

  //Send letter '9'
  UARTBuffer[0] = 0xFF;
  UARTBuffer[1] = 0xFE;
  UARTBuffer[2] = 0x00;
  UARTBuffer[3] = 0x39;
  UARTCount = 4;

  //Send data
  UARTSend( (uint8_t *)UARTBuffer, UARTCount );
  UARTCount = 0;

  //Reenable Interrupts
  LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;

  while(1)
  {



  }


//  while (1)
//  {				/* Loop forever */
//	if ( UARTCount != 0 )
//	{
//	  LPC_UART->IER = IER_THRE | IER_RLS;			/* Disable RBR */
//	  UARTSend( (uint8_t *)UARTBuffer, UARTCount );
//	  UARTCount = 0;
//	  LPC_UART->IER = IER_THRE | IER_RLS | IER_RBR;	/* Re-enable RBR */
//	}
//  }
}
