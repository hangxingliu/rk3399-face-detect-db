
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
// Generate time: 2017/12/07 23:41:08

#include "opencv2/opencv.hpp"
#include "./opencv.hpp"

#ifndef UTILS_HPP
#define UTILS_HPP 1

//  ====================
//  @file ./src/utils/convert2string.cc
//  ====================

//  source: ./src/utils/convert2string.cc:8
void addQuote2String(const char* value, char* result);

//  source: ./src/utils/convert2string.cc:4
void int2String(int value, char* result, bool withQuote);

//  ====================
//  @file ./src/utils/memory.cc
//  ====================

//  ====================
//  @file ./src/utils/opencv.cc
//  ====================

//  source: ./src/utils/opencv.cc:16
unsigned char* copyBGRMat2DataArray(cv::Mat & mat, unsigned char* result, int* resultSize, CopyBGRMat2ArrayRule copyRule);

//  source: ./src/utils/opencv.cc:77
bool detectFace(cv::Mat & image, std::vector<cv::Rect> & result, bool grayScaleOptimize);

//  source: ./src/utils/opencv.cc:8
int getCVMatDataSize(cv::Mat* mat);

//  source: ./src/utils/opencv.cc:65
bool initFaceHaarCascade(const char* haarcascadeXML);

//  ====================
//  @file ./src/utils/fs.cc
//  ====================

//  source: ./src/utils/fs.cc:9
bool isFileReadable(const char* path);

//  source: ./src/utils/fs.cc:10
bool isFileWritable(const char* path);

//  source: ./src/utils/fs.cc:12
void joinPath(const char* p0, const char* p1, char* result);

#endif

// md5sum:
// b035f0b48f6bdce35b02ea9c3a9e8b38  ./src/utils/convert2string.cc
// d41d8cd98f00b204e9800998ecf8427e  ./src/utils/memory.cc
// 9c311990f87f2b1d38b17bb0dd01eb19  ./src/utils/opencv.cc
// 47fecd94bc64769ee5d002326d034ecb  ./src/utils/fs.cc
