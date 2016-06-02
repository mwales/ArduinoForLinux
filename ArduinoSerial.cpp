#include "ArduinoSerial.h"

#include <iostream>


namespace Arduino
{

SerialOutput Serial;

void SerialOutput::print(char const * str)
{
   std::cout << str;
}

void SerialOutput::print(char c)
{
   std::cout << c;
}

void SerialOutput::print(int num)
{
   std::cout << num;
}

void SerialOutput::print(int num, int base)
{
   std::cout << num;
}

void SerialOutput::println(char const * str)
{
   std::cout << str << std::endl;
}

void SerialOutput::println(char c)
{
   std::cout << c << std::endl;
}

void SerialOutput::println(int num)
{
   std::cout << num << std::endl;
}

void SerialOutput::println(int num, int base)
{
   std::cout << num << std::endl;
}

}
