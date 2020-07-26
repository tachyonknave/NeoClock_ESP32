#include "time.h"

class FunctionExecClass {

  protected:

    struct tm m_timeInfo;

  public:

    virtual uint16_t getValue();
    virtual String getName();

    virtual void setTimeInfo(struct tm timeInfo);
};

void FunctionExecClass::setTimeInfo(struct tm timeInfo) {
  m_timeInfo = timeInfo;
}



//=================================================================================

// Derived class
class Countdown: public FunctionExecClass {

  private:
    uint16_t dayOffset = 0;

  public:
    Countdown(uint16_t params)
    {
      dayOffset = params;
    }

    uint16_t getValue() {
      return max(dayOffset - m_timeInfo.tm_yday, 0);
    }

    String getName() {
      return " Day Countdown ";
    }


};

// Derived class
class TwentyFourHourClock: public FunctionExecClass {

  public:
    TwentyFourHourClock() {

    }
    uint16_t getValue() {

      uint16_t hour = m_timeInfo.tm_hour;

      switch (hour)
      {
        case 0:
          hour = 12; break;
        case 20:
        case 21:
        case 22:
        case 23:
          hour = hour % 12;
          break;
      }

      return (hour * 100) +  m_timeInfo.tm_min;
    }

    String getName() {
      return " Twenty Four Hour Clock ";
    }

};
// Derived class
class TwelveHourClock: public FunctionExecClass {

  public:

    TwelveHourClock() {
    }

    uint16_t getValue() {

      uint16_t hour = m_timeInfo.tm_hour % 12;

      if (hour == 0)
        hour = 12;

      return (hour * 100) +  m_timeInfo.tm_min;
    }

    String getName() {
      return " Twelve Hour Clock ";
    }

};

// Derived class
class DateClock: public FunctionExecClass {

  public:

    DateClock() {
    }

    uint16_t getValue() {

      uint16_t month = m_timeInfo.tm_mon + 1;
      uint16_t day   = m_timeInfo.tm_mday;

      return (month * 100) +  day;
    }

    String getName() {
      return " Twelve Hour Clock ";
    }

};
// Derived class
class MinSecTimer: public FunctionExecClass {

  private:
    uint32_t startSecond = 0;
    int16_t offset = 0;

  public:

    MinSecTimer(uint16_t params) {
      offset = params;
    }

    uint16_t getValue() {

      uint32_t elapsed = ((m_timeInfo.tm_hour * 3600) + (m_timeInfo.tm_min * 60) + m_timeInfo.tm_sec) - startSecond;

      return ((elapsed / 60) * 100) +  elapsed % 60;
    }

    String getName() {
      return " Minute Second Timer ";
    }

    void setTimeInfo(struct tm timeInfo) override {
      if (startSecond == 0) {
        startSecond = (timeInfo.tm_hour * 3600) + ( timeInfo.tm_min * 60 ) + timeInfo.tm_sec - offset;
      }

      m_timeInfo = timeInfo;
    }


};

// Derived class
class HourMinTimer: public FunctionExecClass {

  private:
    uint32_t startMinute   = 0;
    uint16_t startDay      = 0;
    int16_t  offsetMinute = 0;

  public:

    HourMinTimer(uint16_t params) {
      offsetMinute = params;
    }

    uint16_t getValue() {

      uint16_t elapsedDays = m_timeInfo.tm_yday - startDay;
      uint32_t elapsed = (elapsedDays * 24 * 60) + (m_timeInfo.tm_hour * 60) + (m_timeInfo.tm_min ) - startMinute;

      if (elapsed > 1199)
        elapsed = 0;

      return ((elapsed / 60) * 100) +  elapsed % 60;
    }

    String getName() {
      return " Hour Minute Timer ";
    }

    void setTimeInfo(struct tm timeInfo) override {
      if (startMinute == 0) {
        startMinute = (timeInfo.tm_hour * 60) + ( timeInfo.tm_min) - offsetMinute;
        startDay    = timeInfo.tm_yday;
      }

      m_timeInfo = timeInfo;
    }


};


// Derived class
class HourMinCountdown: public FunctionExecClass {

  private:
    uint32_t endMinute = 0;

  public:

    HourMinCountdown(uint16_t params) {
      endMinute = params;
    }

    uint16_t getValue() {

      uint16_t remainingMinutes = 0;

      uint16_t nowMinute = ((m_timeInfo.tm_hour * 60) + m_timeInfo.tm_min);

      if (endMinute < nowMinute) {

        //When Countdown completes display 0 for 30 minutes
        if ((nowMinute - endMinute) < 30) {
          remainingMinutes = 0;
        } else {
          remainingMinutes = (endMinute + 1440) - nowMinute;
        }

      } else {
        remainingMinutes = endMinute - nowMinute;
      }

      return ((remainingMinutes / 60) * 100) +  remainingMinutes % 60;
    }

    String getName() {
      return " Hour Minute Countdown ";
    }



};

// Derived class
class AltTimeZone: public FunctionExecClass {

    uint16_t offsetMinutes = 0;

  public:

    AltTimeZone(uint16_t params) {
      offsetMinutes = params;
    }

    uint16_t getValue() {

      uint16_t hour = m_timeInfo.tm_hour;
      uint16_t minutes = m_timeInfo.tm_min;

      uint16_t newTime = ((hour * 60) + minutes + offsetMinutes);

      //Mod 1440 to handle day rollover
      newTime = newTime % 1440;

      hour = (newTime / 60 ) % 12;
      minutes = newTime % 60;

      if (hour == 0)
        hour = 12;

      return (hour * 100) +  minutes;
    }

    String getName() {
      return " Alternate Time Zone ";
    }

};


//=================================================================================

FunctionExecClass* buildFunctionExecClass(uint8_t functionCode,
    uint16_t parameters) {

  FunctionExecClass* returnObject = NULL;

  // 0 - 12 Hour Clock
  // 1 - 24 Hour Clock
  // 2 - Date
  // 3 - HourMin Countdown
  // 4 - Day Coundown
  // 5 - HourMin timer
  // 6 - MinSec Timer
  // 7 - Alt Timezone

  if (functionCode == 0) {
    returnObject = new TwelveHourClock();
  } else if (functionCode == 1) {
    returnObject = new TwentyFourHourClock();
  } else if (functionCode == 2) {
    returnObject = new DateClock();
  } else if (functionCode == 3) {
    returnObject = new HourMinCountdown(parameters);
  } else if (functionCode == 4) {
    returnObject = new Countdown(parameters);
  } else if (functionCode == 5) {
    returnObject = new HourMinTimer(parameters);
  } else if (functionCode == 6) {
    returnObject = new MinSecTimer(parameters);
  } else if (functionCode == 7) {
    returnObject = new AltTimeZone(parameters);
  }


  return returnObject;
}
