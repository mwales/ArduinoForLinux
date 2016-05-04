#include "ArduinoOnLinux.h"

namespace Arduino
{

ArduinoOnLinux::ArduinoOnLinux()
{

}

bool ArduinoOnLinux::setMode(ArduinoGpioMode mode)
{
   return true;
}

void digitalWrite(int, ArduinoGpioOutputLevel)
{

}

void pinMode(int, ArduinoGpioMode)
{

}

ArduinoGpioOutputLevel digitalRead(int)
{
   return HIGH;
}

}
