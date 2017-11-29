cmake_minimum_required(VERSION 2.8)

# Reference: https://phonzia.github.io/2015/12/CMake

project(rk3399-face-detect-db)

# Add compile options
set(CMAKE_CXX_FLAGS "-std=c++11")

# Add DEBUG and RELEASE to g++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE")

# validate build type and get build type string be lowered to setting target file name
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if (BUILD_TYPE STREQUAL "debug")
	message("-- cmake in ${BUILD_TYPE} mode")
elseif(BUILD_TYPE STREQUAL "release")
	message("-- cmake in ${BUILD_TYPE} mode")
else()
	message(FATAL_ERROR "fatal: unknwon CMAKE_BUILD_TYPE: \"${CMAKE_BUILD_TYPE}\" (available type: debug/release)")
endif()

# scan all files in this folder to set into variable: ${SOURCE_FILES}
aux_source_directory(. SOURCE_FILES)
# add_executable("${PROJECT_NAME}.${BUILD_TYPE}" ${SOURCE_FILES})
add_library("${PROJECT_NAME}.${BUILD_TYPE}" SHARED ${SOURCE_FILES})
