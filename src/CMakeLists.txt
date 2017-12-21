cmake_minimum_required(VERSION 2.8)

# Reference: https://phonzia.github.io/2015/12/CMake

option(CMAKE_FOR_ARM "Set this variable to ON if you compile this project on arm board" OFF)

project(rk3399-face-detect-db)
set(TARGET_LIB_NAME rk3399_face_detect_db)

# Get current timestamp:
string(TIMESTAMP NOW_DATE_STR "%Y-%m-%d %H:%M:%S")

# Add compile options
# -std=c++11            use c++ 11 standard
set(CMAKE_CXX_FLAGS "-std=c++11 -Werror -DCOMPILE_DATE='\"${NOW_DATE_STR}\"' ")

# Add DEBUG and RELEASE to g++
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DRELEASE -O2 -s")

# validate build type and get build type string be lowered to setting target file name
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
if (BUILD_TYPE STREQUAL "debug")
	message("-- cmake in ${BUILD_TYPE} mode")
elseif(BUILD_TYPE STREQUAL "release")
	message("-- cmake in ${BUILD_TYPE} mode")
else()
	message(FATAL_ERROR "fatal: unknwon CMAKE_BUILD_TYPE: \"${CMAKE_BUILD_TYPE}\" (available type: debug/release)")
endif()

if (CMAKE_FOR_ARM)
	message("CMake for aarch64(arm) platform ...")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFOR_ARM")
else()
	message("Linux-x86-64")
endif()

# libraries
find_package( OpenCV REQUIRED )
link_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/../libs/arm64-ff-face-sdk"
    "${CMAKE_CURRENT_SOURCE_DIR}/../libs" )

# scan all files in this folder to set into variable: ${SOURCE_FILES}
file(GLOB_RECURSE SOURCE_FILES .
	"api.cc"
	"face/*.cc"
	"global/*.cc"
	"include/*.cc"
	"log/*.cc"
	"media/*.cc"
	"storage/*.cc"
	"types/*.cc"
	"utils/*.cc")

set(TARGET_SHARED_LIB "${TARGET_LIB_NAME}_${BUILD_TYPE}")
add_library(${TARGET_SHARED_LIB} SHARED ${SOURCE_FILES})
target_link_libraries(${TARGET_SHARED_LIB} ${OpenCV_LIBS})

if (CMAKE_FOR_ARM)
	# Link firefly face SDK
	target_link_libraries( ${TARGET_SHARED_LIB}
		rr_facedetect_3_0
		rr_faceverify_3_0
		arcsoft_fsdk_face_detection
		arcsoft_fsdk_face_recognition
		firefly_faceverify)
endif()

# using export symbols file
# https://www.gnu.org/software/gnulib/manual/html_node/LD-Version-Scripts.html
set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} "-Wl,--version-script=${CMAKE_SOURCE_DIR}/export_symbols.map")
# gcc -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# db-client executable file
file(GLOB_RECURSE DB_CLIENT_FILES . "db-client/*.cc")
add_executable("db-client" ${DB_CLIENT_FILES})

