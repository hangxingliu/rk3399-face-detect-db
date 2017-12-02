cmake_minimum_required(VERSION 2.8)

# Reference: https://phonzia.github.io/2015/12/CMake

project(rk3399-face-detect-db-test)

# Add compile options
set(CMAKE_CXX_FLAGS "-std=c++11")

# Add DEBUG and RELEASE to g++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

set(EXT_NAME "test.bin")
file(GLOB_RECURSE UTILS ./utils "**/*.cc" "**/*.cpp" "**/*.c")
add_executable("1.${EXT_NAME}" cpp_test.cc ${UTILS})
add_executable("jna-struct-test.${EXT_NAME}" jna_struct_test_so.cc ${UTILS})
add_library("jna-struct-test" SHARED jna_struct_test_so.cc)
