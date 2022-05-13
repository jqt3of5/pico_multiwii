cmake_minimum_required(VERSION 3.16)
project(pico_msp C CXX ASM)

include(pico-sdk/pico_sdk_init.cmake)
pico_sdk_init()


set(CMAKE_CXX_STANDARD 20)

#add_executable(pico_msp main.cpp)
add_subdirectory(./src)


# Enable USB serial
pico_enable_stdio_usb(pico_msp 1)
pico_enable_stdio_uart(pico_msp 0)



