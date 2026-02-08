# UNO-R4CommandLine_Parser
Arduino UNO-R4: CommandLine_Parser
An example of a simple ASCII commandline- interface (CLI) with elementary File- functions. Gives a little more comfort on Commandline. Supports VT100-Terminal emulation (like TeraTerm, PUTTY, GTKTerm and other) on the USB-Port.
This project is a set of helpful functions for the Arduino UNO-R4 with a DS1307-RTC and SD-Card. It's a nice combination for simple User IO and additional applications enhancements.
![IMG_0578_comp](https://github.com/user-attachments/assets/54898bd9-174e-4ba1-9f40-50ecfe74a7fa)
An UNO-R4 minima with SD-Card, Battery bufferd RTC, BME680 and 20x4 LCD on the I2C- bus

The software supports the CPU internal RTC function and 4ticks/sec callback function for real-time applications. 
In an equal form there is a 100 Hz GPT- Timer running with own callback.

The X/Y-Modem Filetransfer is helpful if you want to exchange files on a running System - whenever you can't remove the SD-Card. In my case, the host-software can handle the files via remote control. The X/Y-Modem transfer performance is not high, but 25-60 kBytes/s is OK in my Application.
  
**CD**, **MD**, **RD** - directory handling

**CONFIG** - shows the SPI- configuration

**COPY** - copy a file from => to filename

**DEL** - removing file 

**DIR** - shows the directory

**DATE** and **TIME** - get and set time/date value

**TYPE** - list an ASCII-File on your terminal

**CLS** - clear the VT100 screen and set the cursor position top-left

**TEMP** - not implemented

**VER** - shows some software informations

**VOL** - list the SD-Card information

**XTRAN** - file transfer from RasPi to host via XMODEM protocol (supports XModem 128Byte/ChkSum and 1K/CRC)

**XREC** - file transfer from host to RasPi via XMODEM protocol (supports XModem 128Byte/ChkSum and 1K/CRC)

**YREC** - file transfer from host to RasPi via YMODEM protocol (helpful to save many files on R4-system)

Needed:
-	external RTC- lib. from Manjunath CV
-	the SD-Card library must be available

