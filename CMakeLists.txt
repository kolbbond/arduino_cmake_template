cmake_minimum_required(VERSION 3.0.0)

# get arduino toolchain file
include(FetchContent)
FetchContent_Declare(arduinotoolchain
    GIT_REPOSITORY "https://github.com/kolbbond/Arduino-CMake-Toolchain.git"
    GIT_PROGRESS TRUE
    GIT_TAG origin/dev
)
FetchContent_GetProperties(arduinotoolchain)
if(NOT arduinotoolchain_POPULATED)
    message("Downloading Arduino Toolchain")
    FetchContent_Populate(arduinotoolchain)
endif()

set(CMAKE_TOOLCHAIN_FILE "${arduinotoolchain_SOURCE_DIR}/Arduino-toolchain.cmake")
message("using toolchain: ${CMAKE_TOOLCHAIN_FILE}")

# rest
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(hello_world CXX)

add_executable(hello_world hello_world.cpp)

# Arduino.h is included in hello_world.cpp, so link with Arduino core
target_link_arduino_libraries(hello_world PRIVATE core)

# This is needed for the generation of HEX binary and uploading it
target_enable_arduino_upload(hello_world)

