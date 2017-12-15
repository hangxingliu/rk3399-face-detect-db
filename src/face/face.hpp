
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
// Generate time: 2017/12/15 11:07:59

#include "../types/base.hpp"
#include "../types/firefly_face_sdk.hpp"
#include "../types/detect.hpp"

#ifndef FACE_HPP
#define FACE_HPP 1

//  ====================
//  @file ./src/face/face.cc
//  ====================

//  source: ./src/face/face.cc:109
float Face_compare(FF_FaceFeatures & f1, FF_FaceFeatures & f2);

//  source: ./src/face/face.cc:53
bool Face_detect(cv::Mat & _image, int resultCapacity, NON_NULL FF_FaceInfo* results, NON_NULL int* resultCount);

//  source: ./src/face/face.cc:80
bool Face_extract(cv::Mat & _image, FF_FaceInfo oneFace, NON_NULL FF_FaceFeatures* result);

//  source: ./src/face/face.cc:43
int Face_getBiggestFaceIndex(FF_FaceInfo* faceArray, int arrayLength);

//  source: ./src/face/face.cc:27
int Face_init();

//  ====================
//  @file ./src/face/type_convert.cc
//  ====================

//  source: ./src/face/type_convert.cc:7
Detect_FaceInfo Detect_FaceInfo_create(FF_FaceInfo* ffFaceInfo, FF_FaceFeatures* ffFaceFeatures, bool matched, float matchedScore, const char* matchedUserId);

//  source: ./src/face/type_convert.cc:43
void FF_FaceFeatures_create(int len, const float* data, FF_FaceFeatures* instance);

#endif

// md5sum:
// c3623eb651ce651fac6490acf0c042ce  ./src/face/face.cc
// ea9c81b32ceb320050f4ab608e94df40  ./src/face/type_convert.cc
