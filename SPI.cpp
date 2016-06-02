#include "SPI.h"
#include <iostream>
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <fcntl.h>
#include <sstream> // std::ostringstream
#include <cstring> // strerror
#include <errno.h>
#include <assert.h>
#include <unistd.h> // close
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

namespace Arduino
{

Spi SPI;

Spi::Spi():
theFd(-1),
theSpiBus(0),
theCurrentSettings(1000,MSBFIRST,SPI_MODE0)
{

}

Spi::~Spi()
{
   if (theFd > 0)
   {
      std::cout << "Closing the SPI device" << std::endl;
   }
}

void Spi::begin()
{
   std::cout << "NOP: " << __PRETTY_FUNCTION__ << std::endl;
}

void Spi::end()
{
   std::cout << "NOP: " << __PRETTY_FUNCTION__ << std::endl;
}

void Spi::beginTransaction(SPISettings const & settings)
{
   std::cerr << "Not implemented" << std::endl;
   if (theFd == -1)
   {
      openSpiBus(theSpiBus, theChipSelect);
   }
}

void Spi::endTransaction()
{
   std::cout << "NOP: " << __PRETTY_FUNCTION__ << std::endl;
}

void Spi::setBitOrder(SpiBitOrdering order)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setClockDivider(uint8_t divider)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setDataMode(SpiDataMode mode, int slaveSelectPin)
{
   std::cerr << "Not implemented" << std::endl;
}

uint8_t Spi::transfer(uint8_t byteOfData)
{
   if (theFd == -1)
   {
      std::cerr << "Bus not initialized" << std::endl;
      return 0;
   }

   uint8_t retData;

   struct spi_ioc_transfer iData;
   memset(&iData, 0, sizeof(iData));
   iData.tx_buf = (__u64) &byteOfData;
   iData.rx_buf = (__u64) &retData;
   iData.len = sizeof(retData);
   iData.speed_hz = theCurrentSettings.theSpeed;

   if (ioctl(theFd, SPI_IOC_MESSAGE(1), &iData) == -1)
   {
      std::cerr << "Encountered error sending 1 byte of SPI data: " << strerror(errno) << std::endl;
   }

   std::cout << "Returned val = " << (int) retData << std::endl;
   return retData;
}

void Spi::transfer(char* buf, int size)
{
   if (theFd == -1)
   {
      std::cerr << "Bus not initialized" << std::endl;
      return;
   }

   struct spi_ioc_transfer iData;
   memset(&iData, 0, sizeof(iData));
   iData.tx_buf = (__u64) buf;
   iData.rx_buf = (__u64) buf;
   iData.len = size;
   iData.speed_hz = theCurrentSettings.theSpeed;

   if (ioctl(theFd, SPI_IOC_MESSAGE(1), &iData) == -1)
   {
      std::cerr << "Encountered error sending " << size << " bytes of SPI data: " << strerror(errno) << std::endl;
   }
   else
   {
      std::cout << "Sent " << size << " bytes of data over SPI bus" << std::endl;
   }
}

uint16_t Spi::transfer(uint16_t wordOfData)
{
   if (theFd == -1)
   {
      std::cerr << "Bus not initialized" << std::endl;
      return 0;
   }

   uint16_t retData;

   struct spi_ioc_transfer iData;
   memset(&iData, 0, sizeof(iData));
   iData.tx_buf = (__u64) &wordOfData;
   iData.rx_buf = (__u64) &retData;
   iData.len = sizeof(retData);
   iData.speed_hz = theCurrentSettings.theSpeed;

   if (ioctl(theFd, SPI_IOC_MESSAGE(1), &iData) == -1)
   {
      std::cerr << "Encountered error sending 2 bytes of SPI data: " << strerror(errno) << std::endl;
   }

   std::cout << "Returned val = " << (int) retData << std::endl;
   return retData;
}

void Spi::usingInterrupt(int intNumber)
{
   std::cerr << "Not implemented" << std::endl;
}

void Spi::setSpiBus(int bus)
{
   if ( (theSpiBus != bus) && (theFd > 0) )
   {
      // We need to reopen the SPI bus device file
      close(theFd);
      theFd = -1;


   }


}

void Spi::setChipSelect(int cs)
{
   assert((cs == 1) || (cs == 0));

   if ( (theFd != -1) && (cs != theChipSelect) )
   {
      // Need to reopen the file
      close(theFd);
      theFd = -1;

      if (openSpiBus(theSpiBus, cs))
      {
         std::cout << "SPI chip select value = " << cs << std::endl;
         theChipSelect = cs;
      }
      else
      {
         std::cout << "Failed to set the SPI chip select value to " << cs << " for bus "
                   << theSpiBus << std::endl;
      }
   }
}

bool Spi::openSpiBus(int bus, int cs)
{
   assert(theFd == -1);

   std::ostringstream oss;
   oss << "/dev/spidev" << bus << "." << cs;

   theFd = open(oss.str().c_str(), O_RDWR);
   if (theFd == -1)
   {
      std::cerr << "Error opening SPI interface: " << oss.str() << ": " << strerror(errno) << std::endl;
      return false;
   }
   else
   {
      std::cout << "Opening SPI interface: " << oss.str() << std::endl;
      return true;
   }
}

}
