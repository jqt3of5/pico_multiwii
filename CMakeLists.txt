cmake_minimum_required(VERSION 3.16)
# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_msp C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 20)

#add_executable(pico_msp main.cpp)
add_subdirectory(./src)

# Enable USB serial
pico_enable_stdio_usb(pico_msp 1)
pico_enable_stdio_uart(pico_msp 0)



