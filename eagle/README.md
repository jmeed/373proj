Before you open the file in eagle make sure to go to library->use and add all the files in the lbr directory



Major Links for Documentation

ARM M-0
* LCP 1114 48 pin : http://www.nxp.com/documents/data_sheet/LPC111X.pdf

Power System:
* MAX17043G+U (Fuel Gage): http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Prototyping/MAX17043-MAX17044.pdf 
 * I2C
* MCP73831T (Battery Charger): http://www.sparkfun.com/datasheets/Prototyping/Batteries/MCP73831T.pdf
* TPS61200 (Boost Converter): http://www.sparkfun.com/datasheets/Prototyping/tps61200.pdf
* MIC5205 (LDO) : http://www.micrel.com/_PDF/mic5205.pdf

Screen: 
* uOLED-G2 : http://www.4dsystems.com.au/downloads/microOLED/uOLED-128-G2/Docs/uOLED-128-G2-Datasheet-REV1.pdf
			 Serial Communication documentation: http://www.4dsystems.com.au/downloads/Software/4D-Workshop4-IDE/Docs/Serial/GOLDELOX-SPE-COMMAND-SET-REV1.3.pdf
  * UART

Bluetooth:
* SPI/UART
  
Perf:
* ADXL345 (Accelerometer): http://www.sparkfun.com/datasheets/Sensors/Accelerometer/ADXL345.pdf
  * SPI/I2C
* Vibrator (2 pins) 
  * 1 pin gpio. Other pin gnd
* Joystick : http://www.sparkfun.com/datasheets/Components/Buttons/SF303GJ26-3.pdf
  *  NOTE: I am not sure how to get this on the board as it is big and the board will be covered on both sides.
  *  5 pins GPIO. 1 pin power
