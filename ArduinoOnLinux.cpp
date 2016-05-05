#include "ArduinoOnLinux.h"

namespace Arduino
{

ArduinoOnLinux* ArduinoOnLinux::theInstance = nullptr;

ArduinoOnLinux::ArduinoOnLinux()
{

}

ArduinoOnLinux* ArduinoOnLinux::getInstance()
{
   if (theInstance == nullptr)
   {
      theInstance = new ArduinoOnLinux();
   }

   return theInstance;
}

void ArduinoOnLinux::digitalWrite(int pinNum, ArduinoGpioOutputLevel lvl)
{
   GetGpio(pinNum)->setLevel(lvl);
}

void ArduinoOnLinux::pinMode(int pinNum, ArduinoGpioMode mode)
{
   auto it = theGpios.find(pinNum);

   if (it == theGpios.end())
   {
      GpioIo* gpio = new GpioIo(pinNum, mode);
      theGpios[pinNum] = gpio;
   }
   else
   {
      it->second->setMode(mode);
   }

}

ArduinoGpioOutputLevel ArduinoOnLinux::digitalRead(int pinNum)
{
   return GetGpio(pinNum)->getLevel();
}

GpioIo* ArduinoOnLinux::GetGpio(int pinNum)
{
   auto it = theGpios.find(pinNum);
   if ( it == theGpios.end())
   {
      GpioIo* gpio = new GpioIo(pinNum);
      theGpios[pinNum] = gpio;
      return gpio;
   }

   return it->second;
}


// C arduino calls
void digitalWrite(int pinNum, ArduinoGpioOutputLevel lvl)
{
   ArduinoOnLinux::getInstance()->digitalWrite(pinNum, lvl);
}

void pinMode(int pinNum, ArduinoGpioMode mode)
{
   ArduinoOnLinux::getInstance()->pinMode(pinNum, mode);
}

ArduinoGpioOutputLevel digitalRead(int pinNum)
{
   return ArduinoOnLinux::getInstance()->digitalRead(pinNum);
}

}
