#include "SPI.h"

namespace Arduino
{

void Spi::begin()
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::end()
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::beginTransaction(SPISettings const & settings)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::endTransaction()
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setBitOrder(SpiBitOrdering order)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setClockDivider(uint8_t divider)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setDataMode(SpiDataMode mode, int slaveSelectPin = 0)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::transfer(uint8_t byteOfData)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::transfer(char* buf, int size)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::transfer(uint16_t wordOfData)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::usingInterrupt(int intNumber)
{
   std::cerr << "Not implemented" << std::endl;
}

}
