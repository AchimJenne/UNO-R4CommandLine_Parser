 /**************************************************/
/*! \brief fnc_AUTO                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_AUTO(const char* szCmdLn)
{
    /* place your code here */
    if (bAuto) {
      Serial.print(F(": off"));
      timer_ms.stop();
      bAuto= false;
    } else {
      Serial.print(F(": on"));
      timer_ms.start();
      bAuto = true;
    }
    return( eAUTO );
}  /* end of fnc_AUTO */

/**************************************************/
/*! \brief fnc_CD                                   
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_CD(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE]={""};
  argPathFn( szCmdLn, &sLine[0]);
  Serial.print(" ");

  if (SD.begin( SDCRD)) {
    digitalWrite(LED_BUILTIN, 1);
    File dir = SD.open(sLine);
    if (dir) {
      strcpy(sPath, sLine);
    } else {
      Serial.println();
      Serial.print(sLine);
      Serial.print(F(" not found"));
    }
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  } 
  return( eCD );
}  /* end of fnc_CD */
 
/**************************************************/
/*! \brief fnc_CLS                                  
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_CLS(const char* szCmdLn)
{
   /* place your code here */
   Serial.print(F("\e[0H\e[2J"));
   return( eCLS );
}  /* end of fnc_CLS */
 
/**************************************************/
/*! \brief fnc_COPY                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_COPY(const char* szCmdLn)
{
  /* place your code here */
  File FH1, FH2;
  char sFnTo[ILINE];
  char sFnFrom[ILINE];
  char *psFnTo= &sFnTo[0];
  char s1[ILINE], s2[ILINE];
  #define IBUFFER 1024
  char cBuff[IBUFFER];

  szCmdLn++;
  int iRet= sscanf(szCmdLn,"%s %s", &s1, &s2);
  if (iRet ==2) {
    argPathFn( &s1[0], &sFnFrom[0]);
    if (strncmp(s2, ".", 1) ==0) {
      strncpy(psFnTo, sPath, ILINE);
      strncat(psFnTo, "/", ILINE);
      strncat(psFnTo, (strrchr(s1, '/')+1), ILINE);
      Serial.println();  
      Serial.println(psFnTo);
    } else {
      argPathFn( &s2[0], &sFnTo[0]);
    }
    if (SD.begin( SDCRD))  {
      digitalWrite(LED_BUILTIN, 1);
      FH1 = SD.open(sFnFrom, FILE_READ);
      if (FH1) {
        FH2 = SD.open(sFnTo, FILE_WRITE);
        if (FH2) {
          while (FH1.available() > 0) {
            size_t iLen = FH1.readBytes(cBuff, IBUFFER);
            FH2.write(cBuff, iLen);
          } 
          Serial.print(F(" OK"));
        } else {
          Serial.print(F(" no Desti."));
        }
        FH2.close();
        FH1.close();
      } else {
        Serial.print(sFnFrom);
        Serial.print(F(" no Source"));
      }
      digitalWrite(LED_BUILTIN, 0);
    }
    SD.end();
  }  
  return( eCOPY );
}  /* end of fnc_COPY */
 
/**************************************************/
/*! \brief fnc_CONFIG                               
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_CONFIG(const char* szCmdLn)
{
  /* place your code here */
  if (!SD.begin( SDCRD)) {
     Serial.println(F(": failed"));
  } else {
    digitalWrite(LED_BUILTIN, 1);
    Serial.println(F(": OK"));
    Serial.println(F("SPI-Interface"));

    Serial.print(F("MISO : "));
    Serial.println(PIN_SPI_MISO);
    Serial.print(F("MOSI : "));
    Serial.println(PIN_SPI_MOSI);
    Serial.print(F("SCK  : "));
    Serial.println(PIN_SPI_SCK);
    Serial.print(F("CS   : "));
    Serial.println(PIN_SPI_CS);
  }
  SD.end(); 
  digitalWrite(LED_BUILTIN, 0);
  return( eCONFIG );
}  /* end of fnc_CONFIG */

/**************************************************/
/*! \brief fnc_DATE                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_DATE(const char* szCmdLn)
{
  char sLine[ILINE];
  struct tm mytm;

  /* place your code here */
  RTC.getTime(inRTC);
  mytm= inRTC.getTmTime();
  if (strlen(szCmdLn) >= 8) {
    int16_t iResult= sscanf( szCmdLn,"%02d.%02d.%04d", &day, &mon, &year);   
    if (iResult == 3) {
      mytm.tm_year = year -1900;
      mytm.tm_mon  = mon -1;
      mytm.tm_mday = day;
      inRTC.setTM(mytm);
      RTC.setTime(inRTC);
      if (bRTC){
        EXRTC.setDate((uint8_t) day, (uint8_t) mon, (uint16_t) year);
      }
    }
  } else {
    Serial.print(F(" : "));
    Serial.print(F(" CPU: "));
    // RTC.getTime(inRTC);
    time_t tiUx= inRTC.getUnixTime();
    strftime(sLine, sizeof(sLine), "(%A) %0d.%0m.20%0y", localtime(&tiUx));
    Serial.print(sLine);
  }
  return( eDATE );
}  /* end of fnc_DATE */
 
/**************************************************/
/*! \brief fnc_DEL                                  
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_DEL(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE]={""};
  argPathFn( szCmdLn, &sLine[0]);
  Serial.print(F(" : "));
  if (SD.begin(SDCRD)) {
    if (SD.exists(sLine)) {
      digitalWrite(LED_BUILTIN, 1);
      int16_t iRes= SD.remove(sLine);
      if (iRes>0) {
        Serial.print(F("removed"));
      } else {
        Serial.print(F("not removed!"));
      }
    } else {
      Serial.print(F("not Found"));
    }
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }   
  return( eDEL );
}  /* end of fnc_DEL */
 
/**************************************************/
/*! \brief fnc_DIR                                  
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
void printDirectory(File dir, int numTabs) {
  static File entry;
  while ( entry =  dir.openNextFile()) {
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print("\e[");
      Serial.print(i*2);
      Serial.print("C");
    }
    int iLen = strlen(entry.name());
    Serial.print(entry.name());
    if (entry.isDirectory())   // files have sizes, directories do not
    {
      Serial.print(F("\\\e["));
      if (iLen <= 27) {
        Serial.print(28);
      } else {
        Serial.print(iLen + 2);
      }
      Serial.println(F("G\e[1m<Dir>\e[0m"));
      // printDirectory(entry, numTabs + 1);  // uncommend for recursive funcion call
    } else {
      Serial.print(F("\e["));
      if (iLen <= 23) {
        Serial.print(F("24"));
      } else {
        Serial.print(iLen + 2);
      }
      Serial.print(F("G"));
      Serial.print(entry.size(), DEC);
      Serial.println(F(" Byte"));
   }
    entry.close();
  }
}

int fnc_DIR(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE];
  argPathFn( szCmdLn, &sLine[0]);

  Serial.println(F(" : "));
  if (SD.begin(SDCRD)) {
    digitalWrite(LED_BUILTIN, 1);
    File dir = SD.open(sLine);
    printDirectory(dir, 0);
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }
   return( eDIR );
}  /* end of fnc_DIR */
 
/**************************************************/
/*! \brief fnc_ECHO                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_ECHO(const char* szCmdLn)
{
  /* place your code here */
  File FH1;
  Serial.print(F("\r\n"));
  if (SD.begin(SDCRD))   {
    digitalWrite(LED_BUILTIN, 1);
    FH1 = SD.open(sLogFn, FILE_WRITE);
    if (FH1) {
      Serial.println(szCmdLn+1);
      FH1.println(szCmdLn+1);
      FH1.close();
    } else {
      Serial.print(F("no File ..."));
      Serial.print(sLogFn);
    }
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }
   return( eECHO );
}  /* end of fnc_ECHO */
 
/**************************************************/
/*! \brief fnc_FORMAT                               
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_FORMAT(const char* szCmdLn)
{
  /* place your code here */
  Serial.println(F(" : "));
  if (SD.begin(SDCRD)) {
    digitalWrite(LED_BUILTIN, 1);
    //bool stFS= SD.format(); // 
    Serial.println(F("A \"format\" function is not implemented"));
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }
  return( eFORMAT );
}  /* end of fnc_FORMAT */
 
/**************************************************/
/*! \brief fnc_HELP                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_HELP(const char* szCmdLn)
{
   /* place your code here */
  Serial.println(F(" :"));
  Serial.println(F("AUTO"));
  Serial.println(F("CLS\t clearscreen"));
  Serial.println(F("CONFIG\t display configuration"));
  Serial.println(F("COPY\t copy file; <src> <targ>"));
  Serial.println(F("DATE\t display/set date; format <DD.MM.YYYY>"));
  Serial.println(F("DEL\t delete file"));
  Serial.println(F("DIR\t display directory"));
  Serial.println(F("ECHO\t copy argument into logfile"));
  Serial.println(F("FORMAT\t <func. not available>"));
  Serial.println(F("HELP\t this help informations"));
  Serial.println(F("PATH\t display actual path"));
  Serial.println(F("REN\t rename file; <src> <targ>"));
  Serial.println(F("CD\t change directory"));
  Serial.println(F("MD\t make directory"));
  Serial.println(F("RD\t remove directory"));
  Serial.println(F("TIME\t display/set time; format <hh.mm.ss>"));
  Serial.println(F("TEMP\t display temperature(s)"));
  Serial.println(F("TYPE\t display ASCII-file"));
  Serial.println(F("VER\t display SW- Version"));
  Serial.println(F("VOL\t display SD-Card informations"));
  Serial.println(F("XREC\t XModem- download to uC"));
  Serial.println(F("XTRAN\t XModem- upload"));
  Serial.println(F("YREC\t YModem- download to uC (1kB and CRC)"));
  return( eHELP );
}  /* end of fnc_HELP */
 
/**************************************************/
/*! \brief fnc_MD                                   
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_MD(const char* szCmdLn)
{
   /* place your code here */
  char sLine[ILINE]={""};
  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F(" : "));
  digitalWrite(LED_BUILTIN, 1);
  if (SD.begin(SDCRD)) {
    if (!SD.exists(sLine)) {
      int16_t iRes= SD.mkdir(sLine);
      if (iRes>0) {
        Serial.print(F("Direcory created"));
      } else {
        Serial.print(F("Direcory not created"));
      }
    } else {
      Serial.print(F("Directory exist"));
    }
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }
  return( eMD );
}  /* end of fnc_MD */

/**************************************************/
/*! \brief fnc_PATH                               
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_PATH(const char* szCmdLn)
{
   /* place your code here */
   Serial.print(" : ");
   Serial.print(sPath);
   return( ePATH );
}  /* end of fnc_PROMPT */
 
/**************************************************/
/*! \brief fnc_RD                                   
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_RD(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE]={""};
  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F(" : "));
  digitalWrite(LED_BUILTIN, 1);
  if (SD.begin(SDCRD)) {
    if (SD.exists(sLine)) {
      int16_t iRes= SD.rmdir(sLine);
      if (iRes>0) {
        Serial.print(F("Direcory removed"));
      } else {
        Serial.print(F("Direcory not removed"));
      }
    } else {
      Serial.print(F(" Directory not found"));
    }
    SD.end();
  }
  digitalWrite(LED_BUILTIN, 0);
  return( eRD );
}  /* end of fnc_RD */
 
/**************************************************/
/*! \brief fnc_REN                               
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_REN(const char* szCmdLn)
{
  /* place your code here */
  // char sFrom[ILINE];
  // char sTo[ILINE];
  // char s1[40];
  // char s2[40];
  // Serial.print(F(" : "));
  // szCmdLn++;
  // int iRet= sscanf(szCmdLn,"%s %s", &s1, &s2);
  // if (iRet ==2) {
  //   strcpy(sFrom, sPath);
  //   strcat(sFrom, "/");
  //   strcat(sFrom, s1);
  //   strcpy(sTo, sPath);
  //   strcat(sTo, "/");
  //   strcat(sTo, s2);
  //   if (SD.begin( SDCRD)) {
  //     digitalWrite(LED_BUILTIN, 1);
  //     bool stFS= SD.rename(sFrom, sTo);
  //     if (stFS) {
  //       Serial.print(F("OK"));
  //     } else {
  //       Serial.print(F("Error"));
  //     }
  //     SD.end();
  //     digitalWrite(LED_BUILTIN, 0);
  //   }
  // } /* end if */
  return( eREN );
}  /* end of fnc_RENAME */
 
/**************************************************/
/*! \brief fnc_TEMP                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_TEMP(const char* szCmdLn)
{
  /* place your code here */
  Serial.print(" CPU: ");
  //Serial.print(analogReadTemp(3.3f));
  return( eTEMP );
}  /* end of fnc_TEMP */
 
/**************************************************/
/*! \brief fnc_TIME                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_TIME(const char* szCmdLn)
{
   /* place your code here */   
  char sLine[ILINE];
  struct tm mytm;
  RTC.getTime(inRTC);
  mytm= inRTC.getTmTime(); 

  if (strlen(szCmdLn) >= 3) {
    int iResult= sscanf( szCmdLn, "%02d:%02d:%02d", &hour, &minute, &second);
    if (iResult == 3) {
      mytm.tm_hour= hour;
      mytm.tm_min = minute;
      mytm.tm_sec = second;
      inRTC.setTM(mytm);
      RTC.setTime(inRTC);
      if (bRTC){
        EXRTC.setTime((uint8_t) hour, (uint8_t) minute, (uint8_t) second);
      }
    } 
  } else {
    Serial.print(" : ");
    Serial.print(F(" CPU: "));
    time_t tiUx= inRTC.getUnixTime();
    strftime(sLine, sizeof(sLine), "%0H:%0M:%0S", localtime(&tiUx));
    Serial.print(sLine);
  }
  return( eTIME );
}  /* end of fnc_TIME */
 
/**************************************************/
/*! \brief fnc_TYPE                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_TYPE(const char* szCmdLn)
{
  /* place your code here */
  #define BUFLEN 1024
  /* place your code here */
  char sLine[ILINE]= {""};
  int16_t iBufLen;
  unsigned char ucBuffer[BUFLEN];

  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F("\r\n"));
  digitalWrite(LED_BUILTIN, 1);
  if (SD.begin(SDCRD)) {
    File FH1 = SD.open(sLine, FILE_READ);
    if (FH1) {
      while (FH1.available()) {
        iBufLen= FH1.readBytes(&ucBuffer[0], BUFLEN);
        Serial.write(&ucBuffer[0], iBufLen);
      }
      Serial.print(F("\r\nFilesize: "));
      Serial.print(FH1.size(), DEC);
      Serial.print(F(" Byte"));
      FH1.close();
    } else {
      Serial.print(sLine);
      Serial.println(F(" not found!"));
    } /* end if */ 
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  } /* end if */
   return( eTYPE );
}  /* end of fnc_TYPE */
 
/**************************************************/
/*! \brief fnc_VER                                  
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_VER(const char* szCmdLn)
{
   /* place your code here */
  Serial.print(F(" : "));
  Serial.println(USB_NAME);
  Serial.print(F("CPU- Frequency: "));
  Serial.print(F_CPU/1000000);
  Serial.println(F(" MHz"));

  Serial.print(F("Softwarebuild "));
  Serial.print(__DATE__);
  Serial.print(F(" "));
  Serial.print(__TIME__);
  return( eVER );
}  /* end of fnc_VER */
 
/**************************************************/
/*! \brief fnc_VOL                                  
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_VOL(const char* szCmdLn)
{
  /* place your code here */
  Sd2Card card;
  SdVolume volume;
  uint8_t iRet;
  Serial.println(F(": "));
  if (SD.begin(SDCRD)) {
    digitalWrite(LED_BUILTIN, 1);
    if ((iRet= card.init(SDCRD)) !=0) {
      Serial.print(" ");
      Serial.print(iRet);
      Serial.print(" ");
      Serial.println(F("initialization failed."));
    } else {
      Serial.println(F("Card is present."));
      Serial.print(F("Card type:         "));
      switch (card.type()) {
        case SD_CARD_TYPE_SD1:
          Serial.println(F("SD1"));
          break;
        case SD_CARD_TYPE_SD2:
          Serial.println(F("SD2"));
          break;
        case SD_CARD_TYPE_SDHC:
          Serial.println(F("SDHC"));
          break;
        default:
          Serial.println(F("Unknown"));
      }
      if (!volume.init(card)) {
        Serial.println(F("No partition."));
      } else {
        Serial.print(F("Clusters:          "));
        Serial.println(volume.clusterCount());
        Serial.print(F("Blocks x Cluster:  "));
        Serial.println(volume.blocksPerCluster());
        Serial.print(F("Total Blocks:      "));
        Serial.println(volume.blocksPerCluster() * volume.clusterCount());
        uint32_t volumesize;
        Serial.print(F("Volume type is:    FAT"));
        Serial.println(volume.fatType(), DEC);
        volumesize = volume.blocksPerCluster();
        volumesize *= volume.clusterCount(); 
        volumesize /= 2;                     
        Serial.print(F("Volume size (Mb):  "));
        volumesize /= 1024; 
        Serial.println(volumesize);
      } /* end if */
    } /* end if */
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  } /* end if */
  return( eVOL );
}  /* end of fnc_VOL */
 
/**************************************************/
/*! \brief fnc_XREC                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
#define XENTRY 0
#define XBLKNUM 1
#define XBLKNUMI 2
#define XDATABLK 3
#define XCHKCRC 4
#define XCHKEOT 5
int fnc_XREC(const char* szCmdLn)
{
  /* place your code here */
  #define Y_BSIZE 1024
  #define X_BSIZE 128
  #define Y_TIOUT 10000
  char sLine[ILINE] = {""};
  int32_t iByteCnt = 0, iBlkCnt;
  int8_t iRecState;   // Receiver statemachine
  bool bRecEnd=false;
  unsigned char inChar;
  int16_t iBlkSize;
  unsigned char ucBuffer[Y_BSIZE];
  uint64_t iStrtTi;
  File FH1;
  Serial.print(" : ");
  if (((strlen(szCmdLn)>1)>=1)&& SD.begin(SDCRD)) {
    digitalWrite(PIN_LED, 1);
    argPathFn( szCmdLn, &sLine[0]);
    iBlkCnt = 0;
    iRecState = 0;
    Serial.flush();
    while (!bRecEnd) {
      if (iRecState == XENTRY) {
        Serial.write(NAK);
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) &&(millis()<iStrtTi)) {
          if (SD.exists(sLine)){
            SD.remove(sLine);
          } /* end if */
          FH1 = SD.open(sLine, FILE_WRITE);
          if ( (inChar== STX) && (millis()<iStrtTi) ) {
            iBlkSize= Y_BSIZE;
            iRecState = XBLKNUM; // next is BlockNr. token 
          } else 
          if ( (inChar== SOH) && (millis()<iStrtTi) ) {
            iBlkSize= X_BSIZE;
            iRecState = XBLKNUM; // next is BlockNr. token 
          } else 
          if ( (inChar== EOT)&&(millis()<iStrtTi) ) {
            Serial.write((uint8_t)NAK); // end of transmision
          } else {
            bRecEnd= true;
          } /* end if */
        } else
        if ((iBlkCnt < 5)&&(millis()>=iStrtTi) ) {
          iBlkCnt++;
        } else {
          bRecEnd= true;
        } /* end if */
      } else 
      if (iRecState == XBLKNUM) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) &&(millis()<iStrtTi)) {
          iBlkCnt= inChar;
          iRecState = XBLKNUMI;
        } else {
          bRecEnd= true;
        } /* end if */
      } else 
      if (iRecState == XBLKNUMI) {  // inverse block counter - not used!
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) &&(millis()<iStrtTi)) {
          iRecState = XDATABLK;
        } else {
          bRecEnd= true;
        } /* end if */
      } else
      if (iRecState == XDATABLK) { 
        iStrtTi= millis()+Y_TIOUT;
        while((Serial.readBytes(&ucBuffer[0],iBlkSize)<iBlkSize)&&(millis()<iStrtTi) ) { }  
        if (millis()<iStrtTi) {
          iByteCnt= iByteCnt + iBlkSize;
          FH1.write(&ucBuffer[0], iBlkSize);
          iRecState = XCHKCRC;
        } else {
          bRecEnd= true;
        } /* end if */
      } else
      if (iRecState == XCHKCRC) { // Checksum; Hint: CRC is not supported
        iStrtTi= millis()+Y_TIOUT;
        while ( (Serial.readBytes(&inChar,1)<1) && (millis() < iStrtTi) ) { }
        if (millis()<iStrtTi) {
          Serial.write((uint8_t)ACK);
          iRecState = XCHKEOT; // found Header token 
        } else {
          bRecEnd= true;
        } /* end if */
      } /* end if */
      if (iRecState == XCHKEOT) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) &&(millis()<iStrtTi)) {
          if ((inChar== STX) && (millis()<iStrtTi)) {
            iBlkSize  = Y_BSIZE;
            iRecState = XBLKNUM; // found token for next data block
          } else
          if ((inChar== SOH) && (millis()<iStrtTi)) {
            iBlkSize  = X_BSIZE;
            iRecState = XBLKNUM; // found token for next data block
          } else
          if ((inChar== EOT) && (millis()<iStrtTi)) {
            Serial.write((uint8_t)ACK);
            bRecEnd   = true;
          } else {
            for (int iL= 1; iL<3; iL++){
              Serial.write((uint8_t)CAN);
            }
            bRecEnd= true;
          } /* end if */
        } /* end if */
      } /* end if */
    } /* end while */
    if (iRecState== XCHKEOT) {
      FH1.close();
      Serial.print("\nByte Cnt ");
      Serial.print(iByteCnt);
      Serial.println("\nDone");
    } else {
      for (int iL= 1; iL<3; iL++) {
        Serial.write((uint8_t)CAN);
      } /* end for */
      Serial.print("\nstoped at state ");
      Serial.print(iRecState);
      Serial.print("\nBlock Cnt ");
      Serial.print(iBlkCnt);
      Serial.print("\nByte Cnt ");
      Serial.print(iByteCnt);
    } /* end if */
  } else {
    Serial.println(" missing argument!");
  } /* end if */
  SD.end();
  digitalWrite(PIN_LED, 0);
  return( eXREC );
}  /* end of fnc_XREC */

/**************************************************/
/*! \brief fnc_XTRAN                                
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_XTRAN(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE]={""};
  uint64_t iFSize = 0;
  uint16_t iCSum, iCrc;
  uint16_t iBlkCnt = 1;
  uint8_t iReTr = 0; 
  bool bTiOut= false;
  bool bTrans = false;
  bool bCrc = true;
  char inChar;       
  unsigned char ucBuffer[Y_BSIZE];
  uint64_t iStrtTi;

  Serial.print(F(" : "));
  if (strlen(szCmdLn) > 1) {
    digitalWrite(PIN_LED, 1); 
    argPathFn( szCmdLn, &sLine[0]);
    iStrtTi = millis(); 
    if (SD.begin(SDCRD)) {
      File FH1 = SD.open(sLine, FILE_READ);
      if (FH1 != 0) {
        iFSize = FH1.size();
        Serial.print(iFSize);
        Serial.print(F(" Bytes"));
        while ((FH1.available()!=0)&&(!bTiOut)) {
          while ((Serial.readBytes(&inChar,1)==0) && (millis()<(iStrtTi+ (X_TIMEOUT*12)))) {  } 
          if (millis()>=(iStrtTi+(X_TIMEOUT*12))) {
            for (int iL= 1; iL<3; iL++) {
              Serial.write((uint8_t) CAN);
            } /* end for */
            bTiOut= true;
          } else
          // Datablock is CRC 1kByte
          if (((inChar=='C')&&(!bTrans))||((inChar==ACK)&&(bTrans)&&bCrc)) {
            bCrc = true;
            iStrtTi= millis();
            bTrans= true;
            iReTr = 0;
            int iRdLen= FH1.read(ucBuffer, Y_BSIZE);
            if (iRdLen > 0) {
              if (iRdLen < Y_BSIZE) {
                for (int iL= iRdLen; iL<Y_BSIZE; iL++){
                  ucBuffer[iL]= 0x00;
                } /* end for */
              } /* end if */
              Serial.write((uint8_t) STX);
              Serial.write((uint8_t) iBlkCnt);
              Serial.write((uint8_t) ~iBlkCnt);
              Serial.write(ucBuffer, Y_BSIZE); // block transfer
              iCrc = 0;
              for (int iL=0; iL<Y_BSIZE; iL++){
                iCrc= uicalcCrc(ucBuffer[iL], iCrc);
              } /* end for */
              Serial.write((uint8_t) ((iCrc &0xff00) >>8));
              Serial.write((uint8_t) (iCrc & 0x00ff));
              iBlkCnt++;
              iBlkCnt= iBlkCnt & 0x00ff;
            // } else {
            //   bCrc = false;  // if rest >128 Byte, transfer in 128 byte mode
            } /* end if */
          } else
          // checksum 128 Byte
          if (((inChar==NAK)&&(!bTrans))||((inChar==ACK)&&(bTrans))||!bCrc) {
            bCrc= false;          
            iStrtTi= millis();
            bTrans= true;
            iCSum = 0;
            iReTr = 0;
            int iRdLen= FH1.read(ucBuffer, X_BSIZE);
            if (iRdLen > 0) {
              if (iRdLen < X_BSIZE) {
                for (int iL= iRdLen; iL<X_BSIZE; iL++){
                  ucBuffer[iL]= 0x00;
                } /* end for */
              } /* end if */
              Serial.write((uint8_t) SOH);
              Serial.write((uint8_t) iBlkCnt);
              Serial.write((uint8_t) ~iBlkCnt);
              // Blocktransfer vs. single char transfer
              Serial.write(ucBuffer, X_BSIZE); // block transfer
              for (int iL=0; iL<X_BSIZE; iL++){
                iCSum = iCSum + ucBuffer[iL];
                iCSum = iCSum & 0x00ff;
              } /* end for */
              Serial.write((uint8_t) iCSum);
              iBlkCnt++;
              iBlkCnt= iBlkCnt & 0x00ff;
            } /* end if */
          } else
          if (inChar == CAN){
            for (int iL= 1; iL<3; iL++) {
              Serial.write((uint8_t) CAN);
            } /* end for */
            bTiOut= true;
          } /* end if */
        } /* end while */ 
        Serial.write((uint8_t) EOT);
        while ((Serial.readBytes(&inChar,1)==0) && (millis()<(iStrtTi+ (X_TIMEOUT)))) {  } 
        Serial.write((uint8_t) EOT);
        while ((Serial.readBytes(&inChar,1)==0) && (millis()<(iStrtTi+ (X_TIMEOUT)))) {  } 
        FH1.close();
      } else {
        Serial.print(sLine);
        Serial.println(F(" not found!"));
      } /* end if */ 
      if (!bTiOut){
        Serial.println(F("\ndone!"));
      } else {
        Serial.println(F("\nTimeout!"));
      } /* end if */
      SD.end();
      digitalWrite(PIN_LED,0);
    } else {
      Serial.println("\nno SD-Card!");
    } /* end if */
  } else {
    Serial.println(" no argument!");
  } /* end if */
  return( eXTRAN );
} /* end of fnc_XTRAN */

/**************************************************/
/*! \brief fnc_YREC                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
#define YENTRY 0
#define YBLKNUM 1
#define YBLKNUMI 2
#define YDATABLK 3
#define YFNAMBLK 4
#define YCHKCRC 5
#define YCHKEOT 6

#define YMRET 5

int fnc_YREC(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE] = {0x00};
  int32_t iByteCnt = 0, iByteSum = 0;
  uint16_t iBlkCnt=0;
  uint8_t uiBlkCnt[2];
  int32_t iFileSize= 0;
  int16_t iRecState;   // Receiver statemachine
  uint8_t iChkSum;
  bool bRecEnd= false;
  unsigned char inChar;
  int16_t iBlkSize;
  unsigned char ucBuffer[Y_BSIZE];
  uint64_t iStrtTi;
  File FH1;
  
  Serial.print("\n");
  iBlkCnt = 0;
  iRecState = YENTRY;
  if (SD.begin(SDCRD)) {
    digitalWrite(PIN_LED, 1);
    Serial.flush();
    while (!bRecEnd) {
      if (iRecState == YENTRY) {
        Serial.write("C");    // "C" request for 1kB/CRC transmission 
        iStrtTi= millis() + 3000; 
        while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
          if ( (inChar== STX) && (millis()<iStrtTi) ) {
            iBlkSize  = Y_BSIZE;
            iRecState = YBLKNUM; // found Header token 
          } else 
          if ( (inChar== SOH) && (millis()<iStrtTi) ) {
            iBlkSize  = X_BSIZE;
            iRecState = YBLKNUM; // found Header token 
          } else 
          if ( (inChar== EOT)&&(millis()<iStrtTi) ) {
            Serial.write((uint8_t)NAK); // end of transmision
            delay (10);
            Serial.write((uint8_t)ACK);
            iRecState = 901;
            bRecEnd = true;
          } else {
          } /* end if */
        } else
        if ((iBlkCnt < YMRET) && (millis()>=iStrtTi) ) {
          iBlkCnt++;
        } else {
          iRecState = 90;
        } /* end if */
      } else 
      if (iRecState == YBLKNUM) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&uiBlkCnt[0],2)==2) &&(millis()<iStrtTi)) {
          iBlkCnt   = uiBlkCnt[0];
          if ((iByteCnt==0) && (iBlkCnt == 0)) {
            iRecState = YFNAMBLK;  // Filename in first block 
          } else {
            iRecState = YDATABLK;
          } /* end if */
        } /* end if */
      } else
      if (iRecState == YDATABLK) { 
        iStrtTi= millis()+Y_TIOUT;
        while((Serial.readBytes(&ucBuffer[0], iBlkSize) < iBlkSize) && (millis()<iStrtTi) ) { }  
        if (millis()<iStrtTi) {
          iRecState = YCHKCRC;
        } else {
          iRecState = 93;
        } /* end if */
      } else
      if (iRecState == YFNAMBLK) { // YMODEM Filename block
        iStrtTi = millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&ucBuffer[0], iBlkSize) == iBlkSize) &&(millis()<iStrtTi)) {
          if (strlen((char*)ucBuffer)>=1) {
            argPathFn( (char*)ucBuffer, &sLine[0]);
            iRecState = YCHKCRC;
          } else { // no filename found
            strcpy(sLine,"");
            iRecState  = YCHKCRC;
          } /* end if */
        } else {
          iRecState  = 931;
        } /* end if */
      } else
      if (iRecState == YCHKCRC) { 
        iStrtTi= millis()+Y_TIOUT;
        unsigned char cCrc[2];
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&cCrc[0], 2)==2) &&(millis()<iStrtTi)) {
          if ( millis()<iStrtTi ) {
            uint16_t iMyCRC =0;   // my calculated CRC
            for (int iL=0; iL < iBlkSize; iL++) {
              iMyCRC= uicalcCrc(ucBuffer[iL],iMyCRC);
            } /* end for */
            uint16_t iCRC = (cCrc[0]<<8) + cCrc[1]; // remote CRC
            if ((iByteCnt==0) && (iBlkCnt==0)) { // Req. New file
              if (strlen(sLine)>1) {
                if (!FH1) {               // if Filehandle was closed
                  if (SD.exists(sLine)) { // remove if file exists 
                    SD.remove(sLine);
                  } /* end if */
                  FH1 = SD.open(sLine, (O_WRITE|O_CREAT)); // create file
                }
                Serial.write((uint8_t)ACK);
                delay(10);
                Serial.write("C");  // YModem: reopen connection after Filename
              } else {  // no filename - exit function
                Serial.write((uint8_t)ACK);
                bRecEnd= true;
              } /* end if */
            } else {
              FH1.write(ucBuffer, iBlkSize);
              iByteCnt = iByteCnt + iBlkSize;
              Serial.write((uint8_t)ACK);
            } /* end if */
            iRecState = YCHKEOT; //  
          }  /* end if */
        } else {
          iRecState =94;
        } /* end if */
      } else
      if (iRecState == YCHKEOT) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
          if ((inChar== STX) && (millis()<iStrtTi)) {
            iBlkSize= Y_BSIZE;
            iRecState = YBLKNUM; // get next data block in 1k-CRC mode
          } else
          if ((inChar== SOH) && (millis()<iStrtTi)) {
            iBlkSize= X_BSIZE;
            iRecState = YBLKNUM; // get next data block in 128 Byte- CRC mode
          } else
          if ((inChar== EOT) && (millis()<iStrtTi)) {
            FH1.close();  // close file handle
            if (strlen(sLine)>1) {            
              Serial.write((uint8_t)NAK);
              delay(5);
              Serial.write((uint8_t)ACK);
              delay(5);
              Serial.write("C");
              iStrtTi= millis()+3000;
              while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
              if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
                if ((inChar== SOH) && (millis()<iStrtTi)) { // start of header
                  iBlkSize = X_BSIZE;
                  iByteSum = iByteSum + iByteCnt;
                  iByteCnt = 0;
                  iRecState= YBLKNUM;  // get next file
                } else {
                  iByteSum = iByteSum + iByteCnt;
                  bRecEnd= true;
                } /* end if */
              } /* end if */
            } else {
              iByteSum = iByteSum + iByteCnt;
              Serial.write((uint8_t)ACK);
              delay(5);
              bRecEnd= true;
            } /* end if */
          } /* end if */
        } else {
        for (int iL= 1; iL<3; iL++){
          Serial.write((uint8_t)CAN);
        } /* end for */
        iRecState = 95;
        } /* end if */
      } else
      if (iRecState >= 90) {  // something goes wrong
        for (int iL= 1; iL<3; iL++){
          Serial.write((uint8_t)CAN);
        } /* end for */
        bRecEnd= true;
      } /* end if */
    } /* end while */
    if (iRecState == YCHKEOT) {
      Serial.print("\nByte Cnt ");
      Serial.print(iByteSum);
      Serial.println("\nDone");
    } else {
      Serial.print("\nBlock Cnt ");
      Serial.print(iBlkCnt);
      Serial.print(" stoped at state ");
      Serial.print(iRecState);
      Serial.print("\nByte Cnt ");
      Serial.print(iByteSum);
    } /* end if */
    SD.end();
    digitalWrite(PIN_LED, 0);
  } /* end if */
  return( eYREC );
}  /* end of fnc_YREC */

/**************************************************/
/*! \brief fnc_YTRAN                                
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_YTRAN(const char* szCmdLn)
{
  /* place your code here */
  char sLine[ILINE] = {0x00};
  int32_t iByteCnt = 0, iByteSum = 0;
  uint16_t iBlkCnt=0;
  uint8_t uiBlkCnt[2];
  int32_t iFileSize= 0;
  int16_t iRecState;   // Receiver statemachine
  uint8_t iChkSum;
  bool bRecEnd= false;
  unsigned char inChar;
  int16_t iBlkSize;
  unsigned char ucBuffer[Y_BSIZE];
  uint64_t iStrtTi;
  File FH1;
  
  Serial.print("\n");
  iBlkCnt = 0;
  iRecState = YENTRY;
  if (SD.begin(SDCRD)) {
    digitalWrite(PIN_LED, 1);
    Serial.flush();
    while (!bRecEnd) {
      if (iRecState == YENTRY) {
        Serial.write("C");    // "C" request for 1kB/CRC transmission 
        iStrtTi= millis() + 3000; 
        while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
          if ( (inChar== STX) && (millis()<iStrtTi) ) {
#ifdef DEBUG  
            Serial1.println("STX");
#endif            
            iBlkSize  = Y_BSIZE;
            iRecState = YBLKNUM; // found Header token 
          } else 
          if ( (inChar== SOH) && (millis()<iStrtTi) ) {
#ifdef DEBUG  
            Serial1.println("SOH");
#endif
            iBlkSize  = X_BSIZE;
            iRecState = YBLKNUM; // found Header token 
          } else 
          if ( (inChar== EOT)&&(millis()<iStrtTi) ) {
#ifdef DEBUG  
            Serial1.println("EOT => NAK-+-ACK");
#endif
            Serial.write((uint8_t)NAK); // end of transmision
            delay (10);
            Serial.write((uint8_t)ACK);
            iRecState = 901;
            bRecEnd = true;
          } else {
          } /* end if */
        } else
        if ((iBlkCnt < YMRET) && (millis()>=iStrtTi) ) {
          iBlkCnt++;
        } else {
          iRecState = 90;
        } /* end if */
      } else 
      if (iRecState == YBLKNUM) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&uiBlkCnt[0],2)==2) &&(millis()<iStrtTi)) {
          iBlkCnt   = uiBlkCnt[0];
#ifdef DEBUG  
          Serial1.print("Block Cnt.: 0x");
          Serial1.print(uiBlkCnt[0], HEX);
          Serial1.print(" (~) 0x");
          Serial1.println(uiBlkCnt[1], HEX);
#endif
          if ((iByteCnt==0) && (iBlkCnt == 0)) {
#ifdef DEBUG  
          Serial1.print("Filename\n");
#endif                      
            iRecState = YFNAMBLK;  // Filename in first block 
          } else {
#ifdef DEBUG  
          Serial1.print("Data\n");
#endif
            iRecState = YDATABLK;
          } /* end if */
        } /* end if */
      } else
      if (iRecState == YDATABLK) { 
        iStrtTi= millis()+Y_TIOUT;
        while((Serial.readBytes(&ucBuffer[0], iBlkSize) < iBlkSize) && (millis()<iStrtTi) ) { }  
        if (millis()<iStrtTi) {
          iRecState = YCHKCRC;
        } else {
          iRecState = 93;
        } /* end if */
      } else
      if (iRecState == YFNAMBLK) { // YMODEM Filename block
        iStrtTi = millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&ucBuffer[0], iBlkSize) == iBlkSize) &&(millis()<iStrtTi)) {
          if (strlen((char*)ucBuffer)>=1) {
            argPathFn( (char*)ucBuffer, &sLine[0]);
#ifdef DEBUG            
            Serial1.print("NameBlock (0): ");
            Serial1.print(iBlkCnt);
            Serial1.print(" Filename: ");
            Serial1.print(sLine);
            Serial1.print(" ");
            unsigned char *psL;
            int iLen= strlen((char*) ucBuffer);
            iLen++;
            psL= &ucBuffer[iLen];
            // Serial1.println((char*)psL);
            sscanf((char*)psL,"%ld ", &iFileSize);
            Serial1.print(iFileSize);
            Serial1.print("Byte\n");
#endif            
            iRecState = YCHKCRC;
          } else { // no filename found
            strcpy(sLine,"");
#ifdef DEBUG
            Serial1.println("Stop: No Filename");
            Serial1.print("BlkNr.: ");
            Serial1.println(iBlkCnt);
            for (int16_t iL=0; iL<iBlkSize; iL++) {
              if ((iL%16)==0) Serial1.println();
              Serial1.print(" 0x");
              Serial1.print (ucBuffer[iL], HEX);
            }
            Serial1.println();
#endif            
            iRecState  = YCHKCRC;
          } /* end if */
        } else {
          iRecState  = 931;
        } /* end if */
      } else
      if (iRecState == YCHKCRC) { 
        iStrtTi= millis()+Y_TIOUT;
        unsigned char cCrc[2];
        while ( (!Serial.available()) && (millis() < iStrtTi) ) { }
        if ((Serial.readBytes(&cCrc[0], 2)==2) &&(millis()<iStrtTi)) {
          if ( millis()<iStrtTi ) {
            uint16_t iMyCRC =0;
            for (int iL=0; iL < iBlkSize; iL++) {
              iMyCRC= uicalcCrc(ucBuffer[iL],iMyCRC);
            } /* end for */
            uint16_t iCRC = (cCrc[0]<<8) + cCrc[1]; // big endian
#ifdef DEBUG
            Serial1.print("Re.CRC: 0x");
            Serial1.print(iCRC, HEX);
            Serial1.print(" MyCRC 0x");
            Serial1.print(iMyCRC, HEX);
            Serial1.println();
#endif               
            if ((iByteCnt==0)&&(iBlkCnt==0)) {
              if (strlen(sLine)>1) {
                if (!FH1) {
                  if (SD.exists(sLine)) { // remove if file exists 
                    SD.remove(sLine);
                    Serial1.print("File-Remove ");
                  } /* end if */
                  FH1 = SD.open(sLine, (O_WRITE|O_CREAT)); // create file
#ifdef DEBUG
                  Serial1.print("File-Open ");
                  Serial1.println(sLine);
#endif                  
                }
                Serial.write((uint8_t)ACK);
                delay(10);
                Serial.write("C");  // YModem: reopen connection after Filename
              } else {  // no filename - exit function
                Serial.write((uint8_t)ACK);
                bRecEnd= true;
              } /* end if */
            } else {
              FH1.write(ucBuffer, iBlkSize);
              iByteCnt = iByteCnt + iBlkSize;
              Serial.write((uint8_t)ACK);
            } /* end if */
            iRecState = YCHKEOT; //  
          }  /* end if */
        } else {
          iRecState =94;
        } /* end if */
      } else
      if (iRecState == YCHKEOT) { 
        iStrtTi= millis()+Y_TIOUT;
        while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
        if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
          if ((inChar== STX) && (millis()<iStrtTi)) {
#ifdef DEBUG
            Serial1.println("STX-Next");
#endif            
            iBlkSize= Y_BSIZE;
            iRecState = YBLKNUM; // get next data block in 1k-CRC mode
          } else
          if ((inChar== SOH) && (millis()<iStrtTi)) {
#ifdef DEBUG
            Serial1.println("SOH-Next");
#endif
            iBlkSize= X_BSIZE;
            iRecState = YBLKNUM; // get next data block in 128 Byte- CRC mode
          } else
          if ((inChar== EOT) && (millis()<iStrtTi)) {
#ifdef DEBUG
            Serial1.print("Close File: ");
            Serial1.println(sLine);
#endif            
            FH1.close();  // close file handle
            if (strlen(sLine)>1) {            
              Serial.write((uint8_t)NAK);
              delay(5);
              Serial.write((uint8_t)ACK);
              delay(5);
              Serial.write("C");
              iStrtTi= millis()+3000;
              while ( (!Serial.available()) && (millis()<iStrtTi) ) { }
              if ((Serial.readBytes(&inChar,1)==1) && (millis()<iStrtTi)) {
                if ((inChar== SOH) && (millis()<iStrtTi)) { // start of header
                  iBlkSize = X_BSIZE;
                  iByteSum = iByteSum + iByteCnt;
                  iByteCnt = 0;
                  iRecState= YBLKNUM;  // get next file
                } else {
                  iByteSum = iByteSum + iByteCnt;
                  bRecEnd= true;
                } /* end if */
              } /* end if */
            } else {
              iByteSum = iByteSum + iByteCnt;
              Serial.write((uint8_t)ACK);
              delay(5);
              bRecEnd= true;
            } /* end if */
          } /* end if */
        } else {
        for (int iL= 1; iL<3; iL++){
          Serial.write((uint8_t)CAN);
        } /* end for */
        iRecState = 95;
        } /* end if */
      } else
      if (iRecState >= 90) {  // something goes wrong
        for (int iL= 1; iL<3; iL++){
          Serial.write((uint8_t)CAN);
        } /* end for */
        bRecEnd= true;
      } /* end if */
    } /* end while */
    if (iRecState == YCHKEOT) {
      Serial.print("\nByte Cnt ");
      Serial.print(iByteSum);
      Serial.println("\nDone");
    } else {
      Serial.print("\nBlock Cnt ");
      Serial.print(iBlkCnt);
      Serial.print(" stoped at state ");
      Serial.print(iRecState);
      Serial.print("\nByte Cnt ");
      Serial.print(iByteSum);
    } /* end if */
    SD.end();
    digitalWrite(PIN_LED, 0);
  } /* end if */
  return( eYTRAN );
}  /* end of fnc_YTRAN */ 

/**************************************************/
/*! \brief fnc_TokenNotFound
    \param argument string: pointer of char
    \return always 0
    \ingroup token_parser */
/**************************************************/
int fnc_TokenNotFound(const char* szCmdLn)
{
   /* place your code here */
   Serial.print(F(" <?>"));
   return( eNoToken );
}  /* end of fnc_TokenNotFound */
 
 
/**************************************************/
/*! \brief parser main function
     fnSDOS_Parser                                  
    \param szCmdLn is a pointer of char Cmd-Line
    \return int value of token; 0: no token found
    \ingroup token_parser
    \sa datetime_t */
/**************************************************/
int fnSDOS_Parser(char *szCmdLn)
{
   int iCmdLn;
   int iCmdPos;
   int iRet;
 
   iCmdPos= strcspn(szCmdLn," ");
   if (iCmdPos <= 0) iCmdPos= strlen(szCmdLn);
 
   iCmdLn= strncmp( szCmdLn, "MD", (iCmdPos >= sizeof("MD"))? iCmdPos: sizeof("MD")-1);
   if (iCmdLn < 0) { // is less than MD
      iCmdLn= strncmp( szCmdLn, "DATE", (iCmdPos >= sizeof("DATE"))? iCmdPos: sizeof("DATE")-1);
      if (iCmdLn < 0) { // is less than DATE
         iCmdLn= strncmp( szCmdLn, "CLS", (iCmdPos >= sizeof("CLS"))? iCmdPos: sizeof("CLS")-1);
         if (iCmdLn < 0) { // is less than CLS
            if (strncmp( szCmdLn, "AUTO", (iCmdPos >= sizeof("AUTO"))? iCmdPos: sizeof("AUTO")-1)==0) {
               iRet= fnc_AUTO(szCmdLn+sizeof("AUTO")-1);
            } else { // not AUTO
               if (strncmp( szCmdLn, "CD", (iCmdPos >= sizeof("CD"))? iCmdPos: sizeof("CD")-1)==0) {
                  iRet= fnc_CD(szCmdLn+sizeof("CD")-1);
               } else { //unknown token
                  iRet= fnc_TokenNotFound(szCmdLn);
               } // End of(2:CD)
            } // End of(1:AUTO)
         } else {
            if (iCmdLn > 0) { // is higher than CLS
               if (strncmp( szCmdLn, "CONFIG", (iCmdPos >= sizeof("CONFIG"))? iCmdPos: sizeof("CONFIG")-1)==0) {
                  iRet= fnc_CONFIG(szCmdLn+sizeof("CONFIG")-1);
               } else { // not CONFIG
                  if (strncmp( szCmdLn, "COPY", (iCmdPos >= sizeof("COPY"))? iCmdPos: sizeof("COPY")-1)==0) {
                     iRet= fnc_COPY(szCmdLn+sizeof("COPY")-1);
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(5:COPY)
               } // End of(4:CONFIG)
            } else {
               if (iCmdLn == 0) { // Token CLS found
                  iRet= fnc_CLS(szCmdLn+sizeof("CLS")-1);
               } // End of(3:CLS)
            }
         }
      } else {
         if (iCmdLn > 0) { // is higher than DATE
            iCmdLn= strncmp( szCmdLn, "ECHO", (iCmdPos >= sizeof("ECHO"))? iCmdPos: sizeof("ECHO")-1);
            if (iCmdLn < 0) { // is less than ECHO
               if (strncmp( szCmdLn, "DEL", (iCmdPos >= sizeof("DEL"))? iCmdPos: sizeof("DEL")-1)==0) {
                  iRet= fnc_DEL(szCmdLn+sizeof("DEL")-1);
               } else { // not DEL
                  if (strncmp( szCmdLn, "DIR", (iCmdPos >= sizeof("DIR"))? iCmdPos: sizeof("DIR")-1)==0) {
                     iRet= fnc_DIR(szCmdLn+sizeof("DIR")-1);
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(8:DIR)
               } // End of(7:DEL)
            } else {
               if (iCmdLn > 0) { // is higher than ECHO
                  if (strncmp( szCmdLn, "FORMAT", (iCmdPos >= sizeof("FORMAT"))? iCmdPos: sizeof("FORMAT")-1)==0) {
                     iRet= fnc_FORMAT(szCmdLn+sizeof("FORMAT")-1);
                  } else { // not FORMAT
                     if (strncmp( szCmdLn, "HELP", (iCmdPos >= sizeof("HELP"))? iCmdPos: sizeof("HELP")-1)==0) {
                        iRet= fnc_HELP(szCmdLn+sizeof("HELP")-1);
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(11:HELP)
                  } // End of(10:FORMAT)
               } else {
                  if (iCmdLn == 0) { // Token ECHO found
                     iRet= fnc_ECHO(szCmdLn+sizeof("ECHO")-1);
                  } // End of(9:ECHO)
               }
            }
         } else {
            if (iCmdLn == 0) { // Token DATE found
               iRet= fnc_DATE(szCmdLn+sizeof("DATE")-1);
            } // End of(6:DATE)
         }
      }
   } else {
      if (iCmdLn > 0) { // is higher than MD
         iCmdLn= strncmp( szCmdLn, "TYPE", (iCmdPos >= sizeof("TYPE"))? iCmdPos: sizeof("TYPE")-1);
         if (iCmdLn < 0) { // is less than TYPE
            iCmdLn= strncmp( szCmdLn, "REN", (iCmdPos >= sizeof("REN"))? iCmdPos: sizeof("REN")-1);
            if (iCmdLn < 0) { // is less than REN
               if (strncmp( szCmdLn, "PATH", (iCmdPos >= sizeof("PATH"))? iCmdPos: sizeof("PATH")-1)==0) {
                  iRet= fnc_PATH(szCmdLn+sizeof("PATH")-1);
               } else { // not PATH
                  if (strncmp( szCmdLn, "RD", (iCmdPos >= sizeof("RD"))? iCmdPos: sizeof("RD")-1)==0) {
                     iRet= fnc_RD(szCmdLn+sizeof("RD")-1);
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(14:RD)
               } // End of(13:PATH)
            } else {
               if (iCmdLn > 0) { // is higher than REN
                  if (strncmp( szCmdLn, "TEMP", (iCmdPos >= sizeof("TEMP"))? iCmdPos: sizeof("TEMP")-1)==0) {
                     iRet= fnc_TEMP(szCmdLn+sizeof("TEMP")-1);
                  } else { // not TEMP
                     if (strncmp( szCmdLn, "TIME", (iCmdPos >= sizeof("TIME"))? iCmdPos: sizeof("TIME")-1)==0) {
                        iRet= fnc_TIME(szCmdLn+sizeof("TIME")-1);
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(17:TIME)
                  } // End of(16:TEMP)
               } else {
                  if (iCmdLn == 0) { // Token REN found
                     iRet= fnc_REN(szCmdLn+sizeof("REN")-1);
                  } // End of(15:REN)
               }
            }
         } else {
            if (iCmdLn > 0) { // is higher than TYPE
               iCmdLn= strncmp( szCmdLn, "XREC", (iCmdPos >= sizeof("XREC"))? iCmdPos: sizeof("XREC")-1);
               if (iCmdLn < 0) { // is less than XREC
                  if (strncmp( szCmdLn, "VER", (iCmdPos >= sizeof("VER"))? iCmdPos: sizeof("VER")-1)==0) {
                     iRet= fnc_VER(szCmdLn+sizeof("VER")-1);
                  } else { // not VER
                     if (strncmp( szCmdLn, "VOL", (iCmdPos >= sizeof("VOL"))? iCmdPos: sizeof("VOL")-1)==0) {
                        iRet= fnc_VOL(szCmdLn+sizeof("VOL")-1);
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(20:VOL)
                  } // End of(19:VER)
               } else {
                  if (iCmdLn > 0) { // is higher than XREC
                     iCmdLn= strncmp( szCmdLn, "YREC", (iCmdPos >= sizeof("YREC"))? iCmdPos: sizeof("YREC")-1);
                     if (iCmdLn == 0) {
                        iRet= fnc_YREC(szCmdLn+sizeof("YREC")-1);
                     } else { // not YREC
                        if (iCmdLn < 0) {
                           iCmdLn= strncmp( szCmdLn, "XTRAN", (iCmdPos >= sizeof("XTRAN"))? iCmdPos: sizeof("XTRAN")-1);
                           if (iCmdLn == 0) {
                              iRet= fnc_XTRAN(szCmdLn+sizeof("XTRAN")-1);
                           } else { //unknown token
                              iRet= fnc_TokenNotFound(szCmdLn);
                           }
                        } else {
                           if (strncmp( szCmdLn, "YTRAN", (iCmdPos >= sizeof("YTRAN"))? iCmdPos: sizeof("YTRAN")-1)==0) {
                              iRet= fnc_YTRAN(szCmdLn+sizeof("YTRAN")-1);
                           } else { //unknown token
                              iRet= fnc_TokenNotFound(szCmdLn);
                           } // End of(24:YTRAN)
                        } // End of(22:XTRAN)
                     } // End of(23:YREC)
                  } else {
                     if (iCmdLn == 0) { // Token XREC found
                        iRet= fnc_XREC(szCmdLn+sizeof("XREC")-1);
                     } // End of(21:XREC)
                  }
               }
            } else {
               if (iCmdLn == 0) { // Token TYPE found
                  iRet= fnc_TYPE(szCmdLn+sizeof("TYPE")-1);
               } // End of(18:TYPE)
            }
         }
      } else {
         if (iCmdLn == 0) { // Token MD found
            iRet= fnc_MD(szCmdLn+sizeof("MD")-1);
         } // End of(12:MD)
      }
   }
   return(iRet);
} /* end of function fnSDOS_Parser */