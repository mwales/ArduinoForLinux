cmake_minimum_required (VERSION 2.6)

set(SOURCE_FILES ArduinoOnLinux.cpp 
                 GpioIo.cpp 
                 ArduinoSerial.cpp 
                 SPI.cpp
                 Wire.cpp
                 )

add_library(arduino ${SOURCE_FILES})

find_package (Threads)
target_link_libraries (arduino ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall")

set(CMAKE_CXX_COMPILER "clang++")


