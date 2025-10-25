# UNO-R4CommandLine_Parser
Arduino UNO-R4: CommandLine_Parser
A simple ASCII commandline- interface (CLI) with prinary File- functions and comfort. Supports VT100-Terminal emulation (like TeraTerm, PUTTY, GTKTerm and other) on the USB-Port.
This software is a helpful basis function set of UNO-R4 and the DS1307-RTC with SD-Card enhancements. It's a nice combination for simple applications.
![IMG_0578_comp](https://github.com/user-attachments/assets/54898bd9-174e-4ba1-9f40-50ecfe74a7fa)
An UNO-R4 minima with SD-Card, Battery bufferd RTC, BME680 and 20x4 LCD on I2C- bus

The software supports the CPU internal RTC function and 256ticks/sec callback function for real-time applications. 
In an equal form there is a 100 Hz GPT- Timer running with own callback.

  
**CD**, **MD**, **RD** - directory handling

**CONFIG** - shows the SPI- configuration

**~~DEL~~** - removing file - is not implemented

**DIR** - shows the directory

**DATE** and **TIME** - get and set time/date value

**TYPE** - list an ASCII-File on your terminal

**CLS** - clear the VT100 screen and set the cursor position top-left

**TEMP** - not implemented

**VER** - shows some software informations

**VOL** - list the SD-Card information

Needed:
-	external RTC- lib. from Manjunath CV
-	the SD-Card library must be available

