#ifndef ARDUINOONLINUX_H
#define ARDUINOONLINUX_H

#include <map>
#include <string>
#include <fstream>
#include <map>

#include "GpioIo.h"
#include "ArduinoSerial.h"

namespace Arduino
{


#define byte uint8_t

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

void delay(int ms);

char*  itoa ( int value, char * str, int base );

unsigned long millis();

//void attachInterrupt(int pinNumber, void *isrRoutine(void), ArduinoGpioInterruptEdge);

}


#endif // ARDUINOONLINUX_H
