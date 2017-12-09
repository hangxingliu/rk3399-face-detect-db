
// 888       888        d8888 8888888b.  888b    888 8888888 888b    888  .d8888b.   888 888 888
// 888   o   888       d88888 888   Y88b 8888b   888   888   8888b   888 d88P  Y88b  888 888 888
// 888  d8b  888      d88P888 888    888 88888b  888   888   88888b  888 888    888  888 888 888
// 888 d888b 888     d88P 888 888   d88P 888Y88b 888   888   888Y88b 888 888         888 888 888
// 888d88888b888    d88P  888 8888888P"  888 Y88b888   888   888 Y88b888 888  88888  888 888 888
// 88888P Y88888   d88P   888 888 T88b   888  Y88888   888   888  Y88888 888    888  Y8P Y8P Y8P
// 8888P   Y8888  d8888888888 888  T88b  888   Y8888   888   888   Y8888 Y88b  d88P   "   "   " 
// 888P     Y888 d88P     888 888   T88b 888    Y888 8888888 888    Y888  "Y8888P88  888 888 888
// 
// 
// This header file is generated by script.
// DON'T modify manually!
// Generate time: 2017/12/09 15:56:55

#include "opencv2/opencv.hpp"
#include "./_constant.hpp"
#include "../types/config.hpp"

#ifndef GLOBAL_GLOBAL_HPP
#define GLOBAL_GLOBAL_HPP 1

//  ====================
//  @file ./src/global/configurations.cc
//  ====================

//  source: ./src/global/configurations.cc:10
int Config_initGlobalConfig(GlobalConfig* mergeInto);

//  source: ./src/global/configurations.cc:29
int Config_mergeConfig(GlobalConfig* mergeTo, GlobalConfig* overwrite);

//  ====================
//  @file ./src/global/frame_buffer.cc
//  ====================

//  source: ./src/global/frame_buffer.cc:28
cv::Mat* FrameBuffer_giveMeBuffer(int* bufferId);

//  source: ./src/global/frame_buffer.cc:12
int FrameBuffer_init();

//  source: ./src/global/frame_buffer.cc:19
bool FrameBuffer_isBufferValid(int bufferId);

//  source: ./src/global/frame_buffer.cc:26
void FrameBuffer_setInvalid(int bufferId);

//  source: ./src/global/frame_buffer.cc:25
void FrameBuffer_setValid(int bufferId);

//  ====================
//  @file ./src/global/lockers.cc
//  ====================

//  source: ./src/global/lockers.cc:31
void lockDatabase();

//  source: ./src/global/lockers.cc:28
void lockDetectAndExtract();

//  source: ./src/global/lockers.cc:15
void lockFrameAccess(int id);

//  source: ./src/global/lockers.cc:12
void lockFrameSwitcher();

//  source: ./src/global/lockers.cc:32
void unlockDatabase();

//  source: ./src/global/lockers.cc:29
void unlockDetectAndExtract();

//  source: ./src/global/lockers.cc:21
void unlockFrameAccess(int id);

//  source: ./src/global/lockers.cc:13
void unlockFrameSwitcher();

#endif

// md5sum:
// 64f6cf0a05b2cf532c7ad1cbc2f8ee39  ./src/global/configurations.cc
// 5690699f986520c02392e298ab075530  ./src/global/frame_buffer.cc
// 2328a5d0735150536c66f7f4c1b755af  ./src/global/lockers.cc
