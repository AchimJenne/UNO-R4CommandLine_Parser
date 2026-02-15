# UNO-R4CommandLine_Parser
Arduino UNO-R4: CommandLine_Parser
An example of a simple ASCII commandline- interface (CLI) with elementary File- functions. Gives a little more comfort on Commandline. Supports VT100-Terminal emulation (like TeraTerm, PUTTY, GTKTerm and other) on the USB-Port.
This project is a set of helpful functions for the Arduino UNO-R4 with a DS1307-RTC and SD-Card. It's a nice combination for simple User IO and additional applications enhancements.
![IMG_0578_comp](https://github.com/user-attachments/assets/54898bd9-174e-4ba1-9f40-50ecfe74a7fa)
An UNO-R4 minima with SD-Card, Battery bufferd RTC, BME680 and 20x4 LCD on the I2C- bus

The software supports the CPU internal RTC function and 4ticks/sec callback function for real-time applications. 
In an equal form there is a 100 Hz GPT- Timer running with own callback.

**Old-School X/Y-Modem File Transfer**
Most of the Terminal emulations supports X- and Y-Modem File transfer. I think, sometime it's helpful to transfer files to/from the uC without stopping the full uC environment or removing the SD-Card from the uC.

From uC to host: The XModem transfer in 128Byte/Checksum and 1kB/CRC was implemented. YModem is not implemented. Reason: each file must be defined on the commandline - same like XModem. 
Transfer rate: In the 128Byte-Transfermode the transfer rate is more then 20kByte/s and up to 70kByte/s in CRC-Mode.

The YModem transfer from host to uC reached transfer rates 23-60 kByte/s in my tests. The XModem transfer rate is 6-10 kByte/s in 128Byte mode and 20-50 kByte/s in 1k-CRC.
(I don't know why the YModem transfer is as faster as the same XModem 1k-CRC transfer)
The Filetransfer was tested with Tera-Term ((C) 1994-2021 T. Teranishi,TeraTerm Project)
  
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

