/****************************************************************************
 *   $Id:: i2c_main.c 4785 2010-09-03 22:39:27Z nxp21346                    $
 *   Project: NXP LPC11xx I2C example
 *
 *   Description:
 *     This file contains I2C test modules, main entry, to test I2C APIs.
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

#include <stdio.h>
#include <assert.h>
#include "type.h"
#include "i2c.h"

#define WAADR 0x90
#define RAADR 0x91

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
#define DLM        0x01 << 3
#define EFR        0x02 << 3
#define XON1       0x04 << 3
#define XON2       0x05 << 3
#define XOFF1      0x06 << 3
#define XOFF2      0x07 << 3

extern volatile uint32_t I2CCount;
extern volatile uint8_t I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;

uint32_t write_register(uint8_t reg, uint8_t addr);
uint8_t read_register(uint8_t reg, uint8_t addr);

/*******************************************************************************
**   Main Function  main()
*******************************************************************************/
int main (void)
{
	  /* Basic chip initialization is taken care of in SystemInit() called
	   * from the startup code. SystemInit() and chip settings are defined
	   * in the CMSIS system_<part family>.c file.
	   */
	printf("I2C to UART for Bluetooth started\n");
  uint32_t i;

  if ( I2CInit( (uint32_t)I2CMASTER ) == FALSE )	/* initialize I2c */
  {
	while ( 1 );				/* Fatal error */
  }

  /* In order to start the I2CEngine, the all the parameters 
  must be set in advance, including I2CWriteLength, I2CReadLength,
  I2CCmd, and the I2cMasterBuffer which contains the stream
  command/data to the I2c slave device. 
  (1) If it's a I2C write only, the number of bytes to be written is 
  I2CWriteLength, I2CReadLength is zero, the content will be filled 
  in the I2CMasterBuffer. 
  (2) If it's a I2C read only, the number of bytes to be read is 
  I2CReadLength, I2CWriteLength is 0, the read value will be filled 
  in the I2CMasterBuffer. 
  (3) If it's a I2C Write/Read with repeated start, specify the 
  I2CWriteLength, fill the content of bytes to be written in 
  I2CMasterBuffer, specify the I2CReadLength, after the repeated 
  start and the device address with RD bit set, the content of the 
  reading will be filled in I2CMasterBuffer index at 
  I2CMasterBuffer[I2CWriteLength+2]. 
  
  e.g. Start, DevAddr(W), WRByte1...WRByteN, Repeated-Start, DevAddr(R), 
  RDByte1...RDByteN Stop. The content of the reading will be filled 
  after (I2CWriteLength + two devaddr) bytes. */

  /* Write SLA(W), address and one data byte */
//  I2CWriteLength = 6;
//  I2CReadLength = 0;
//  I2CMasterBuffer[0] = PCF8594_ADDR;
//  I2CMasterBuffer[1] = 0x00;		/* address */
//  I2CMasterBuffer[2] = 0x55;		/* Data0 */
//  I2CMasterBuffer[3] = 0xAA;		/* Data1 */
//  I2CMasterBuffer[4] = 0x12;		/* Data0 */
//  I2CMasterBuffer[5] = 0x34;		/* Data1 */
//  I2CEngine();
//
//  /* Be careful with below fixed delay. From device to device, or
//  even same device with different write length, or various I2C clock,
//  below delay length may need to be changed accordingly. Having
//  a break point before Write/Read start will be helpful to isolate
//  the problem. */
//  for ( i = 0; i < 0x200000; i++ );	/* Delay after write */
//
//  for ( i = 0; i < BUFSIZE; i++ )
//  {
//	I2CSlaveBuffer[i] = 0x00;
//  }
  /* Write SLA(W), address, SLA(R), and read one byte back. */
//  I2CWriteLength = 2;
//  I2CReadLength = 4;
//  I2CMasterBuffer[0] = PCF8594_ADDR;
//  I2CMasterBuffer[1] = 0x1C;		/* address */
//  I2CMasterBuffer[2] = PCF8594_ADDR | RD_BIT;

  uint32_t result = 0;

//  // WHO I AM
//  I2CWriteLength = 0;
//  I2CReadLength = 1;
//  I2CMasterBuffer[0] = 0x3A;
//  I2CMasterBuffer[1] = 0x0D;
//  I2CMasterBuffer[2] = 0x3B;
//  result = I2CEngine();

//  for(i = 0; i < I2CReadLength; i++) {
//	  printf("%d", I2CSlaveBuffer[i]);
//  }
  // END WRITE
// Set to active mode
  // BEGIN WRITE


  // Bluetooth


  // Configure the UART Baud rate
  write_register(LCR, 0x80);
  uint8_t f = read_register(LCR, 0x80);
  while(1);

  I2CWriteLength = 3;
  I2CReadLength = 0;
  I2CMasterBuffer[0] = 0x90;
  //0x01
  I2CMasterBuffer[1] = 0x01 << 3;
  I2CMasterBuffer[2] = 0x01;
  result = I2CEngine();
  //0x02
  I2CMasterBuffer[1] = 0x02 << 3;
  I2CMasterBuffer[2] = 0xC0;
  result = I2CEngine();
  //0x03
  I2CMasterBuffer[1] = 0x03 << 3;
  I2CMasterBuffer[2] = 0x80;
  result = I2CEngine();
  //for ( i = 0; i < 0x200000; i++ );

  // Read 0x01
  I2CWriteLength = 2;
  I2CMasterBuffer[0] = 0x90;
  I2CMasterBuffer[1] = 0x01 << 3;
  result = I2CEngine();
  I2CWriteLength = 0;
  I2CReadLength = 1;
  I2CMasterBuffer[0] = 0x91;
  result = I2CEngine();


  for ( i = 0; i < 0x20000; i++ );
  // Read 0x02 -- UNABLE TO CONFIRM THAT THIS WRITE WORKS
 I2CWriteLength = 2;
 I2CReadLength = 0;
 I2CMasterBuffer[0] = 0x90;
 I2CMasterBuffer[1] = 0x02 << 3;
 result = I2CEngine();
 I2CWriteLength = 0;
 I2CReadLength = 1;
 I2CMasterBuffer[0] = 0x91;
 result = I2CEngine();

 // Read 0x03
 I2CWriteLength = 2;
 I2CReadLength = 0;
 I2CMasterBuffer[0] = 0x90;
 I2CMasterBuffer[1] = 0x03 << 3;
 result = I2CEngine();
 I2CWriteLength = 0;
 I2CReadLength = 1;
 I2CMasterBuffer[0] = 0x91;
 result = I2CEngine();









  while(1);

  // Accelerometer



    I2CWriteLength = 3;
    I2CReadLength = 0;
    I2CMasterBuffer[0] = 0x3A;
    I2CMasterBuffer[1] = 0x2A;
    I2CMasterBuffer[2] = 0x01;
    result = I2CEngine();


    // END WRITE


  //while(1);
  while(1) {
	  int i = 0;
	  printf("Got new input\n");


	  // Get X
	  I2CWriteLength = 0;
	  I2CReadLength = 1;
	  I2CMasterBuffer[0] = 0x3A;
	  I2CMasterBuffer[1] = 0x01;
	  I2CMasterBuffer[2] = 0x3B;
	  result = I2CEngine();

	  for(i = 0; i < I2CReadLength; i++) {
		  printf("%d", I2CSlaveBuffer[i]);
	  }


//	  I2CWriteLength = 0;
//	  I2CReadLength = 1;
//	  I2CMasterBuffer[0] = 0x3A;
//	  I2CMasterBuffer[1] = 0x02;
//	  I2CMasterBuffer[2] = 0x3B;
//	  result = I2CEngine();
//
//	  for(i = 0; i < I2CReadLength; i++) {
//		  printf("%d", I2CSlaveBuffer[i]);
//	  }
	  printf("\n");

	  // Busy wait
	  for (i=0; i < 0x200000; i++);

  }
  /* Check the content of the Master and slave buffer */
  while ( 1 );
  return 0;
}


uint32_t write_register(uint8_t reg, uint8_t addr) {
	  I2CWriteLength = 3;
	  I2CReadLength = 0;
	  I2CMasterBuffer[0] = WAADR;
	  I2CMasterBuffer[1] = reg;
	  I2CMasterBuffer[2] = addr;
	  I2CEngine();
	  printf("W reg result: %d\n", I2CMasterState);
	  fflush(stdout);

	  // Assert if the I2C is not in ok mode
	  assert(I2CMasterState == I2C_OK);
	  return I2CMasterState;
}

uint8_t read_register(uint8_t reg, uint8_t addr) {
	  I2CWriteLength = 2;
	  I2CReadLength = 1;
	  I2CMasterBuffer[0] = WAADR;
	  I2CMasterBuffer[1] = reg;
	  I2CMasterBuffer[2] = RAADR;
	  I2CEngine();
	  printf("R reg result: %d\n", I2CMasterState);
	  fflush(stdout);

	  // Assert if the I2C is not in ok mode
	  assert(I2CMasterState == I2C_OK);
	  return I2CSlaveBuffer[0];
}

/******************************************************************************
**                            End Of File
******************************************************************************/
