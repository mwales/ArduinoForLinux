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
   ArduinoOnLinux();

   bool setMode(ArduinoGpioMode mode);

protected:

   std::map<int, GpioIo*> theGpios;


};

void digitalWrite(int, ArduinoGpioOutputLevel);

void pinMode(int, ArduinoGpioMode);

ArduinoGpioOutputLevel digitalRead(int);

}


#endif // ARDUINOONLINUX_H
