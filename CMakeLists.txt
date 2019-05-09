cmake_minimum_required(VERSION 3.14)
project(tispp)

set(CMAKE_CXX_STANDARD 11)

add_executable(main.exe main.cpp tispp.hpp)

add_executable(test.exe test.cpp tispp.hpp test.cpp)