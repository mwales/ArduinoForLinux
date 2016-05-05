#ifndef ARDUINOONLINUX_H
#define ARDUINOONLINUX_H

#include <map>
#include <string>
#include <fstream>
#include <map>

#include "GpioIo.h"
namespace Arduino
{




class ArduinoOnLinux
{
public:

   static ArduinoOnLinux* getInstance();

   void digitalWrite(int, ArduinoGpioOutputLevel);

   void pinMode(int, ArduinoGpioMode);

   ArduinoGpioOutputLevel digitalRead(int);

protected:

   ArduinoOnLinux();

   GpioIo* GetGpio(int pinNum);

   static ArduinoOnLinux* theInstance;

   std::map<int, GpioIo*> theGpios;


};

void digitalWrite(int, ArduinoGpioOutputLevel);

void pinMode(int, ArduinoGpioMode);

ArduinoGpioOutputLevel digitalRead(int);

}


#endif // ARDUINOONLINUX_H
