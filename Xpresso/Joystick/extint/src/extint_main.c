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

   /* use port2_1 as input event, interrupt test. */
  //GPIOSetDir( PORT2, 1, 0 );	//UP
  GPIOSetDir(PORT1, 0, 0);
  GPIOSetDir(PORT1, 2, 0);

  /* port2_1, single edge trigger, active high. */
  //GPIOSetInterrupt( PORT2, 1, 0, 0, 1 );
  GPIOSetInterrupt( PORT1, 0, 0, 0, 1 );
  GPIOSetInterrupt( PORT1, 2, 0, 0, 1 );
  //GPIOIntEnable( PORT2, 1 );
  GPIOIntEnable( PORT1, 0 );
  GPIOIntEnable( PORT1, 2 );

  /* Set LED port line to output */
  //GPIOSetDir( LED_PORT, LED_BIT, 1 );
  /* Turn LED on */
  //GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );

  //uint32_t derp = GPIOIntStatus(PORT2, 1);
  while( 1 )
  {	  int i = LPC_GPIO0->DATA;
	  printf("Pin 2/1 is: %d",i );

  }
}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
