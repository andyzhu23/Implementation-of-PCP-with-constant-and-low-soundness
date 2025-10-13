cmake_minimum_required(VERSION 3.10)
project(PCP_Project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put all build outputs (binaries, libraries, objects) under the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE PCP_SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Build a static library from src
add_library(pcp_core STATIC ${PCP_SRC})

# Add test executable
add_executable(power_operation_test test/power_operation_test.cpp)
target_link_libraries(power_operation_test PRIVATE pcp_core)

