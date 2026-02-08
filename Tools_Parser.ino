#include "utility/SdFat.h"
/****************************************************************************/
/**
  * @brief copy external RTC-values into internal RTC
**/
void synToExtRTC()
{
  /* epoch calculation from the time.h  */
  struct tm t;
  t.tm_year = EXRTC.getYear() - 1900;  // Year - 1900
  t.tm_mon  = EXRTC.getMonth() - 1;    // Month, where 0 = jan
  t.tm_mday = EXRTC.getDay();
  t.tm_hour = EXRTC.getHours();
  t.tm_min  = EXRTC.getMinutes();
  t.tm_sec  = EXRTC.getSeconds();
  RTCTime mytime;
  mytime.setTM(t);
  mytime.setSaveLight(SaveLight::SAVING_TIME_ACTIVE);
  RTC.setTime(mytime);
}


/**************************************************/
/**
  * @brief FAT real time callback
**/
// call back for file timestamps
void setFATDdatetime(uint16_t* date, uint16_t* time) {
  RTC.getTime(inRTC);
  struct tm mytm= inRTC.getTmTime();
  // return date 
  *date = (((mytm.tm_year-80)<<9)|((mytm.tm_mon+1)<<5)|(mytm.tm_mday));
  // return time 
  *time = ((mytm.tm_hour<<11)|(mytm.tm_min<<5)|(mytm.tm_sec>>1));
}

/**************************************************/
/**
  * @brief periodic real time clock event/callback
**/
void periodicCallback()
{
  static uint16_t cntRTC = 0; // set value once 
  cntRTC++;
  if ( cntRTC >= 2){
    cntRTC = 0;
    bRTC_Flag = true;
  }
}

/****************************************************************************/
/**
  * @brief GPT- Timer callback method 
**/
void timer_ms_callback(timer_callback_args_t __attribute((unused)) *p_args) {
  static uint16_t cntGPT= 0;
  static int iAvrgCnt=0;
  static int iADC[10][NADC];
  cntGPT++;
  if (cntGPT >= 5){
    bGPT_Flag = true;
    cntGPT=0;
  } /* end if */

  for (int8_t iLADC=0; iLADC<NADC; iLADC++){
    iADC[iAvrgCnt][iLADC]= analogRead(PIN_A0 + iLADC);
    uADC[iLADC] = 0;
    for (int8_t iLAVRG=0; iLAVRG<=9; iLAVRG++){
      uADC[iLADC] += iADC[iLAVRG][iLADC];
    } /* end for */
  } /* end for */
  iAvrgCnt= (iAvrgCnt++)%10;
}
/****************************************************************************/
/**
  * @brief GPT- Timer setup  
**/
bool beginTimer(float rate) {
  bool bReturn;
  uint8_t timer_type = GPT_TIMER;
  int8_t tindex = FspTimer::get_available_timer(timer_type);
  if (tindex < 0){
    tindex = FspTimer::get_available_timer(timer_type, true);
  } /* end if */
  if (tindex >= 0) {
    FspTimer::force_use_of_pwm_reserved_timer();
    if (timer_ms.begin(TIMER_MODE_PERIODIC, timer_type, tindex, rate, 0.0f, timer_ms_callback)) 
    {
      if (timer_ms.setup_overflow_irq()) {
        if (timer_ms.open()) {
          if (timer_ms.start()) {
            bReturn = true;
          } /* end if */
        } /* end if */
      }/* end if */
    } else {
      bReturn = false;
    } /* end if */
  } else {
    bReturn = false;
  } /* end if */
  return (bReturn);
}
/**************************************************/
/*! \brief parser main function
     func_MonParser                                 
    \param szCmdLn is a pointer of char Cmd-Line
    \return int value of token; 0: no token found
    \ingroup token_parser
    \sa datetime_t */
/**************************************************/
int func_MonParser(char *szCmdLn)
{
   int iCmdLn;
   int iCmdPos;
   int iRet;
 
   iCmdPos= strcspn(szCmdLn," ");
   if (iCmdPos <= 0) iCmdPos= strlen(szCmdLn);
 
   iCmdLn= strncmp( szCmdLn, "Jul", (iCmdPos>(const size_t)strlen("Jul")? iCmdPos: (const size_t)strlen("Jul")));
   if (iCmdLn < 0) // is less than Jul
   {
      iCmdLn= strncmp( szCmdLn, "Dec", (iCmdPos>(const size_t)strlen("Dec")? iCmdPos: (const size_t)strlen("Dec")));
      if (iCmdLn < 0) // is less than Dec
      {
         if (strncmp( szCmdLn, "Apr", (iCmdPos>(const size_t)strlen("Apr")? iCmdPos: (const size_t)strlen("Apr")))== 0)
         {
            iRet= 4;
         } else { // not Apr
            if (strncmp( szCmdLn, "Aug", (iCmdPos>(const size_t)strlen("Aug")? iCmdPos: (const size_t)strlen("Aug")))== 0)
            {
               iRet= 8;
            } else { //unknown token
               iRet= 0;
            } // End of(2:Aug)
         } // End of(1:Apr)
      } else {
         if (iCmdLn > 0) // is higher than Dec
         {
            if (strncmp( szCmdLn, "Feb", (iCmdPos>(const size_t)strlen("Feb")? iCmdPos: (const size_t)strlen("Feb")))== 0)
            {
               iRet= 2;
            } else { // not Feb
               if (strncmp( szCmdLn, "Jan", (iCmdPos>(const size_t)strlen("Jan")? iCmdPos: (const size_t)strlen("Jan")))== 0)
               {
                  iRet= 1;
               } else { //unknown token
                  iRet= 0;
               } // End of(5:Jan)
            } // End of(4:Feb)
         } else {
            if (iCmdLn == 0) // Token Dec found
            {
               iRet= 12;
            } // End of(3:Dec)
         }
      }
   } else {
      if (iCmdLn > 0) // is higher than Jul
      {
         iCmdLn= strncmp( szCmdLn, "Mar", (iCmdPos>(const size_t)strlen("Mar")? iCmdPos: (const size_t)strlen("Mar")));
         if (iCmdLn < 0) // is less than Mar
         {
            if (strncmp( szCmdLn, "Jun", (iCmdPos>(const size_t)strlen("Jun")? iCmdPos: (const size_t)strlen("Jun")))== 0)
            {
               iRet= 6;
            } else { // not Jun
               if (strncmp( szCmdLn, "Mai", (iCmdPos>(const size_t)strlen("Mai")? iCmdPos: (const size_t)strlen("Mai")))== 0)
               {
                  iRet= 5;
               } else { //unknown token
                  iRet= 0;
               } // End of(8:Mai)
            } // End of(7:Jun)
         } else {
            if (iCmdLn > 0) // is higher than Mar
            {
               iCmdLn= strncmp( szCmdLn, "Oct", (iCmdPos>(const size_t)strlen("Oct")? iCmdPos: (const size_t)strlen("Oct")));
               if (iCmdLn == 0)
               {
                  iRet= 10;
               } else { // not Oct
                  if (iCmdLn < 0)
                  {
                     iCmdLn= strncmp( szCmdLn, "Nov", (iCmdPos>(const size_t)strlen("Nov")? iCmdPos: (const size_t)strlen("Nov")));
                     if (iCmdLn == 0)
                     {
                        iRet= 11;
                     } else { //unknown token
                        iRet= 0;
                     }
                  } else {
                     if (strncmp( szCmdLn, "Sep", (iCmdPos>(const size_t)strlen("Sep")? iCmdPos: (const size_t)strlen("Sep")))== 0)
                     {
                        iRet= 9;
                     } else { //unknown token
                        iRet= 0;
                     } // End of(12:Sep)
                  } // End of(10:Nov)
               } // End of(11:Oct)
            } else {
               if (iCmdLn == 0) // Token Mar found
               {
                  iRet= 3;
               } // End of(9:Mar)
            }
         }
      } else {
         if (iCmdLn == 0) // Token Jul found
         {
            iRet= 7;
         } // End of(6:Jul)
      }
   }
   return(iRet);
} /* end of function func_MonParser */