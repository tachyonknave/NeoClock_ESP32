#include <NeoPixelBus.h>
#include "FunctionExecClass.h"


/**
*/
class ClockFunctionClass {

  private:

    FunctionExecClass* funcPtr;

    RgbColor color;

    uint16_t dutyCycle;

    ClockFunctionClass* nextFunctionClass;

  public:
    static const uint8_t FunctionCodeIndex = 0;
    static const uint8_t DutyCycleIndex = 1;
    static const uint8_t ParameterHigh = 2;
    static const uint8_t ParamterLow = 3;
    static const uint8_t RGB_Red = 4;
    static const uint8_t RGB_Green = 5;
    static const uint8_t RGB_Blue = 6;
  
    ClockFunctionClass(uint8_t functionCommand[]);

    bool hasNext();

    //Getters
    RgbColor            getColor();
    uint8_t             getDutyCycle();
    uint16_t            getClockValue();
    String              getFunctionName();
    ClockFunctionClass* getNext();

    //Setters
    void    setTimeInfo(struct tm ti);
    void    setNext(ClockFunctionClass* cfc);
};


ClockFunctionClass::ClockFunctionClass(uint8_t functionCommand[])
{

  uint8_t functionCode = functionCommand[FunctionCodeIndex];
  dutyCycle = functionCommand[DutyCycleIndex];

  uint16_t params = (functionCommand[ParameterHigh] << 8) + functionCommand[ParamterLow];

  funcPtr = buildFunctionExecClass(functionCode, params);

  color = RgbColor(functionCommand[RGB_Red],
                   functionCommand[RGB_Green],
                   functionCommand[RGB_Blue]);

  nextFunctionClass = NULL;

  char message[32];
  sprintf(message, "%.2X  %.2X  %.2X %.2X   %.2X %.2X %.2X\n\0",
          functionCommand[0],
          functionCommand[1],
          functionCommand[2],
          functionCommand[3],
          functionCommand[4],
          functionCommand[5],
          functionCommand[6]);
  Serial.println(message);


}

RgbColor ClockFunctionClass::getColor() {

  return color;

}

uint8_t ClockFunctionClass::getDutyCycle() {

  return dutyCycle;

}

void ClockFunctionClass::setTimeInfo(struct tm timeInfo) {

  if (funcPtr != NULL)
    funcPtr->setTimeInfo(timeInfo);

}


uint16_t ClockFunctionClass::getClockValue() {

  uint16_t returnValue = 0;

  if (funcPtr != NULL)
    returnValue = funcPtr->getValue();

  return returnValue;

}

String ClockFunctionClass::getFunctionName() {

  String returnValue = "";

  if (funcPtr != NULL)
    returnValue = funcPtr->getName();

  return returnValue;

}

bool ClockFunctionClass::hasNext() {
  return (nextFunctionClass != NULL);
}

ClockFunctionClass* ClockFunctionClass::getNext() {
  return nextFunctionClass;
}

void ClockFunctionClass::setNext(ClockFunctionClass* cfc)
{
  nextFunctionClass = cfc;
}
