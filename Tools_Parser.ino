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
  cntGPT++;
  if (cntGPT >= 5){
    bGPT_Flag = true;
    cntGPT=0;
  }
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
  }
  if (tindex >= 0) {
    FspTimer::force_use_of_pwm_reserved_timer();
    if (timer_ms.begin(TIMER_MODE_PERIODIC, timer_type, tindex, rate, 0.0f, timer_ms_callback)) 
    {
      if (timer_ms.setup_overflow_irq()) {
        if (timer_ms.open()) {
          if (timer_ms.start()) {
            bReturn = true;
          }
        }
      }
    } else {
      bReturn = false;
    }
  } else {
    bReturn = false;
  }
  return (bReturn);
}
