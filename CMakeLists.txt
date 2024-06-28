cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME G431LedBlink)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

set(FIRMWARE_NAME firmware)

# Create an executable object type
add_executable(
    ${FIRMWARE_NAME}
    src/main.cpp
    src/gpio.c
    src/tasks.cpp
    src/usart.c
    src/tim.c
    src/stm32g4xx_it.cpp
    src/stm32g4xx_hal_msp.c
    src/stm32g4xx_hal_timebase_tim.c
    src/sysmem.c
    src/syscalls.c
    src/system_stm32g4xx.c
    src/PeriphWrappers.cpp
    src/Device.cpp
    src/ST7567_StateMachine.cpp
    src/SSD1306.cpp
    src/SH1106.cpp
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${FIRMWARE_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${FIRMWARE_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${FIRMWARE_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${FIRMWARE_NAME} PRIVATE
    # Add user defined symbols
)

target_compile_options(${FIRMWARE_NAME} PRIVATE
    -Wno-missing-field-initializers
)

# Add linked libraries
target_link_libraries(${FIRMWARE_NAME}
    stm32cubemx

    # Add user defined libraries
)


add_custom_command(
    TARGET ${FIRMWARE_NAME}
    POST_BUILD
    BYPRODUCTS ${FIRMWARE_NAME}.bin
    DEPENDS ${FIRMWARE_NAME}
    COMMENT "generate .bin file"
    COMMAND ${CMAKE_OBJCOPY} -O binary "$<TARGET_FILE:${FIRMWARE_NAME}>" "$<TARGET_FILE:${FIRMWARE_NAME}>.bin"
    VERBATIM
)

add_custom_target(
    flash-${FIRMWARE_NAME}
    DEPENDS ${FIRMWARE_NAME}.bin
    COMMAND st-flash --reset write  "$<TARGET_FILE:${FIRMWARE_NAME}>.bin" 0x08000000 
)