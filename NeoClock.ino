/**@file NeoClock.ino */
#ifndef __WIFI_H__
#include <WiFi.h>
#define __WIFI_H__
#endif
#include "NeoClockInterface.h"
#include "ClockWebClient.h"
#include "ClockFunctionClass.h"
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

//!
const int    HECTOSECONDS = 100;

ClockFunctionClass* clockFunctions = NULL;

WiFiServer server(80);

/**
   Print value of local clock to serial port
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
   Connect to wifi and grab time from NTP server. Used to set local clock.
*/
void setupTime() {

  Serial.begin(115200);

  Serial.println("---------------------------------");

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);

  Serial.print("WiFi MAC Address:");
  Serial.println(WiFi.macAddress());

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


  //Set up Webserver
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

/**
   Arduino Setup Function
*/
void setup()
{
  neoClock = new NeoClockInterface();

  neoClock->begin();
  neoClock->allOff();

  setupTime();

  clockFunctions = NULL;

}

/**

*/
void deleteOldFunctions() {

  ClockFunctionClass* thisCFC = NULL;

  while (clockFunctions != NULL) {
    thisCFC = clockFunctions->getNext();
    delete clockFunctions;
    clockFunctions = thisCFC;
  }

}

/**

*/
void loadNewFunctions(ClockWebClient* cwc) {

  ClockFunctionClass* firstFunction = NULL;
  ClockFunctionClass* thisFunction = NULL;
  CommandsBuffer*     commandsBuffer = cwc->getCommandBuffer();

  //If there are commands to be process,
  // build a new linked list of functions
  // and delete the old list.

  //If there are functions
  if (commandsBuffer != NULL)
  {
    commandsBuffer->reset();

    if (commandsBuffer->areThereMoreCommands()) {

      //Get First Function
      uint8_t* thisBuffer = commandsBuffer->getNextCommand();

      firstFunction = new ClockFunctionClass(thisBuffer);
      thisFunction = firstFunction;

      //While there are more functions
      while (commandsBuffer->areThereMoreCommands()) {
        thisBuffer = commandsBuffer->getNextCommand();
        thisFunction->setNext(new ClockFunctionClass(thisBuffer));

        //Advance down the linked list
        thisFunction = thisFunction->getNext();
      }

      //Clean up old functions and set function lists to the new list
      deleteOldFunctions();
      clockFunctions =  firstFunction;
    }
  }
}

/**
   Main Loop
*/
void loop()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }

  if (clockFunctions == NULL) {
    //Draw Time
    neoClock->drawTime_12(timeinfo.tm_hour,
                          timeinfo.tm_min,
                          rgb_white_dim );
    neoClock->drawPixels();
  } else {

    ClockFunctionClass* cfc = clockFunctions;

    while (cfc != NULL)
    {
      cfc->setTimeInfo(timeinfo);

      Serial.print("Function: ");
      Serial.print(cfc->getFunctionName());
      Serial.print(" \tDutyCycle:  ");
      Serial.print(cfc->getDutyCycle());
      Serial.print(" \tClockValue: ");
      Serial.println(cfc->getClockValue());

      neoClock->drawClockValue(cfc->getClockValue(),
                               cfc->getColor());
      neoClock->drawPixels();

      delay(cfc->getDutyCycle() * HECTOSECONDS);

      cfc = cfc->getNext();
    }

  }

  //Serve any Web Requests
  WiFiClient client = server.available();
  if (client) {

    ClockWebClient* cwc = new ClockWebClient(client);

    cwc->processRequest();

    loadNewFunctions(cwc);

    delete cwc;
    cwc = NULL;


  }//End if client

}//End Loop
