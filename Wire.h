
#pragma once

#include <stdint.h>
#include <vector>

// Implentation of Arduino Wire library

namespace Arduino
{



class I2cArduino
{

public:

   I2cArduino();

   ~I2cArduino();

   void begin();

   void begin(uint8_t address, uint8_t busNum = 1);

   uint8_t requestFrom(uint8_t address, uint8_t quantity, bool stop);

   void beginTransmission(uint8_t address);

   int endTransmission(bool stop = true);

   uint8_t write(uint8_t);

   uint8_t write(char const * string);

   uint8_t write(char const * dataBuf, uint8_t length);

   uint8_t available();

   uint8_t read();

protected:

   void SetAddress(uint8_t address);

   int theDevFileFd;

   uint8_t theAddress;

   std::vector<uint8_t> theDataBuffer;

   // void onRequest(function pointer);

   // void onReceive(function pointer);
};

extern I2cArduino Wire;

}
