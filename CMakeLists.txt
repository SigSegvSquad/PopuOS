cmake_minimum_required(VERSION 3.17)
project(PopuOS)

set(CMAKE_CXX_STANDARD 14)

add_executable(PopuOS main.cpp OperatingSystem/RegisterBank.cpp OperatingSystem/RegisterBank.h OperatingSystem/Utility.h OperatingSystem/Processor.cpp OperatingSystem/Processor.h)