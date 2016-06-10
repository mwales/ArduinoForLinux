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
   /// MSBFIRST is the most common
   MSBFIRST,
   LSBFIRST
};

enum SpiDataMode
{
   SPI_MODE0,
   SPI_MODE1,
   SPI_MODE2,
   SPI_MODE3
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

   /// Bits per second / MHz
   uint32_t theSpeed;

};

class Spi
{

public:

   Spi();
   ~Spi();

   void begin();

   void end();

   void beginTransaction(SPISettings const & settings);

   void endTransaction();

   /**
    * Sets bit-order for MSB vs LSB
    *
    * Implemented but testing on RPi shows no difference in settings.  Might be a RPi issue, or
    * maybe my code sucks, who knows...
    *
    * @param order
    */
   void setBitOrder(SpiBitOrdering order);

   void setClockDivider(uint8_t divider);

   /**
    * Sets the SPI data transfer mode
    *
    * @param mode Modes are SPI_MODE0...3
    * @param slaveSelectPin Doesn't do anything for Linux
    */
   void setDataMode(SpiDataMode mode, int slaveSelectPin = 0);

   /**
    * This is not part of arduino API, but used for Linux
    *
    * @param bus interface to use for SPI (default is 0)
    */
   void setSpiBus(int bus);

   /**
    * This is not part of the arduino API, but used for Linux
    *
    * @note It appeared to me on the O-Scope that the selected device had it's CS signal go low when
    *       selected and data on it's way.  At all other times CS signal was high
    *
    * @param cs Chipselect setting (0 or 1)
    */
   void setChipSelect(int cs);

   uint8_t transfer(uint8_t byteOfData);
   void transfer(char* buf, int size);
   uint16_t transfer(uint16_t wordOfData);

   void usingInterrupt(int intNumber);

   int theFd;

   int theSpiBus;

   int theChipSelect;

   SpiBitOrdering theBitOrdering;

   SPISettings theCurrentSettings;

protected:

   bool openSpiBus(int bus, int cs);
};

extern Spi SPI;

}
