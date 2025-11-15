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
  #define IBUFFER 256
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
    RTC.getTime(inRTC);
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
      Serial.print(F(" is an existing Directory"));
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
  char sLine[ILINE]= {""};
 
  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F(" : "));
  Serial.println(sLine);
  digitalWrite(LED_BUILTIN, 1);
  if (SD.begin(SDCRD)) {
    File FH1 = SD.open(sLine, FILE_READ);
    if (FH1) {
      while (FH1.available()) {
        Serial.write(FH1.read());
      }
      Serial.print(F("\r\nFilesize: "));
      Serial.print(FH1.size(), DEC);
      Serial.print(F(" Byte"));
      FH1.close();
    } else {
      Serial.print(sLine);
      Serial.println(F(" not found!"));
    } 
    SD.end();
    digitalWrite(LED_BUILTIN, 0);
  }
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
  Serial.println(F(": "));
  if (SD.begin(SDCRD)) {
    digitalWrite(LED_BUILTIN, 1);
    if (!card.init( SDCRD)) {
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
      }
    }
    SD.end();
    digitalWrite(LED_BUILTIN,0);
  }
  return( eVOL );
}  /* end of fnc_VOL */
 
/**************************************************/
/*! \brief fnc_XREC                                 
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_XREC(const char* szCmdLn)
{
   /* place your code here */
  char sLine[ILINE]={""};
  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F(" : "));
  digitalWrite(PIN_LED, 1);  
  if (SD.begin(SDCRD)) {
    File FH1 = SD.open(sLine, FILE_WRITE);
    uint64_t iStrtTi = 0;
    if (FH1) {
      do {
        if (millis() >= (iStrtTi+ 3000)){
          Serial.write(" ", 1);
          // iStrtTi = millis();
        }    
      } while(millis() <= (iStrtTi+ 3000));
      Serial.println(F(" received!"));
    } else {
      Serial.println(F(" can\'t create file"));
    }
    SD.end();
    digitalWrite(PIN_LED,0);
  }
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
  argPathFn( szCmdLn, &sLine[0]);

  Serial.print(F(" : "));
  digitalWrite(PIN_LED, 1);  
  if (SD.begin(SDCRD)) {
    File FH1 = SD.open(sLine, FILE_READ);
    if (FH1) {
      uint64_t iFSize = 0;
      int16_t iCSum, iCrc;
      uint8_t iBlkCnt = 1;
      uint8_t iReTr = 0; 
      bool bTrans = false;
      char inChar;
      uint64_t iStrtTi = millis();
      unsigned char ucBuffer[X_BLOCK_SIZE];
      iFSize = FH1.size();
      Serial.print(iFSize);
      Serial.print(F(" Bytes"));
      while (FH1.available()) {
        while (Serial.readBytes(&inChar, 1) == 0) {  } 
        if (((inChar == NAK) && (!bTrans)) || ((inChar == ACK) && (bTrans))) {
            bTrans= true;
            iCSum = 0;
            iCrc  = 0; 
            iReTr = 0;
            int iRdLen= FH1.read(ucBuffer, X_BLOCK_SIZE);
            if (iRdLen > 0){
              if (iRdLen < X_BLOCK_SIZE) {
                for (int iL= iRdLen; iL<X_BLOCK_SIZE; iL++){
                  ucBuffer[iL]= 0x00;
                } /* end for */
              } /* end if */
              Serial.write((uint8_t) SOH);
              Serial.write((uint8_t) iBlkCnt);
              Serial.write((uint8_t) ~iBlkCnt);
              for (int iL=0; iL<X_BLOCK_SIZE; iL++){
                Serial.write(ucBuffer[iL]);
                uicalcCrc(ucBuffer[iL], iCrc);
                iCSum = iCSum + ucBuffer[iL];
                iCSum = iCSum & 0x00ff;
              } /* end for */
              Serial.write((uint8_t) iCSum);
              iBlkCnt++;
            } else {

            } /* end if */
        } /* end if */
      } /* end while */  
      delay (10);
      Serial.write((uint8_t) EOT);
      delay(10);
      FH1.close();
    } else {
      Serial.print(sLine);
      Serial.println(F(" not found!"));
    } 
    Serial.println(F(" transferred!"));
    SD.end();
    digitalWrite(PIN_LED,0);
  }
  return( eXTRAN );
} /* end of fnc_XTRAN */
 
/**************************************************/
/*! \brief fnc_YTRAN                                
    \param argument string: pointer of char
    \return int- value of token
    \ingroup token_parser */
/**************************************************/
int fnc_YTRAN(const char* szCmdLn)
{
  /* place your code here */
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
 
   iCmdLn= strncmp( szCmdLn, "MD", (iCmdPos>(const size_t)strlen("MD")? iCmdPos: (const size_t)strlen("MD")));
   if (iCmdLn < 0) // is less than MD
   {
      iCmdLn= strncmp( szCmdLn, "DATE", (iCmdPos>(const size_t)strlen("DATE")? iCmdPos: (const size_t)strlen("DATE")));
      if (iCmdLn < 0) // is less than DATE
      {
         iCmdLn= strncmp( szCmdLn, "CLS", (iCmdPos>(const size_t)strlen("CLS")? iCmdPos: (const size_t)strlen("CLS")));
         if (iCmdLn < 0) // is less than CLS
         {
            if (strncmp( szCmdLn, "AUTO", (iCmdPos>(const size_t)strlen("AUTO")? iCmdPos: (const size_t)strlen("AUTO")))== 0)
            {
               iRet= fnc_AUTO(szCmdLn+((const size_t) strlen("AUTO")));
            } else { // not AUTO
               if (strncmp( szCmdLn, "CD", (iCmdPos>(const size_t)strlen("CD")? iCmdPos: (const size_t)strlen("CD")))== 0)
               {
                  iRet= fnc_CD(szCmdLn+((const size_t) strlen("CD")));
               } else { //unknown token
                  iRet= fnc_TokenNotFound(szCmdLn);
               } // End of(2:CD)
            } // End of(1:AUTO)
         } else {
            if (iCmdLn > 0) // is higher than CLS
            {
               if (strncmp( szCmdLn, "CONFIG", (iCmdPos>(const size_t)strlen("CONFIG")? iCmdPos: (const size_t)strlen("CONFIG")))== 0)
               {
                  iRet= fnc_CONFIG(szCmdLn+((const size_t) strlen("CONFIG")));
               } else { // not CONFIG
                  if (strncmp( szCmdLn, "COPY", (iCmdPos>(const size_t)strlen("COPY")? iCmdPos: (const size_t)strlen("COPY")))== 0)
                  {
                     iRet= fnc_COPY(szCmdLn+((const size_t) strlen("COPY")));
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(5:COPY)
               } // End of(4:CONFIG)
            } else {
               if (iCmdLn == 0) // Token CLS found
               {
                  iRet= fnc_CLS(szCmdLn+((const size_t) strlen("CLS")));
               } // End of(3:CLS)
            }
         }
      } else {
         if (iCmdLn > 0) // is higher than DATE
         {
            iCmdLn= strncmp( szCmdLn, "ECHO", (iCmdPos>(const size_t)strlen("ECHO")? iCmdPos: (const size_t)strlen("ECHO")));
            if (iCmdLn < 0) // is less than ECHO
            {
               if (strncmp( szCmdLn, "DEL", (iCmdPos>(const size_t)strlen("DEL")? iCmdPos: (const size_t)strlen("DEL")))== 0)
               {
                  iRet= fnc_DEL(szCmdLn+((const size_t) strlen("DEL")));
               } else { // not DEL
                  if (strncmp( szCmdLn, "DIR", (iCmdPos>(const size_t)strlen("DIR")? iCmdPos: (const size_t)strlen("DIR")))== 0)
                  {
                     iRet= fnc_DIR(szCmdLn+((const size_t) strlen("DIR")));
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(8:DIR)
               } // End of(7:DEL)
            } else {
               if (iCmdLn > 0) // is higher than ECHO
               {
                  if (strncmp( szCmdLn, "FORMAT", (iCmdPos>(const size_t)strlen("FORMAT")? iCmdPos: (const size_t)strlen("FORMAT")))== 0)
                  {
                     iRet= fnc_FORMAT(szCmdLn+((const size_t) strlen("FORMAT")));
                  } else { // not FORMAT
                     if (strncmp( szCmdLn, "HELP", (iCmdPos>(const size_t)strlen("HELP")? iCmdPos: (const size_t)strlen("HELP")))== 0)
                     {
                        iRet= fnc_HELP(szCmdLn+((const size_t) strlen("HELP")));
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(11:HELP)
                  } // End of(10:FORMAT)
               } else {
                  if (iCmdLn == 0) // Token ECHO found
                  {
                     iRet= fnc_ECHO(szCmdLn+((const size_t) strlen("ECHO")));
                  } // End of(9:ECHO)
               }
            }
         } else {
            if (iCmdLn == 0) // Token DATE found
            {
               iRet= fnc_DATE(szCmdLn+((const size_t) strlen("DATE")));
            } // End of(6:DATE)
         }
      }
   } else {
      if (iCmdLn > 0) // is higher than MD
      {
         iCmdLn= strncmp( szCmdLn, "TYPE", (iCmdPos>(const size_t)strlen("TYPE")? iCmdPos: (const size_t)strlen("TYPE")));
         if (iCmdLn < 0) // is less than TYPE
         {
            iCmdLn= strncmp( szCmdLn, "REN", (iCmdPos>(const size_t)strlen("REN")? iCmdPos: (const size_t)strlen("REN")));
            if (iCmdLn < 0) // is less than REN
            {
               if (strncmp( szCmdLn, "PATH", (iCmdPos>(const size_t)strlen("PATH")? iCmdPos: (const size_t)strlen("PATH")))== 0)
               {
                  iRet= fnc_PATH(szCmdLn+((const size_t) strlen("PATH")));
               } else { // not PATH
                  if (strncmp( szCmdLn, "RD", (iCmdPos>(const size_t)strlen("RD")? iCmdPos: (const size_t)strlen("RD")))== 0)
                  {
                     iRet= fnc_RD(szCmdLn+((const size_t) strlen("RD")));
                  } else { //unknown token
                     iRet= fnc_TokenNotFound(szCmdLn);
                  } // End of(14:RD)
               } // End of(13:PATH)
            } else {
               if (iCmdLn > 0) // is higher than REN
               {
                  if (strncmp( szCmdLn, "TEMP", (iCmdPos>(const size_t)strlen("TEMP")? iCmdPos: (const size_t)strlen("TEMP")))== 0)
                  {
                     iRet= fnc_TEMP(szCmdLn+((const size_t) strlen("TEMP")));
                  } else { // not TEMP
                     if (strncmp( szCmdLn, "TIME", (iCmdPos>(const size_t)strlen("TIME")? iCmdPos: (const size_t)strlen("TIME")))== 0)
                     {
                        iRet= fnc_TIME(szCmdLn+((const size_t) strlen("TIME")));
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(17:TIME)
                  } // End of(16:TEMP)
               } else {
                  if (iCmdLn == 0) // Token REN found
                  {
                     iRet= fnc_REN(szCmdLn+((const size_t) strlen("REN")));
                  } // End of(15:REN)
               }
            }
         } else {
            if (iCmdLn > 0) // is higher than TYPE
            {
               iCmdLn= strncmp( szCmdLn, "XREC", (iCmdPos>(const size_t)strlen("XREC")? iCmdPos: (const size_t)strlen("XREC")));
               if (iCmdLn < 0) // is less than XREC
               {
                  if (strncmp( szCmdLn, "VER", (iCmdPos>(const size_t)strlen("VER")? iCmdPos: (const size_t)strlen("VER")))== 0)
                  {
                     iRet= fnc_VER(szCmdLn+((const size_t) strlen("VER")));
                  } else { // not VER
                     if (strncmp( szCmdLn, "VOL", (iCmdPos>(const size_t)strlen("VOL")? iCmdPos: (const size_t)strlen("VOL")))== 0)
                     {
                        iRet= fnc_VOL(szCmdLn+((const size_t) strlen("VOL")));
                     } else { //unknown token
                        iRet= fnc_TokenNotFound(szCmdLn);
                     } // End of(20:VOL)
                  } // End of(19:VER)
               } else {
                  if (iCmdLn > 0) // is higher than XREC
                  {
                     if (strncmp( szCmdLn, "XTRAN", (iCmdPos>(const size_t)strlen("XTRAN")? iCmdPos: (const size_t)strlen("XTRAN")))== 0)
                     {
                        iRet= fnc_XTRAN(szCmdLn+((const size_t) strlen("XTRAN")));
                     } else { // not XTRAN
                        if (strncmp( szCmdLn, "YTRAN", (iCmdPos>(const size_t)strlen("YTRAN")? iCmdPos: (const size_t)strlen("YTRAN")))== 0)
                        {
                           iRet= fnc_YTRAN(szCmdLn+((const size_t) strlen("YTRAN")));
                        } else { //unknown token
                           iRet= fnc_TokenNotFound(szCmdLn);
                        } // End of(23:YTRAN)
                     } // End of(22:XTRAN)
                  } else {
                     if (iCmdLn == 0) // Token XREC found
                     {
                        iRet= fnc_XREC(szCmdLn+((const size_t) strlen("XREC")));
                     } // End of(21:XREC)
                  }
               }
            } else {
               if (iCmdLn == 0) // Token TYPE found
               {
                  iRet= fnc_TYPE(szCmdLn+((const size_t) strlen("TYPE")));
               } // End of(18:TYPE)
            }
         }
      } else {
         if (iCmdLn == 0) // Token MD found
         {
            iRet= fnc_MD(szCmdLn+((const size_t) strlen("MD")));
         } // End of(12:MD)
      }
   }
   return(iRet);
} /* end of function fnSDOS_Parser */ 