/**************************************************/
/*! \file PicoCommandLine_Parser.ino
    \brief Raspberry pico command line interface (CLI)
    \defgroup command line parser
    \author Achim Jenne 25.10.2025
    \sa This code is "AS IS" without warranty or liability. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.. 
    */
/**************************************************/

/**************************************************/
/*! \brief Automatic generated enum- commands
    \defgroup token_parser
    \sa datetime_t */
/**************************************************/
enum eCmdSt {eNoToken=0,
      eAUTO, eCD, eCLS, eCONFIG, eCOPY, eDATE, eDEL, eDIR,
      eECHO, eFORMAT, eHELP, eMD, ePATH, eRD, eREN, eTEMP,
      eTIME, eTYPE, eVER, eVOL, eXREC, eXTRAN, eYTRAN}; 

/**************************************************/
#include <pins_arduino.h>
#include <arduino.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "I2C_RTC.h"
#include <SPI.h>
#include <SD.h>
#include "SD-OS_R4.h"

// Include the RTC library
#include "RTC.h"
#include "FspTimer.h"
FspTimer timer_ms;
/**************************************************/
volatile bool bRTC_Flag = false;
volatile bool bGPT_Flag = false;

static bool bLED;
static bool bGPT;
char sLogFn[40]= "start.txt";
char sPath[ILINE]= {""};
volatile bool bAuto = false;
bool bRTC = false;

RTCTime inRTC; // (24, Month::OCTOBER, 2025, 10, 00, 00, DayOfWeek::FRIDAY, SaveLight::SAVING_TIME_ACTIVE);
int day, mon, year;
int hour, minute, second;
static DS1307 EXRTC;

/**************************************************/
void setup() {
  char sLine[ILINE];
  Sd2Card card;
  Serial.begin(115200);
  // wait on Serial to be available 
  while (!Serial){
    delay(10);
  } 
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_TX, OUTPUT);
  pinMode(LED_RX, OUTPUT);
  analogReadResolution(14);
  //analogReadTemp(3.3f);

  Serial.print(S_CLS);

  Serial.println(USB_NAME);
  Serial.print(F("CPU- Frequency:   "));
  Serial.print(F_CPU/1000000);
  Serial.println(F(" MHz"));

  RTC.begin();    // Initialize the RTC- interface
  struct tm mytm;
  mytm.tm_year = 2025 -1900;
  mytm.tm_mon = 11 -1;
  mytm.tm_mday = 7;
  sscanf( __TIME__, "%02d:%02d:%02d", &hour, &minute, &second);
  mytm.tm_hour = hour;
  mytm.tm_min  = minute;
  mytm.tm_sec  = second;
  
  inRTC.setTM(mytm);
  // inRTC.setTM(mytm);
  RTC.setTime(inRTC); // Set the initial time 
  if (!RTC.isRunning()) {
      Serial.println("RTC not running");
      while (true){}
  } else {
    Serial.println("RTC OK");
  }
  
  Serial.print(F("extRTC "));
  bool bRet= EXRTC.begin();
  if (EXRTC.isRunning()) {
    Serial.println(F("OK"));
    EXRTC.setHourMode(CLOCK_H24);
    synToExtRTC();
    bRTC= true;
  } else {
    Serial.println(F("failed"));
    bRTC= false;
  }

  //SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  if (!SD.begin(SDCRD)) {
    Serial.println(F("SD initialization failed."));
    while (true);
  } else {
    Serial.println(F("SD init OK."));
  }
  SD.end();
  if (beginTimer(10)) {          // 10 Hz = 100 ms timer interrupt
    Serial.println(F("GPT- OK"));
  } else {
    Serial.println(F("GPT- Error"));
  }
  if (!RTC.setPeriodicCallback(periodicCallback, Period::N4_TIMES_EVERY_SEC)) {
    Serial.println(F("ERROR: periodic callback is wrong"));
    while (true);
  }
  Serial.print(sPath);
  Serial.print(F(">"));
}

/**************************************************/
void loop() {
  bool bEM;
  char inChar;
  static char sLine[ILINE]; 
  char *psLine= &sLine[0];

  // put your main code here, to run repeatedly:
  if (Serial.available()) { 
    inChar = (char)Serial.read();
    if (bEM= editLine(psLine, inChar)) {
      /************************************************************/
      for (int iL=0; iL <strlen(psLine); iL++) {
        *(psLine+iL) = (char) toupper((int) *(psLine+iL));
      } /* end for */
      int iRet= fnSDOS_Parser(psLine);
      //Serial.print(psLine);
      *psLine= 0;
      Serial.print(F("\r\n"));
      Serial.print(sPath);
      Serial.print(F(">"));
    } /* end if */
  } else {
    if (bRTC_Flag) {
      if (bLED) {
        bLED= false;
      } else {
        bLED= true;
      } /* end if */
      digitalWrite(LED_RX, bLED);
      bRTC_Flag= false;
    } /* end if */
    if (bGPT_Flag) {
      if (bGPT) {
        bGPT= false;
      } else {
        bGPT= true;
      } /* end if */
      digitalWrite(LED_TX, bLED);
      bGPT_Flag= false;
    } /* end if */
  } /* end if */
} /* end main loop */