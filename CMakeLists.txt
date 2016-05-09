cmake_minimum_required (VERSION 2.6)


set(SOURCE_FILES ArduinoOnLinux.cpp GpioIo.cpp)

add_library(arduino ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11 -Wall")


set(CMAKE_CXX_COMPILER "clang++")


