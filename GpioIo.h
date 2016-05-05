#ifndef GPIOIO_H
#define GPIOIO_H

#include <iostream>
#include <fstream>

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

class GpioIo
{
public:

   GpioIo(int pin, ArduinoGpioMode mode = OUTPUT, ArduinoGpioOutputLevel lvl = LOW);

   ~GpioIo();

   void setLevel(ArduinoGpioOutputLevel lvl);

   ArduinoGpioOutputLevel getLevel();

   void setMode(ArduinoGpioMode mode);

protected:

   void exportGpio();

   int thePin;

   ArduinoGpioMode theMode;

   std::fstream theGpioValueFile;
};

}
#endif // GPIOIO_H
