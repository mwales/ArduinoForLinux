#include "Wire.h"
#include <cstdlib>
#include <iostream>
#include <linux/i2c-dev.h>
#include <cassert>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

// Implentation of Arduino Wire library

// Uncomment to enable I2C debugging
//#define WIRE_DEBUGGING_ENABLED
//#define WIRE_SIMULATION

namespace Arduino
{

I2cArduino Wire;

I2cArduino::I2cArduino()
{
   std::cout << __FUNCTION__ << std::endl;

   theDevFileFd = -1;
}

I2cArduino::~I2cArduino()
{
   std::cout << __FUNCTION__ << std::endl;

   if (theDevFileFd != -1)
   {
      std::cout << "Closing the I2C device file" << std::endl;
      close(theDevFileFd);
      theDevFileFd = -1;
   }
}

void I2cArduino::begin()
{
   std::cout << __FUNCTION__ << " (no params)" << std::endl;

#ifdef WIRE_SIMULATION
   std::cout << "WIRE SIMULATION " << __PRETTY_FUNCTION__ << std::endl;
   return;
#endif

   if (theDevFileFd == -1)
   {
      const char* DEFAULT_I2C_BUS = "/dev/i2c-1";
      theDevFileFd = open(DEFAULT_I2C_BUS, O_RDWR);
      if (theDevFileFd == -1)
      {
         std::cerr << "Error opening the I2C dev file (" << DEFAULT_I2C_BUS << "):"
                   << strerror(errno) << std::endl;
      }
      else
      {
         std::cout << "Succesfully opened I2C device file (no address or bus specified) "
                   << DEFAULT_I2C_BUS << std::endl;
      }
   }
   else
   {
      std::cerr << "You already opened the I2c Device" << std::endl;
   }
}

void I2cArduino::begin(uint8_t address, uint8_t busNum)
{
   std::cout << __FUNCTION__ << "(" << std::hex << (int) address << std::dec << ", "
             << (int) busNum << ")" << std::endl;

   //assert (busNum >= 1);
   if (busNum <= 0)
   {
      std::cerr << "WTF, bus number invalid" << std::endl;
   }

#ifdef WIRE_SIMULATION
   std::cout << "WIRE SIMULATION " << __PRETTY_FUNCTION__ << std::endl;
   return;
#endif

   if (theDevFileFd == -1)
   {
      std::ostringstream ossFilename;
      ossFilename << "/dev/i2c-" << (int) busNum;

      std::cout << "About to open file: " << ossFilename.str() << std::endl;

      theDevFileFd = open(ossFilename.str().c_str(), O_RDWR);

      if (theDevFileFd == -1)
      {
         std::cerr << "Error opening the I2C dev file (" << ossFilename.str() << "):"
                   << strerror(errno) << std::endl;
      }
      else
      {
         std::cout << "Succesfully opened I2C device file " << ossFilename.str() << std::endl;

         SetAddress(address);
      }
   }
   else
   {
      std::cerr << "You already opened the I2c Device" << std::endl;
   }
}

uint8_t I2cArduino::requestFrom(uint8_t address, uint8_t quantity, bool stop)
{
   std::cout << __FUNCTION__ << std::endl;

   return 0;
}

void I2cArduino::beginTransmission(uint8_t address)
{
   SetAddress(address);

   theDataBuffer.clear();


}

int I2cArduino::endTransmission(bool stop)
{
   if (theDataBuffer.empty())
   {
      std::cerr << "Nothing to send over the I2c transmission, buffer empty" << std::endl;
      return 0;
   }

   uint8_t command = theDataBuffer.front();
   theDataBuffer.erase(theDataBuffer.begin());

   assert(theDataBuffer.size() <= 32);

#ifdef WIRE_DEBUGGING_ENABLED
   std::ostringstream oss;
   oss << "I2C Data Out = 0x" << std::hex << (int) command;
   for(auto&& dataByte : theDataBuffer)
   {
      oss << " 0x" << std::hex << (int) dataByte;
   }
   std::cout << oss.str() << std::endl;
#endif

#ifdef WIRE_SIMULATION
   std::cout << "WIRE SIMULATION " << __PRETTY_FUNCTION__ << std::endl;
   return theDataBuffer.size();
#endif

   int status = i2c_smbus_write_i2c_block_data(theDevFileFd,
                                               command,
                                               theDataBuffer.size(),
                                               theDataBuffer.data());

   return theDataBuffer.size();
}

uint8_t I2cArduino::write(uint8_t byteOfData)
{
#ifdef WIRE_DEBUGGING_ENABLED
   std::cout << __FUNCTION__ << " single byte = 0x" << std::hex << (int) byteOfData << std::endl;
#endif

   theDataBuffer.push_back(byteOfData);
   return 1;
}

uint8_t I2cArduino::write(char const * string)
{
#ifdef WIRE_DEBUGGING_ENABLED
   std::cout << __FUNCTION__ << std::endl;
#endif
   int i = 0;
   while(string[0] != 0)
   {
      theDataBuffer.push_back(string[0]);
      i++;
      string++;
   }

   theDataBuffer.push_back(0); // write the null terminator
   return i + 1;
}

uint8_t I2cArduino::write(char const * dataBuf, uint8_t length)
{
   std::cout << __FUNCTION__ << std::endl;
   assert(false);

   return 0;
}

uint8_t I2cArduino::available()
{
   std::cout << __FUNCTION__ << std::endl;
   assert(false);

   return 0;
}

uint8_t I2cArduino::read()
{
   std::cout << __FUNCTION__ << std::endl;
   assert(false);

   return 0;
}

void I2cArduino::SetAddress(uint8_t address)
{
#ifdef WIRE_SIMULATION
   return;
#endif

   if (address == theAddress)
   {
      return;
   }

   if (ioctl(theDevFileFd, I2C_SLAVE, address) < 0)
   {
      std::cerr << "Error setting the I2c address (" << std::hex << (int) address
                << std::dec << "): " << strerror(errno) << std::endl;
   }
   else
   {
      std::cout << "Set I2c address to " << std::hex << (int) address << std::dec << std::endl;
      theAddress = address;
   }
}

}









