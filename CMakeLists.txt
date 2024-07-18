#####################################################################################
# Build instructions for Bianconi-Barabasi model generating function in C++
#####################################################################################

set(CMAKE_CXX_STANDARD 23)


set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

cmake_minimum_required(VERSION 3.5.0)
project(bbmodel VERSION 0.1.0 LANGUAGES CXX)

add_executable(bbmodel src/main.cpp)

target_include_directories( bbmodel
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
