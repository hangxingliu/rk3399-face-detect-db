#include <stdio.h>

#include "./api.hpp"
#include "../global/global.hpp"

static int draw_face_rect(
	int frameId,
	NON_NULL Detect_FaceRectAttr* attr,
	NON_NULL Detect_FaceInfo* faceInfo,
	API_OUT NEED_TO_FREE ucharArray* newImageData,
	API_OUT int* _w, API_OUT int* _h) {

	CHECK_NON_NULL_FOR_API(attr, "attr");
	CHECK_NON_NULL_FOR_API(faceInfo, "faceInfo");

	cv::Mat image;
	int status = FrameBuffer_cloneBuffer(frameId, image);
	if(status != 0) return status;

	auto w = image.cols, h = image.rows;
	auto color = CV_RGB(attr->r, attr->g, attr->b);
	auto p0 = cv::Point(faceInfo->x0, faceInfo->y0);
	auto p1 = cv::Point(faceInfo->x1, faceInfo->y1);
	cv::rectangle(image, p0, p1, color, attr->thickness);

	if(faceInfo->matched && faceInfo->matchedUserId) {
		int xText = faceInfo->x0, yText = faceInfo->y0 - 10;
		if(yText < 20) yText = faceInfo->y1 + 10;
		cv::Point pText(xText, yText);
		cv::putText(image, faceInfo->matchedUserId, pText,
			cv::FONT_HERSHEY_DUPLEX, 2.0f, color);
	}

	size_t size = image.cols * image.rows * image.channels();
	ucharArray data = (ucharArray) malloc(size);
	memcpy(data, image.data, size);

	*newImageData = data;
	if(w) *_w = w;
	if(h) *_h = h;
	return API_OK;
}

// ==========================================
//    E x p o r t   F u n c t i o n s

static const char* API_NAME = "draw_face_rect";
int face_draw_face_rect(
	int frameId,
	NON_NULL Detect_FaceRectAttr* attr,
	NON_NULL Detect_FaceInfo* faceInfo,
	API_OUT NEED_TO_FREE ucharArray* newImageData,
	API_OUT int* _w, API_OUT int* _h) {

	LOG_API_INVOKE(API_NAME, "%d, %p, %p, %p, %p, %p",
		frameId, attr, faceInfo, newImageData, _w, _h);
	APITimer timer(API_NAME);

	int status = draw_face_rect(frameId, attr, faceInfo, newImageData, _w, _h);

	timer.click();
	return status;
}
