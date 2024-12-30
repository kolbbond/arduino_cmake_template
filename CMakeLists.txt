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

## add executable
add_executable(hello_world hello_world.cpp)
target_link_arduino_libraries(hello_world PRIVATE core ESP8266WiFi)

add_executable(esp8266_blink esp8266_blink.cpp)
target_link_arduino_libraries(esp8266_blink PRIVATE core ESP8266WiFi)

add_executable(esp8266server esp8266server.cpp)
target_link_arduino_libraries(esp8266server PRIVATE 
    core 
    ESP8266WiFi
    ESP8266WebServer
    ESP8266mDNS
)

add_executable(esp8266_dht11 esp8266_dht11.cpp)
target_link_arduino_libraries(esp8266_dht11 PRIVATE core ESP8266WiFi Adafruit_Unified_Sensor DHT )

add_executable(esp8266_dht22 esp8266_dht22.cpp)
target_link_arduino_libraries(esp8266_dht22 PRIVATE core ESP8266WiFi Adafruit_Unified_Sensor DHT )

# This is needed for the generation of HEX binary and uploading it
target_enable_arduino_upload(hello_world)
target_enable_arduino_upload(esp8266server)
target_enable_arduino_upload(esp8266_blink)
target_enable_arduino_upload(esp8266_dht11)
target_enable_arduino_upload(esp8266_dht22)

