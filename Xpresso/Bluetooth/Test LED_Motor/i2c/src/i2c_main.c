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
#include "shared.h"

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

extern volatile uint32_t I2CCount;
extern volatile uint8_t I2CMasterBuffer[BUFSIZE];
extern volatile uint8_t I2CSlaveBuffer[BUFSIZE];
extern volatile uint32_t I2CMasterState;
extern volatile uint32_t I2CReadLength, I2CWriteLength;

char msg_g[BUFSIZE];
uint32_t msg_count_g;

uint8_t is_running_on_battery_g;

uint16_t x_g;
uint16_t y_g;
uint16_t z_g;

uint32_t write_byte_to_register(uint8_t addr, uint8_t reg, uint8_t value);
uint32_t send_i2c_msg(uint8_t addr, uint8_t reg);
uint8_t read_byte_from_register(uint8_t r_addr, uint8_t w_addr, uint8_t reg);
uint32_t uartConnected();
void process_bl_msg();
void send_bl_msg();
void configure_i2c_devices();
void set_up_i2c();
uint8_t is_running_on_battery();
uint16_t get_current_voltage();
uint16_t get_acc_direction(uint8_t reg);
void update_acc_data();

/*******************************************************************************
 **   Main Function  main()
 *******************************************************************************/
int main (void)
{
	/* Basic chip initialization is taken care of in SystemInit() called
	 * from the startup code. SystemInit() and chip settings are defined
	 * in the CMSIS system_<part family>.c file.
	 */
	printf("I2C to UART for Bluetooth started \n");

	set_up_i2c();
	while(1) {
		// Check the Bluetooth
		while(read_byte_from_register(BL_RAADR, BL_WAADR, LSR) & 0x01) {
			msg_g[msg_count_g] = (char) read_byte_from_register(BL_RAADR, BL_WAADR, RHR);
			msg_count_g++;
			if(msg_g[msg_count_g - 1] == '\0') {
				printf("%s\n", msg_g);
				fflush(stdout);
				process_bl_msg();
				msg_count_g = 0;
			}
		}

		// Check the accelerometer
		update_acc_data();
		printf("x: %d\ty: %d\tz: %d\n", x_g, y_g, z_g);

		// Check the fuel gauge
		if(is_running_on_battery()) {
			printf("%d%%\n", is_running_on_battery_g);
		} else {
			printf("voltage: %d\n", get_current_voltage());
		}


		fflush(stdout);
		// Sleep
		int i;
		for ( i = 0; i < 0x200000; i++ );
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


	// Fuel gage

	//  while(1) {
	//	  printf("%d", read_register(0x02));
	//	  printf("%d\n", read_register(0x03));
	//	  fflush(stdout);
	//  }


	// Configure the I2C and UART


	write_register(LCR, 0x80); // 0x80 to program baudrate
	write_register(DLH, 0x00); // ([14.7456 * 10 ^ 6] / 1) / (115200 * 16) = 8 => 0x0008
	write_register(DLL, 0x08); // The desired baud rate is 115200

	//  write_register(LCR, 0xBF); // access EFR register
	//  write_register(EFR, EFR_ENABLE_ENHANCED_FUNCTIONS); // enable enhanced registers
	//  printf("EFR %d\n", read_register(EFR));

	//  printf("EFR %d\n", read_register(EFR));
	//  write_register(MCR, 0x00);
	//  printf("MCR %d\n", read_register(MCR));
	//  printf("EFR %d\n", read_register(EFR));
	//  write_register(0x06, 0x48);
	//  write_register(0x07, 0x11);
	//  printf("TCR %d\n", read_register(0x06));
	//  printf("TLR %d\n", read_register(0x07));
	write_register(LCR, 0x03); // 8 data bit, 1 stop bit, no parity
	write_register(FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	write_register(FCR, 0x01); // enable FIFO mode
	write_register(IER, 0x01); // enable RHR interrupt
	//  write_register(IOCTRL, 0x01); //

	//  if(!uartConnected()){
	//    assert(0);
	//    };
	//  while(1);
	printf("LSR %d\n", read_register(LSR));
	printf("started\n");
	fflush(stdout);

//	for ( i = 0; i < 0x800000; i++ );

	printf("finished\n");
	fflush(stdout);

	// Try to read something
	//  printf("THR %d\n", read_register(THR));
	printf("IER %d\n", read_register(IER));
	printf("FCR %d\n", read_register(FCR));
	printf("IIR %d\n", read_register(IIR));
	printf("LCR %d\n", read_register(LCR));
	printf("MCR %d\n", read_register(MCR));
	printf("LSR %d\n", read_register(LSR));
	//  printf("MSR %d\n", read_register(MSR));
	//  printf("SPR %d\n", read_register(SPR));
	//  printf("TXLVL %d\n", read_register(TXLVL));
	//  printf("RXLVL %d\n", read_register(RXLVL));
	//  printf("DLAB %d\n", read_register(DLAB));
	//  printf("IODIR %d\n", read_register(IODIR));
	//  printf("IOSTATE %d\n", read_register(IOSTATE));
	//  printf("IOINTMSK %d\n", read_register(IOINTMSK));
	//  printf("IOCTRL %d\n", read_register(IOCTRL));
	//  printf("EFCR %d\n", read_register(EFCR));

	printf("------\n\n");
	msg_count_g = 0;
	while(1) {
		if(read_register(LSR) & 0x01) {
			msg_g[msg_count_g] = (char) read_register(RHR);
			msg_count_g++;
			if(msg_g[msg_count_g - 1] == '\0') {
				process_bl_msg();
				msg_count_g = 0;
			}
			//		  printf("%c", ((char) read_register(RHR)));
			//		  printf("LSR %d\n", read_register(LSR));
			//		  fflush(stdout);
		}
	}

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


//	for ( i = 0; i < 0x20000; i++ );
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

uint32_t send_i2c_msg(uint8_t addr, uint8_t reg) {
	I2CWriteLength = 2 + msg_count_g;
	I2CReadLength = 0;
	I2CMasterBuffer[0] = addr;
	I2CMasterBuffer[1] = reg;
	int i;
	for(i = 0; i < msg_count_g; i++) {
		I2CMasterBuffer[i + 2] = msg_g[i];
	}
	I2CEngine();

	// Assert if the I2C is not in ok mode
	assert(I2CMasterState == I2C_OK);
	for ( i = 0; i < 0x200000; i++ );
	return I2CMasterState;
}

uint32_t write_byte_to_register(uint8_t addr, uint8_t reg, uint8_t value) {
	msg_count_g = 1;
	msg_g[0] = value;
	send_i2c_msg(addr, reg);
	msg_count_g = 0;
	return I2CMasterState;
}

uint8_t read_byte_from_register(uint8_t r_addr, uint8_t w_addr, uint8_t reg) {
	I2CWriteLength = 2;
	I2CReadLength = 1;
	I2CMasterBuffer[0] = w_addr;
	I2CMasterBuffer[1] = reg;
	I2CMasterBuffer[2] = r_addr;
	I2CEngine();

	// Assert if the I2C is not in ok mode
	assert(I2CMasterState == I2C_OK);
	return I2CSlaveBuffer[0];
}

uint32_t uartConnected() {
	/*

     Check that UART is connected and operational.

	 */
	// Perform read/write test to check if UART is working
	const char TEST_CHARACTER = 'H';

	write_register(SPR, TEST_CHARACTER);

	return (read_register(SPR) == TEST_CHARACTER);
}

void process_bl_msg() {
	assert(msg_count_g >= 3); // Ensure that the length is at least 3/enough to cover the opcode "00\0"
	// The first two characters are actually the bluetooth code
	int opcode = 10 * (msg_g[0] - '0');
	opcode = opcode + (msg_g[1] - '0');
	printf("BL received. opcode: %d\n", opcode);
	switch(opcode) {
	case AUTHENTICATE:
		send_i2c_msg(BL_WAADR, THR);
		break;
	default:
		printf("ERR: opcode [%d] from bluetooth receive not valid\n", opcode);
		assert(0);
		break;
	}
}

void set_up_i2c() {
	// Initialize I2C
	uint32_t i;

	if ( I2CInit( (uint32_t)I2CMASTER ) == FALSE )	/* initialize I2c */
	{
		while ( 1 );				/* Fatal error */
	}

	// Check if running on power or battery
	if(is_running_on_battery())
		printf("Device is running on battery\n");
	else
		printf("Device is running on power\n");

	configure_i2c_devices();
	printf("I2C configured\n");
}

void configure_i2c_devices() {
	// Set up Bluetooth
	write_byte_to_register(BL_WAADR, LCR, 0x80); // 0x80 to program baudrate
	write_byte_to_register(BL_WAADR, DLH, 0x00); // ([14.7456 * 10 ^ 6] / 1) / (115200 * 16) = 8 => 0x0008
	write_byte_to_register(BL_WAADR, DLL, 0x08); // The desired baud rate is 115200
	write_byte_to_register(BL_WAADR, LCR, 0x03); // 8 data bit, 1 stop bit, no parity
	write_byte_to_register(BL_WAADR, FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	write_byte_to_register(BL_WAADR, FCR, 0x01); // enable FIFO mode
	write_byte_to_register(BL_WAADR, IER, 0x01); // enable RHR interrupt

	// Set up the Accelerometer
	write_byte_to_register(AC_WAADR, 0x2A, 0x01); // Take the accelerometer out of sleep mode
}

// Return 0 if not running on battery. Else return the % of battery left
uint8_t is_running_on_battery() {
	is_running_on_battery_g = read_byte_from_register(FG_RAADR, FG_WAADR, 0x04);
	return is_running_on_battery_g == 0 ? 0 : 1;
}

uint16_t get_current_voltage() {
	return (read_byte_from_register(FG_RAADR, FG_WAADR, 0x02) << 8) + (read_byte_from_register(FG_RAADR, FG_WAADR, 0x03) << 4);
}

uint16_t get_acc_direction(uint8_t reg) {
	return (read_byte_from_register(AC_RAADR, AC_WAADR, reg) << 4) + (read_byte_from_register(AC_RAADR, AC_WAADR, (reg + 0x01)) >> 4);
}

void update_acc_data() {
	x_g = get_acc_direction(0x01);
	y_g = get_acc_direction(0x03);
	z_g = get_acc_direction(0x05);
}


/******************************************************************************
 **                            End Of File
 ******************************************************************************/
