cmake_minimum_required(VERSION 3.18)

project(RDA5807M C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(MCU atmega328p)
set(CMAKE_CXX_COMPILER ${AVR_GCC_PATH}/avr-g++)
set(CMAKE_OBJCOPY ${AVR_GCC_PATH}/avr-objcopy)
set(CMAKE_CXX_FLAGS "-mmcu=${MCU} -DF_CPU=8000000UL -Os")

add_library(RDA5807M STATIC src/RDA5807.c)

target_include_directories(
  RDA5807M PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_command(TARGET RDA5807M POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom RDA5807M main.hex
  COMMENT "Generating main.hex"
)
