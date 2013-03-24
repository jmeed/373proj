/*****************************************************************************
 *   gpiotest.c:  main C entry file for NXP LPC11xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.07  ver 1.00    Preliminary version, first Release
 *
******************************************************************************/
#include "driver_config.h"
#include "target_config.h"

#include "gpio.h"

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{
  /* Basic chip initialization is taken care of in SystemInit() called
   * from the startup code. SystemInit() and chip settings are defined
   * in the CMSIS system_<part family>.c file.
   */

  /* Initialize GPIO (sets up clock) */
  GPIOInit();
  GPIOSetDir( 2, 7, 1 );
  GPIOSetDir( 2, 8, 1 );
   /* use port2_1 as input event, interrupt test. */
  GPIOSetDir( PORT3, 0, 0 );
  /* port2_1, single edge trigger, active high. */
  GPIOSetInterrupt( PORT3, 0, 1, 1, 1 );
  GPIOIntEnable( PORT3, 0 );

  /* Set LED port line to output */
//  GPIOSetDir( LED_PORT, LED_BIT, 1 );
//  /* Turn LED on */
//  GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );

  while( 1 );
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
