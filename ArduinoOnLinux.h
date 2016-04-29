#ifndef ARDUINOONLINUX_H
#define ARDUINOONLINUX_H

#include <map>
#include <string>

namespace Arduino
{


enum ArduinoGpioOutputLevel
{
   LOW,
   HIGH
};

enum ArduinoGpioMode
{
   INPUT,
   OUTPUT
};

class ArduinoOnLinux
{
public:
   ArduinoOnLinux();
};

void digitalWrite(int, ArduinoGpioOutputLevel);

void pinMode(int, ArduinoGpioMode);

ArduinoGpioOutputLevel digitalRead(int);

}


#endif // ARDUINOONLINUX_H
