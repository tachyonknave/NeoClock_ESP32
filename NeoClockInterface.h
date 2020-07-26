#ifndef __NEOCLOCKINTERFACE_H__
#define __NEOCLOCKINTERFACE_H__

#include <NeoPixelBus.h>


// Define the pin number, and number of pixels we'll be using
#define PIXELS_PIN 18
#define PIXELS_COUNT 96

#define HOUR_DIGIT_START 84
#define HOUR_DIGIT_END PIXELS_COUNT

#define SEGMENT_SIZE 4

RgbColor pixelArray[PIXELS_COUNT];

RgbColor rgb_white_dim = RgbColor(5, 5, 5);
RgbColor NCI_RGB_OFF = RgbColor(0, 0, 0);

// Create NeoPixelBus object, tell it what type of pixels we're using, what speed
// to use in talking to them, and the number of pixels and pin number
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(PIXELS_COUNT, PIXELS_PIN);

/**
    Hardware interface for the custom made clock composed of NeoPixel segments
*/
class NeoClockInterface {

  private:

    RgbColor pixelArray[PIXELS_COUNT];

    void drawHour(int hour, RgbColor color);
    void drawMinute(int minutes, RgbColor color);
    void drawTheOne(RgbColor color);

    void drawSegment(int segmentIndex, RgbColor color);
    void drawNumberA( int number, RgbColor color);
    void drawNumberB(int digit, int number, RgbColor color);

    void allDigitsTest();
    void segmentCycle(int segmentIndex);

  public:
    NeoClockInterface();

    void begin();

    void allOff();
    void drawPixels();

    void drawTime_12(int hour, int minute, RgbColor color);

    void drawClockValue(int clockValue, RgbColor color);
};

/**
   Default constructor
*/
NeoClockInterface::NeoClockInterface() {
}

//-------------------------------------------------------
//Low Level Functions
//-------------------------------------------------------


/**
   Copy the color values from the local pixel array out to the hardware.
   This is "drawing" the pixels. To turn off a pixel, chose the "off" color
*/
void NeoClockInterface::drawPixels()
{

  // Set all the pixels to green
  for (int i = 0; i < PIXELS_COUNT; i++) {
    pixels.SetPixelColor(i, pixelArray[i]);
  }

  pixels.Show();

}

/**
   Initialize the NeoPixel library and hardwire
*/
void NeoClockInterface::begin() {
  // Tell the NeoPixelBus object to set itself up
  pixels.Begin();

}


/**
   Draw a segment of a 7-segment number, there are 4 pixels per segment

   @param segmentIndex 0-based index of segment to draw
   @param color color to draw segment, can be "off" color
*/
void NeoClockInterface::drawSegment(int segmentIndex, RgbColor color)
{
  int pixelStart = segmentIndex * SEGMENT_SIZE;
  int pixelEnd   = pixelStart + SEGMENT_SIZE;

  for (int pixelIndex = pixelStart; pixelIndex < pixelEnd; pixelIndex++)
  {
    pixelArray[pixelIndex] = color;
  }
}


/**
   Of three 7-segment displays, there are two configurations. This draws the first one

   @param number number to disply on 0th (Minute, one's digit) segment
   @param color color to draw segment, can be "off" color
*/
void NeoClockInterface::drawNumberA( int number, RgbColor color) {

  /*
     Segments
       / 2 \
      3     1
      +- 6 -+
      4     0
       \ 5 /
  */

  int startSegment = 0;

  switch (number) {
    case 0:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 1:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, NCI_RGB_OFF);
      drawSegment(startSegment + 3, NCI_RGB_OFF);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 2:
      drawSegment(startSegment,     NCI_RGB_OFF);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, NCI_RGB_OFF);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 3:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, NCI_RGB_OFF);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 4:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, NCI_RGB_OFF);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, color);
      break;
    case 5:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, NCI_RGB_OFF);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 6:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, NCI_RGB_OFF);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 7:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 8:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 9:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;

  }

}

/**
   Of three 7-segment displays, there are two configurations. This draws the second one

   @param digit which 7-segment dispay to use, 1 (minute 10's digit) or 2 (hour 1's digit)
   @param number number to disply on 0th (Minute, one's digit) segment
   @param color color to draw segment, can be "off" color
*/
void NeoClockInterface::drawNumberB(int digit, int number, RgbColor color) {

  /*
     Segments
       / 2 \
      1     3
      +- 0 -+
      6     4
       \ 5 /
  */

  int startSegment = 0;

  switch (digit) {
    case 1: startSegment = 7; break;
    case 2: startSegment = 14; break;
  }

  switch (number) {
    case 0:
      drawSegment(startSegment, NCI_RGB_OFF);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 1:
      drawSegment(startSegment, NCI_RGB_OFF);
      drawSegment(startSegment + 1, NCI_RGB_OFF);
      drawSegment(startSegment + 2, NCI_RGB_OFF);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 2:
      drawSegment(startSegment, color);
      drawSegment(startSegment + 1, NCI_RGB_OFF);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, NCI_RGB_OFF);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 3:
      drawSegment(startSegment, color);
      drawSegment(startSegment + 1, NCI_RGB_OFF);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 4:
      drawSegment(startSegment, color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, NCI_RGB_OFF);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 5:
      drawSegment(startSegment, color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, NCI_RGB_OFF);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 6:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, NCI_RGB_OFF);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 7:
      drawSegment(startSegment,     NCI_RGB_OFF);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, NCI_RGB_OFF);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;
    case 8:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, color);
      break;
    case 9:
      drawSegment(startSegment,     color);
      drawSegment(startSegment + 1, color);
      drawSegment(startSegment + 2, color);
      drawSegment(startSegment + 3, color);
      drawSegment(startSegment + 4, color);
      drawSegment(startSegment + 5, color);
      drawSegment(startSegment + 6, NCI_RGB_OFF);
      break;

  }

}

/**
   Turn off all pixels
   Sets all pixels to "off" color in pixel array and then draws the array
*/
void NeoClockInterface::allOff()
{
  for (int i = 0; i < PIXELS_COUNT; i++)
  {
    pixelArray[i] = NCI_RGB_OFF;
  }

  drawPixels();
}


/**
   Draw the Ten's digit of the hour, 1 is the only value number
   @param color Color of the hour's ten's digit, can be "off" color
*/
void NeoClockInterface::drawTheOne(RgbColor color) {

  for (int pixelIndex = HOUR_DIGIT_START; pixelIndex < HOUR_DIGIT_END; pixelIndex++)
  {
    pixelArray[pixelIndex] = color;
  }
}

/**
   Draw the hour digits
   @param hour (1-12 or 1-19 depending on mode)
   @param color to draw the digits, can be "off" color
*/
void NeoClockInterface::drawHour(int hour, RgbColor color)
{
  int hour_12 = hour;

  if (hour_12 == 0)
  {
    hour_12 = 12;
  }

  //If Hour is greater than nine then light the 4th digit (1)
  drawTheOne((hour_12 > 9) ? color : NCI_RGB_OFF);

  //Hour One's Digit
  drawNumberB(2, hour_12 % 10, color);

}

/**
   Draw the minute digits
   @param minutes (00-59)
   @param color to draw the digits, can be "off" color
*/
void NeoClockInterface::drawMinute(int minutes, RgbColor color)
{
  int ones = minutes % 10;
  int tens = minutes / 10;

  drawNumberA(ones, color);
  drawNumberB(1, tens, color);
}

//-------------------------------------------------------
//Debug Functions
//-------------------------------------------------------

/**
   Debug Function: Draw each 7-segment display, one number at a time. Drawn in blue
*/
void NeoClockInterface::allDigitsTest()
{
  allOff();
  delay (500);

  //Draw minute ones's place test
  for (int i = 0; i < 10; i++)
  {
    drawNumberA(i, rgb_white_dim);
    drawPixels();
    delay(500);
  }


  //Draw minute ten's place test
  for (int i = 0; i < 10; i++)
  {
    drawNumberB(1, i, rgb_white_dim);
    drawPixels();
    delay(500);
  }

  //Draw hour ones's place test
  for (int i = 0; i < 10; i++)
  {
    drawNumberB(2, i, rgb_white_dim);
    drawPixels();
    delay(500);
  }

  drawTheOne(rgb_white_dim);
  drawPixels();
  delay(500);

}

/**
   Debug Function: Draw a segment in white, all 7-segement displays simultaneously.
   Used to figure out mapping of number segment to led segment
*/
void NeoClockInterface::segmentCycle(int segmentIndex)
{
  allOff();

  drawSegment(segmentIndex, rgb_white_dim);
  drawSegment(segmentIndex + 7, rgb_white_dim);
  drawSegment(segmentIndex + 14, rgb_white_dim);

  drawPixels();
}

//-------------------------------------------------------
//High Level Functions
//-------------------------------------------------------


/**
   Draw a number (between 0 and 1999)
   @param clockValue Number to draw (mod 1999)
   @param color Color to draw number
*/
void NeoClockInterface::drawClockValue(int clockValue, RgbColor color) {

  if (clockValue > 1999) {
    clockValue = clockValue % 2000;
  }

  allOff();

  drawNumberA(    clockValue % 10,        color);
  drawNumberB(1, (clockValue / 10) % 10,  color);
  drawNumberB(2, (clockValue / 100) % 10, color);

  if (clockValue >= 1000)
    drawTheOne(color);
  else
    drawTheOne(NCI_RGB_OFF);

}


/**
   Draw the time in 12-hour format
   @param hour Hour (1-23)
   @param minute Minute (1-59)
   @param color Color to draw date
*/
void NeoClockInterface::drawTime_12(int hour, int minute, RgbColor color) {

  drawHour(hour % 12, color);
  drawMinute(minute, color);


}

#endif
