#include "ArduinoOnLinux.h"
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <time.h>

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

void delay(int ms)
{
   usleep(ms * 1000);
}

char* itoa(int value, char * str, int base)
{
   if (base == 10)
   {
      std::ostringstream oss;
      oss << value;
      strcpy(str, oss.str().c_str());
      return str;
   }

   if (base == 16)
   {
      std::ostringstream oss;
      oss << std::hex << value << std::dec;
      strcpy(str, oss.str().c_str());
      return str;
   }

   strcpy(str, "N/A");
   return str;
}


unsigned long millis()
{
   struct timespec t;
   bzero(&t, sizeof(t));

   if (clock_gettime(CLOCK_REALTIME, &t) == -1)
   {
      std::cerr << "Error getting the wall clock time for millis(): " << strerror(errno) << std::endl;
      return 0;
   }

   // For convenience and ret value sizing, we will maek this timer roll over 1/day.
   int seconds = t.tv_sec % (24*60*60);
   int milliseconds = t.tv_nsec / 1000000000;

   return seconds * 1000 + milliseconds;
}

//void attachInterrupt(int pinNumber, void *isrRoutine(void), ArduinoGpioInterruptEdge)
//{

//}

}
