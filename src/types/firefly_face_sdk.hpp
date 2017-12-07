#include "./base.hpp"

#ifndef PUBLIC_TYPES_FIREFLY_FACE_HPP
#define PUBLIC_TYPES_FIREFLY_FACE_HPP 1

#ifdef __cplusplus
extern "C" {
#endif

typedef void * FF_Detector;
typedef void * FF_Verifier;

typedef enum {
	FF_IMAGE_BGR8UC3 = 0,	///< BGR  8:8:8   24bpp ( 3通道24bit BGR 像素 )
	FF_IMAGE_BGRA8UC4 = 1,	///< BGRA 8:8:8:8 32bpp ( 4通道32bit BGRA 像素 )
	FF_IMAGE_GRAY8UC1 = 2,	///< Y    1        8bpp ( 单通道8bit灰度像素 )
	FF_IMAGE_MAX
} FF_ImageType;


/**
 * Firefly Face SDK Rect(left, top, right, bottom)
 */
typedef struct FF_Rect {
	int left;	///< 矩形左边的坐标
	int top;	///< 矩形顶边的坐标
	int right;	///< 矩形右边的坐标
	int bottom;	///< 矩形底边的坐标
} FF_Rect;


/**
 * Firefly Face SDK Image(OpenCVImageData, imageType, w, h)
 */
typedef struct FF_Image {
	unsigned char *data;
	FF_ImageType type;
	int width;
	int height;
} FF_Image;

/**
 * Firefly Face SDK Point(x, y)
 */
typedef struct FF_Point {
	float x;
	float y;
} FF_Point;

/**
 * Firefly Face SDK Face Info(rect, confidence, landmarks[5],  y,p,r, id, quality )
 */
typedef struct FF_FaceInfo {
	FF_Rect rect;
	/** chinese: 置信度 */
	float confidence;
	FF_Point landmarks[5];

	int yaw;
	int pitch;
	int roll;

	int id;

	float quality;
} FF_FaceInfo;


/**
 * Firefly Face SDK Face Features(len, data[6000])
 */
typedef struct FF_FaceFeatures {
	int len;
	float data[FACE_FEATURE_LENGTH];
} FF_FaceFeatures;

#ifdef __cplusplus
}
#endif

#endif
