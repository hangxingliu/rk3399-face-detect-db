cmake_minimum_required(VERSION 2.8)

# Reference: https://phonzia.github.io/2015/12/CMake

project(rk3399-face-detect-db-test)

# Add compile options
set(CMAKE_CXX_FLAGS "-std=c++11")

# Add DEBUG and RELEASE to g++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../build/release"
	"${CMAKE_CURRENT_SOURCE_DIR}/../libs" )
find_package( OpenCV REQUIRED )

set(EXT_NAME "test.bin")
file(GLOB_RECURSE UTILS . "utils/*.cc" "utils/*.cpp" "utils/*.c")

# GUI Test

add_executable("opencv_base_test.gui" "gui/opencv_test.cc")
target_link_libraries("opencv_base_test.gui" ${OpenCV_LIBS} )

# Automatic Unit Test

add_executable("cpp_base.${EXT_NAME}" cpp_test.cc ${UTILS})

add_executable("call_release_so.${EXT_NAME}" call_release_so.test.cc)
target_link_libraries("call_release_so.${EXT_NAME}" rk3399-face-detect-db-release)

add_executable("jna-struct-test.${EXT_NAME}" jna_struct_test_so.cc ${UTILS})
add_library("jna-struct-test" SHARED jna_struct_test_so.cc)
