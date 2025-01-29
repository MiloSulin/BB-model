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
project(BBnetworkModel${CMAKE_SYSTEM_NAME} VERSION 0.5.0 LANGUAGES CXX)

# add_executable(BBnetworkModel${CMAKE_SYSTEM_NAME}  src/NetworkComponents.cpp src/Generator.cpp src/Network.cpp src/test.cpp)
add_library(BBnetworkModel${CMAKE_SYSTEM_NAME} SHARED src/NetworkComponents.cpp src/Generator.cpp src/Network.cpp src/bbModelCapi.cpp)

target_include_directories( BBnetworkModel${CMAKE_SYSTEM_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_link_libraries( BBnetworkModel${CMAKE_SYSTEM_NAME} -static-libgcc -static-libstdc++)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
target_link_libraries( BBnetworkModel${CMAKE_SYSTEM_NAME} -std=libc++)
endif()
