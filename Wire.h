
#pragma once

#include <stdint.h>

// Implentation of Arduino Wire library

namespace Arduino
{



class I2cArduino
{

public:

   void begin();

   void begin(uint8_t address);

   uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stop);

   void beginTransmission(uint8_t address);

   uint8_t endTransmission(bool stop = true);

   uint8_t write(uint8_t);

   uint8_t write(char const * string);

   uint8_t write(char const * dataBuf, uint8_t length);

   uint8_t available();

   uint8_t read();

   // void onRequest(function pointer);

   // void onReceive(function pointer);
};

extern I2cArduino Wire;

}
