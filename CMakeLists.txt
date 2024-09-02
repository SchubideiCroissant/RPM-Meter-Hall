# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Hall_Sensor_Adc C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(Hall_Sensor_Adc Hall_Sensor_Adc.cpp )

pico_set_program_name(Hall_Sensor_Adc "Hall_Sensor_Adc")
pico_set_program_version(Hall_Sensor_Adc "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(Hall_Sensor_Adc 0)
pico_enable_stdio_usb(Hall_Sensor_Adc 1)

# Add the standard library to the build
target_link_libraries(Hall_Sensor_Adc
        pico_stdlib hardware_adc)

# Add the standard include files to the build
target_include_directories(Hall_Sensor_Adc PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts or any other standard includes, if required
)

pico_add_extra_outputs(Hall_Sensor_Adc)

