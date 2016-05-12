#include "Wire.h"

// Implentation of Arduino Wire library

namespace Arduino
{

I2cArduino Wire;

void I2cArduino::begin()
{

}

void I2cArduino::begin(uint8_t address)
{

}

uint8_t I2cArduino::requestFrom(uint8_t address, uint8_t quantity, bool stop)
{
   return 0;
}

void I2cArduino::beginTransmission(uint8_t address)
{

}

uint8_t I2cArduino::endTransmission(bool stop)
{
   return 0;
}

uint8_t I2cArduino::write(uint8_t)
{
   return 0;
}

uint8_t I2cArduino::write(char const * string)
{
   return 0;
}

uint8_t I2cArduino::write(char const * dataBuf, uint8_t length)
{
   return 0;
}

uint8_t I2cArduino::available()
{
   return 0;
}

uint8_t I2cArduino::read()
{
   return 0;
}




}









