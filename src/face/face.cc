
#include "../include/firefly_faceverify.hpp"

#include "../types/base.hpp"
#include "../types/firefly_face_sdk.hpp"
#include "../types/api_error_no.hpp"

#include "./face_addon.hpp"

#include "opencv2/opencv.hpp"

#define FACE_FATAL(...) char _buf[256]; snprintf(_buf, 256, __VA_ARGS__); LOG_FATAL(_buf);
#define GET_MIN(a,b) (a<b?a:b)

static FF_Detector faceDetector = nullptr;
static FF_Verifier faceVerifier = nullptr;

static FF_Image FF_Image_FromCVMat(cv::Mat &image) {
	FF_Image result;
	result.data = image.data;
	result.type = FF_IMAGE_BGR8UC3;
	result.width = image.cols;
	result.height = image.rows;
	return result;
}

int Face_init() {
	if(!faceDetector)
		faceDetector = firefly_fd_init_detector();
	if(!faceDetector)
		return API_FACE_INIT_FD_FAILED;
	LOG_INFO("firefly faceDetector initialized!");

	if(!faceVerifier)
		faceVerifier = firefly_fv_init_verifier();
	if(!faceVerifier)
		return API_FACE_INIT_FV_FAILED;
	LOG_INFO("firefly faceVerifier initialized!");

	return API_OK;
}

int Face_getBiggestFaceIndex(
	FF_FaceInfo* faceArray,
	int arrayLength) {
	MaxRect max;
	LOOP_TIMES(i, arrayLength)
		max.addRect(i, &(faceArray[i].rect));
	return max.getMaxId();
//	return &(faceArray[max.getMaxId()]);
}

bool Face_detect(cv::Mat &_image,
	int resultCapacity,
	NON_NULL FF_FaceInfo* results,
	NON_NULL int* resultCount) {

	FF_FaceInfo* apiResult;
	int apiResultCount = 0;

	auto image = FF_Image_FromCVMat(_image);
	int status = firefly_fd_detect_image(faceDetector, image, &apiResult, &apiResultCount);
	if(status != FF_OK) {
		FACE_FATAL("firefly_fd_detect_image return: %d", status);
		return false;
	}
	if(!apiResult)
		return false;

	*resultCount = GET_MIN(resultCapacity, apiResultCount);
	DUMP_INT(*resultCount, "firefly_fd_detect_image found face count");

	for(int i = 0, j = *resultCount; i < j ; i ++ )
		memcpy(&(results[i]), &(apiResult[i]), sizeof(FF_FaceInfo));

	firefly_fd_free_detect_result(apiResult);
	return true;
}

bool Face_extract(cv::Mat &_image,
	FF_FaceInfo oneFace,
	NON_NULL FF_FaceFeatures* result) {

	if(!result) {
		LOG_FATAL("Face_extract: result pointer is null");
		return false;
	}

	auto image = FF_Image_FromCVMat(_image);
	result->len = 0;
	int status = firefly_fv_extract_feature(faceVerifier,
		image, oneFace, result);
	if(status != FF_OK) {
		LOG_FATAL_F("firefly_fv_extract_feature return: %d", status);
		return false;
	}
	if(result->len <= 0) {
		LOG_WARN_F("firefly_fv_extract_feature (invalid length: %d) <= 0", result->len);
		result->len = 0;
	}
	const int maxLength = (int) FACE_FEATURE_SIZE;
	if(result->len >= maxLength) {
		LOG_WARN_F("firefly_fv_extract_feature (invalid length: %d) >= %d", result->len, maxLength);
		result->len = 0;
	}
	return true;
}

float Face_compare(
	FF_FaceFeatures& f1,
	FF_FaceFeatures& f2) {
	return firefly_fv_compare_feature(f1, f2);
}
