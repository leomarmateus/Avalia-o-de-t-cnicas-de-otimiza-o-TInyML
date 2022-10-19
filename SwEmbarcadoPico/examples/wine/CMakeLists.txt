
cmake_minimum_required(VERSION 3.12)

project(wine C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(wine "")

target_include_directories(wine
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/.
)

set_target_properties(
  wine
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

target_sources(wine
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/main_functions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/debug.cpp
  ${CMAKE_CURRENT_LIST_DIR}/LoRa-RP2040.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Print.cpp
  ${CMAKE_CURRENT_LIST_DIR}/transmitter.cpp
  ${CMAKE_CURRENT_LIST_DIR}/tempo.cpp
  ${CMAKE_CURRENT_LIST_DIR}/main_functions.h
  ${CMAKE_CURRENT_LIST_DIR}/model.h
  ${CMAKE_CURRENT_LIST_DIR}/debug.h
  ${CMAKE_CURRENT_LIST_DIR}/LoRa-RP2040.h
  ${CMAKE_CURRENT_LIST_DIR}/Print.h
  ${CMAKE_CURRENT_LIST_DIR}/transmitter.h
  ${CMAKE_CURRENT_LIST_DIR}/data_base.h
  ${CMAKE_CURRENT_LIST_DIR}/tempo.h
  
)

target_link_libraries(
  wine
  pico-tflmicro
  hardware_pwm
  pico_stdlib
  hardware_spi
  hardware_rtc
)

# enable usb output, disable uart output
pico_enable_stdio_usb(wine 1)
pico_enable_stdio_uart(wine 0)

pico_add_extra_outputs(wine)