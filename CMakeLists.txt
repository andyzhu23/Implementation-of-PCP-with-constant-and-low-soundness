# CMakeLists.txt
#
# Author: Andy Zhu
# @date    2025-10-16 14:12:59
# @version 1.0.0
#

cmake_minimum_required(VERSION 3.10)
project(PCP_Project LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Toggle additional consistency constraints inside the PCP constructions
option(ENFORCE_CONSISTENCY "Enable consistency-enforcing constraints" ON)
if(ENFORCE_CONSISTENCY)
	add_compile_definitions(ENFORCE_CONSISTENCY)
endif()

# Put all build outputs (binaries, libraries, objects) under the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/test)