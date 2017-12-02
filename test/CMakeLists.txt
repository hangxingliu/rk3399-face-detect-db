cmake_minimum_required(VERSION 2.8)

# Reference: https://phonzia.github.io/2015/12/CMake

project(rk3399-face-detect-db-test)

# Add compile options
set(CMAKE_CXX_FLAGS "-std=c++11")

# Add DEBUG and RELEASE to g++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set(EXT_NAME "test.bin")
add_executable("1.${EXT_NAME}" cpp_test.cc utils/index.cc)
