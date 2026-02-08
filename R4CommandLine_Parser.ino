/**************************************************/
/*! \file R4CommandLine_Parser.ino
    \brief Arduino UNO-R4 command line interface (CLI)
    \defgroup command line parser
    \author Achim Jenne 25.10.2025
    \sa This code is "AS IS" without warranty or liability. 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed WITHOUT ANY WARRANTY; 
    without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
    See the GNU General Public License for more details.
    */
/**************************************************/

/**************************************************/
/*! \brief Automatic generated enum- commands
    \defgroup token_parser
    \sa datetime_t */
/**************************************************/
enum eCmdSt {eNoToken=0,
      eAUTO, eCD, eCLS, eCONFIG, eCOPY, eDATE, eDEL, eDIR,
      eECHO, eFORMAT, eHELP, eMD, ePATH, eRD, eREN, 
      eTEMP, eTIME, eTYPE, eVER, eVOL, eXREC, eXTRAN,
      eYREC, eYTRAN}; 

/**************************************************/
#include <pins_arduino.h>
#include <arduino.h>
//#include <Wire.h>
#include <I2C_RTC.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
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
char sPath[ILINE]= {"/"};
volatile bool bAuto = false;
volatile int32_t uADC[NADC];
bool bRTC = false;

RTCTime inRTC; 
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

#ifdef DEBUG
  Serial1.begin(115200);
  Serial1.flush();
  Serial1.print(S_CLS);
  Serial1.println(USB_NAME);
  Serial1.println("Debug Port\n>");
#endif

  Serial.print(S_CLS);
  Serial.println(USB_NAME);
  Serial.print(F("CPU- Frequency:   "));
  Serial.print(F_CPU/1000000);
  Serial.println(F(" MHz"));
  analogReadResolution(14);
  //analogReference(AR_INTERNAL_1_5V);
  // analogReadTemp(3.3f);  
  Serial.println((analogRead(PIN_A0)*UVCC), 3);
  RTC.begin();    // Initialize the RTC from SW- build date
  struct tm mytm;
  char sMon[5];
  sscanf( __DATE__, "%3s %2d %4d", &sMon, &day, &year);
  mytm.tm_year = year -1900;
  mytm.tm_mon  = func_MonParser(&sMon[0]) -1;
  mytm.tm_mday = day;
  sscanf( __TIME__, "%02d:%02d:%02d", &hour, &minute, &second);
  mytm.tm_hour = hour;
  mytm.tm_min  = minute;
  mytm.tm_sec  = second;
  inRTC.setTM(mytm);

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
  Serial.print("SD_Init ");
  SPI.beginTransaction(SPISettings((4000000*4), MSBFIRST, SPI_MODE0));
  if (!SD.begin(SDCRD)) {
    Serial.println(F("failed"));
    while (true);
  } else {
    Serial.println(F("OK"));
  }
  SD.end();
  if (beginTimer(10)) {          // Frequency in Hz; timer interrupt
    Serial.println(F("GPT- OK"));
  } else {
    Serial.println(F("GPT- Error"));
  }
   // set date time callback function
  SdFile::dateTimeCallback(setFATDdatetime);
  
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
  static int16_t iLineLen;

  // put your main code here, to run repeatedly:
  if (Serial.available()) { 
    inChar  = (char)Serial.read();
    bEM     = editLine(psLine, inChar);
    iLineLen= strlen(psLine);
    if (bEM) {
      for (int16_t iL=0; iL < iLineLen; iL++) {
        *(psLine+iL) = (char) toupper((int) *(psLine+iL));
      } /* end for */
      iLineLen = 0;
      int iRet= fnSDOS_Parser(psLine); 
      *psLine= 0;
      Serial.print(F("\r\n"));
      Serial.print(sPath);
      Serial.print(F(">"));
#ifdef DEBUG
        Serial1.println();
        Serial1.print(sPath);
        Serial1.print(F(">"));
#endif       
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
      if ((bAuto) && (iLineLen == 0)) {
        Serial.print(" | ");
        for (int iL=0; iL<NADC; iL++){
          Serial.print(uADC[iL]*UVCC, 3);
          Serial.print(" V | ");
        } /* end for */
        Serial.print(F("\r\n"));
        Serial.print(sPath);
        Serial.print(F(">"));      
      }
      bGPT_Flag= false;
    } /* end if */
  } /* end if */
} /* end main loop */