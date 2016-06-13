#pragma once

namespace Arduino
{

//enum BASE
//{
//   DEC,
//   HEX,
//   OCT,
//   BIN
//};

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

class SerialOutput
{
public:

   void print(char const * str);

   void print(char c);

   void print(int num);

   void print(int num, int base);

   void println(char const* str);

   void println(char c);

   void println(int num);

   void println(int num, int base);

   void begin(int baudRate);

   void flush();
};

extern SerialOutput Serial;

}
