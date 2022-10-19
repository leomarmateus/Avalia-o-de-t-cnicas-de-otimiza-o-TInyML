
cmake_minimum_required(VERSION 3.12)

project(case1 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

add_executable(case1 "")

target_include_directories(case1
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/.
)

set_target_properties(
  case1
  PROPERTIES
  COMPILE_FLAGS -fno-rtti
  COMPILE_FLAGS -fno-exceptions
  COMPILE_FLAGS -fno-threadsafe-statics
  COMPILE_FLAGS -nostdlib
)

target_sources(case1
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
  case1
  pico-tflmicro
  hardware_pwm
  pico_stdlib
  hardware_spi
  hardware_rtc
)

# enable usb output, disable uart output
pico_enable_stdio_usb(case1 1)
pico_enable_stdio_uart(case1 0)

pico_add_extra_outputs(case1)