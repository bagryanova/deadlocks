cmake_minimum_required(VERSION 3.0)

project(deadlocks VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include_directories(
    src
)

file(GLOB SOURCES
    "src/*.h"
    "src/*.cpp"
)

add_executable(deadlocks ${SOURCES})
