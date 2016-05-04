#include "GpioIo.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>

namespace Arduino
{

const char* GPIO_SYSFS_EXPORT_PATH = "/sys/class/gpio/export";

const char* GPIO_SYSFS_PIN_DIR = "/sys/class/gpio/gpio"; // Immediatly followed by number

const char* GPIO_SYSFS_ACTIVE_LOW_FILE = "active_low";
const char* GPIO_SYSFS_DIRECTION_FILE = "direction";
const char* GPIO_SYSFS_EDGE_FILE = "edge";
const char* GPIO_SYSFS_VALUE_FILE = "value";


GpioIo::GpioIo(int pin, ArduinoGpioMode mode, ArduinoGpioOutputLevel lvl):
   thePin(pin),
   theMode(mode)
{
   exportGpio();
   setMode(theMode);

   // Open the GPIO value file
   std::ostringstream valueFilename;
   valueFilename << GPIO_SYSFS_PIN_DIR << thePin << "/" << GPIO_SYSFS_VALUE_FILE;
   theGpioValueFile.open(valueFilename.str().c_str(), std::ios::in | std::ios::out);
   if (!theGpioValueFile.is_open())
   {
      // If the GPIO value file wasn't successfully opened, throw exception
      throw(new std::runtime_error("GPIO value file did not open"));
   }

   if (theMode == OUTPUT)
   {
      setLevel(lvl);
   }
}

GpioIo::~GpioIo()
{

}

void GpioIo::setLevel(ArduinoGpioOutputLevel lvl)
{
   if (theMode != OUTPUT)
   {
      std::cerr << "Error: GPIO " << thePin << "isn't configured for output" << std::endl;
      return;
   }

   theGpioValueFile << (lvl == LOW ? 0 : 1);
   theGpioValueFile.seekp(std::ios::beg);
}

bool GpioIo::getLevel(ArduinoGpioOutputLevel* lvl)
{
   return true;
}

void GpioIo::setMode(ArduinoGpioMode mode)
{
   std::fstream directionFile;

   std::ostringstream dirFilename;
   dirFilename << GPIO_SYSFS_PIN_DIR << thePin << "/" << GPIO_SYSFS_DIRECTION_FILE;
   directionFile.open(dirFilename.str().c_str(), std::ios::out);

   if (mode == INPUT)
   {
      directionFile << "in";
   }
   else
   {
      directionFile << "out";
   }
}

void GpioIo::exportGpio()
{
   std::fstream exportFile;
   exportFile.open(GPIO_SYSFS_EXPORT_PATH, std::ios::in | std::ios::out);

   exportFile << thePin;

   exportFile.close();

   // If GPIO file doesn't appear, throw exception
   std::ostringstream gpioPinDirName;
   gpioPinDirName << GPIO_SYSFS_PIN_DIR << thePin;

   struct stat gpioFileInfo;
   if (stat(gpioPinDirName.str().c_str(), &gpioFileInfo))
   {
      std::cerr << "Export of pin " << thePin << " failed" << std::endl;
      throw(std::invalid_argument("pin number"));
   }
}

}
