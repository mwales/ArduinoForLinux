#ifndef GPIOIO_H
#define GPIOIO_H

#include <iostream>
#include <fstream>
#include <pthread.h>

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

enum ArduinoGpioInterruptEdge
{
   CHANGE,
   RISING,
   FALLING
};

class GpioIo
{
public:

   GpioIo(int pin, ArduinoGpioMode mode = OUTPUT, ArduinoGpioOutputLevel lvl = LOW);

   ~GpioIo();

   void setLevel(ArduinoGpioOutputLevel lvl);

   ArduinoGpioOutputLevel getLevel();

   void setMode(ArduinoGpioMode mode);

   void attachInterrupt(void (*gpioIsr)(), ArduinoGpioInterruptEdge edgeCondition);

protected:

   void exportGpio();

   friend void* isrThreadMain(void* gpioParam);

   int thePin;

   ArduinoGpioMode theMode;

   std::fstream theGpioValueFile;

   pthread_t theIsrThread;

   bool theIsrCreated;

   /// Set to true to signal thread to quit
   bool theThreadStopEvent;

   void (*theIsrCallbackFunc)();
};

}
#endif // GPIOIO_H
