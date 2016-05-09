#pragma once

#include <cstdint>

// Shelled implentation of Arduino SPI library

namespace Arduino
{

const uint8_t SPI_CLOCK_DIV2   = 2;
const uint8_t SPI_CLOCK_DIV4   = 4;
const uint8_t SPI_CLOCK_DIV8   = 8;
const uint8_t SPI_CLOCK_DIV16  = 16;
const uint8_t SPI_CLOCK_DIV32  = 32;
const uint8_t SPI_CLOCK_DIV64  = 64;
const uint8_t SPI_CLOCK_DIV128 = 128;

enum SpiBitOrdering
{
   MSBFIRST,
   LSBFIRST
};

enum SpiDataMode
{
   SPI_MODE0,
   SPI_MODE1,
   SPI_MODE2,
};

class SPISettings
{
public:

   SPISettings(uint32_t speed, SpiBitOrdering bitOrder, SpiDataMode mode)
   {
      theSpeed = speed;
      theMode = mode;
      theOrder = bitOrder;
   }

   SpiBitOrdering theOrder;

   SpiDataMode theMode;

   uint32_t theSpeed;

};

class Spi
{

public:

   void begin();

   void end();

   void beginTransaction(SPISettings const & settings);

   void endTransaction();

   void setBitOrder(SpiBitOrdering order);

   void setClockDivider(uint8_t divider);

   void setDataMode(SpiDataMode mode, int slaveSelectPin = 0);

   void transfer(uint8_t byteOfData);
   void transfer(char* buf, int size);
   void transfer(uint16_t wordOfData);

   void usingInterrupt(int intNumber);
};

Spi SPI;

}
