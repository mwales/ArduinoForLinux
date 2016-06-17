#include "GpioIo.h"
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include <cstring>

#include <stdio.h>
#include <poll.h>
#include <fcntl.h>
#include <pthread.h>

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
   theMode(mode),
   theIsrCreated(false),
   theThreadStopEvent(false)
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

   bzero(&theIsrThread, sizeof(pthread_t));
}

GpioIo::~GpioIo()
{
   theThreadStopEvent = true;

   if (theIsrCreated)
   {
      std::cout << "Stopping ISR(" << thePin << ")" << std::endl;
      pthread_join(theIsrThread, NULL);
      theIsrCreated = false;

      std::cout << "ISR(" << thePin << ") cleanup complete" << std::endl;
   }
   else
   {
      std::cout << "No ISR(" << thePin << ") thread to cleanup" << std::endl;
   }
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

ArduinoGpioOutputLevel GpioIo::getLevel()
{
   int val;
   theGpioValueFile >> val;
   theGpioValueFile.seekg(std::ios::beg);

   return (val ? HIGH : LOW);
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

void* isrThreadMain(void* gpioParam)
{
   GpioIo* obj;
   obj = (GpioIo*) gpioParam;

   std::cout << __PRETTY_FUNCTION__ << " ISR thread started" << std::endl;

   std::ostringstream valueFileName;
   valueFileName << GPIO_SYSFS_PIN_DIR << obj->thePin << "/" << GPIO_SYSFS_VALUE_FILE;

   int fd = open(valueFileName.str().c_str(), O_RDONLY);

   if (fd == -1)
   {
      std::cerr << "Error opening " << valueFileName.str() << " for ISR polling" << std::endl
                << "  Msg = " << strerror(errno) << std::endl;
      return 0;
   }

   // Setup the event polling structure
   struct pollfd eventsToPoll;
   bzero(&eventsToPoll, sizeof(eventsToPoll));
   eventsToPoll.fd = fd;
   eventsToPoll.events = POLLPRI;

   // Buffer to read the file into when interrupt occurs to clear interrupt
   const int BUF_LEN = 5;
   char* buf[BUF_LEN];
   memset(buf, 0, BUF_LEN);

   bool discardFirstPoll = true;

   while(!obj->theThreadStopEvent)
   {
      eventsToPoll.fd = fd;
      eventsToPoll.events = POLLPRI;

      int success = poll(&eventsToPoll, 1, 5000);

      if (success > 0)
      {
         // GPIO interrupt

         // The following is just a bunch of debug code

//         std::cout << "poll returned" << std::endl;

//         if (eventsToPoll.revents & POLLIN)
//         {
//            std::cout << "poll POLLIN set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLPRI)
//         {
//            std::cout << "poll POLLPRI set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLOUT)
//         {
//            std::cout << "poll POLLOUT set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLRDHUP)
//         {
//            std::cout << "poll POLLRDHUP set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLERR)
//         {
//            std::cout << "poll POLLERR set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLHUP)
//         {
//            std::cout << "poll POLLHUP set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLNVAL)
//         {
//            std::cout << "poll POLLNVAL set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLRDNORM)
//         {
//            std::cout << "poll POLLRDNORM set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLRDBAND)
//         {
//            std::cout << "poll POLLRDBAND set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLWRNORM)
//         {
//            std::cout << "poll POLLWRNORM set" << std::endl;
//         }

//         if (eventsToPoll.revents & POLLWRBAND)
//         {
//            std::cout << "poll POLLWRBAND set" << std::endl;
//         }

         int numBytesRead = read(fd, buf, BUF_LEN-1);

         // std::cout << "Read " << numBytesRead << " bytes, revents = " << eventsToPoll.revents << std::endl; // << QString(*buf);

         if(-1 == lseek(fd, 0, SEEK_SET))
         {
            std::cout << "lseek error: " << strerror(errno) << std::endl;
         }

         if(discardFirstPoll)
         {
            std::cout << "Discarded the first one" << std::endl;
            discardFirstPoll = false;
            continue;
         }

         std::cout << "Calling ISR(" << obj->thePin << ") callback" << std::endl;
         obj->theIsrCallbackFunc();

      }
      else if (success == 0)
      {
         std::cout << __PRETTY_FUNCTION__ << ":  Poll timed out" << std::endl;
      }
      else
      {
         // Error
         std::cout << __PRETTY_FUNCTION__ << ":  Poll call returned error" << success << std::endl;
      }
   }

   return 0;
}

void GpioIo::attachInterrupt(void (*gpioIsr)(), ArduinoGpioInterruptEdge edgeCondition)
{
   std::cout << __PRETTY_FUNCTION__ << " for pin " << thePin << std::endl;

   // Set trigger edge
   std::fstream edgeFile;

   std::ostringstream edgeFileName;
   edgeFileName << GPIO_SYSFS_PIN_DIR << thePin << "/" << GPIO_SYSFS_EDGE_FILE;
   edgeFile.open(edgeFileName.str().c_str(), std::ios::out);

   switch (edgeCondition)
   {
      case RISING:
         edgeFile << "rising";
         break;
      case FALLING:
         edgeFile << "falling";
         break;
      case CHANGE:
         edgeFile << "both";
         break;
      default:
         edgeFile << "none";
         break;
   }

   std::cout << "About to start ISR thread for pin " << thePin << std::endl;

   if (!theIsrCreated)
   {
      // theIsrThread = new std::thread(isrThreadMain, this);
      int success = pthread_create(&theIsrThread, NULL, &isrThreadMain, (void*) this);
      if (success != 0)
      {
         std::cerr << "Failed to create the ISR thread for pin " << thePin << std::endl;
      }
      else
      {
         std::cout << "Thread created successfully for pin " << thePin << std::endl;
         theIsrCreated = true;
      }
   }
   else
   {
      std::cerr << __PRETTY_FUNCTION__ << " for pin " << thePin << " already had ISR attached" << std::endl;
   }

   // Save the callback method
   theIsrCallbackFunc = gpioIsr;
}

}
