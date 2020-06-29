/**@file NeoClock.ino */

#include <WiFi.h>
#include "NeoClockInterface.h"
#include "Wifi_Info.h"
#include "time.h"

//!Pointer to the NeoClock Interface object, set by setup()
NeoClockInterface* neoClock = NULL;

//Time settings
//! Which NTP Server to use
const char* ntpServer = "pool.ntp.org";
//! Set our time zone as a GMT offset
const long  gmtOffset_sec = -5 * 3600;
//! Set Daylight Saving Offset in seconds
const int   daylightOffset_sec = 3600;

//! Count of the times the function loop() has been called
int loopCounter;

/**
 * Print value of local clock to serial port
 */
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print(&timeinfo, "%A, %B %d %Y %H:%M:%S   --  ");
}

/**
 * Connect to wifi and grab time from NTP server. Used to set local clock.
 */
void setupTime() {

  Serial.begin(115200);

  Serial.println("---------------------------------");

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);

  //Loop until connected (TODO add break?)
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  Serial.println("---------------------------------");

  //Configure time with Time Zone, Daylight Saving and Network Time server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();
  Serial.println("");
  
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);


}

/**
 * Arduino Setup Function
 */
void setup()
{
  neoClock = new NeoClockInterface();

  neoClock->begin();
  neoClock->allOff();

  loopCounter = 0;

  setupTime();
}


/**
 * Main Loop
 */
void loop()
{
  printLocalTime();

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

/*

  //Draw Time
  neoClock->drawTime_12(timeinfo.tm_hour,
                     timeinfo.tm_min,
                     rgb_green_dim);

  neoClock->drawPixels();

  delay(5000);
*/

  //Draw Time
  neoClock->drawTime_24(timeinfo.tm_hour,
                     timeinfo.tm_min,
                     rgb_white_dim);
  neoClock->drawPixels();

  delay(5000);


  loopCounter++;

}
