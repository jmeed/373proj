uart
=====================
Demonstrates serial communications. Echos back characters received at
115200 baud. On the Embedded Artists LPCXpresso Baseboard, the UART is
connected to a USB-to-Serial IC. To set up for this demo, you need to
install FDTI drivers for the serial port. These should be included with your
EA baseboard or downloadable from http://www.ftdichip.com/FTDrivers.htm
You will also need to remove the two jumpers from J54. This disables PC
control of the reset and ISP pins so that the micro can run normally
with a PC connected to the serial port.   