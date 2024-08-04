#####################################################################################
# Build instructions for Bianconi-Barabasi model generating function in C++
#####################################################################################

set(CMAKE_CXX_STANDARD 23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

cmake_minimum_required(VERSION 3.5.0)
project(BBnetworkModel${CMAKE_SYSTEM_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_library(BBnetworkModel${CMAKE_SYSTEM_NAME} SHARED src/NetworkComponents.cpp src/Generator.cpp src/Network.cpp src/test.cpp)

target_include_directories( BBnetworkModel${CMAKE_SYSTEM_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
