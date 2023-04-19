cmake_minimum_required(VERSION 3.0.0)
project(Laba1LOIS VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(-O2)

file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "src/*.cpp"
  "src/*.h"
)

add_executable(Laba1LOIS ${SOURCES} main.cpp)
