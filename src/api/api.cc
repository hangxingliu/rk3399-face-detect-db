#include "opencv2/opencv.hpp"

#include "./api.hpp"

#include "../types/capture.hpp"
#include "../types/detect.hpp"
#include "../types/database_operation.hpp"
#include "../types/api_error_no.hpp"
#include "../types/config.hpp"

#include "../log/log.hpp"
#include "../utils/utils.hpp"
#include "../global/global.hpp"
#include "../media/media.hpp"

#ifdef NDEBUG
#define LOG_API_INVOKE(name, ps, ...) 0;
#else
#define LOG_API_INVOKE(name, ps, ...)\
	char __api_log__1[64];char __api_log__2[256];\
	sprintf(__api_log__1, "Invoke API %s(%s)", name, ps);\
	sprintf(__api_log__2, __api_log__1, __VA_ARGS__);\
	LOG_DEBUG(__api_log__2);
#endif

/*
@param opts **allow NULL**
@param returnFrameImgAndFaceInfo
@return

int getFrameFromCapture(CaptureRequestOptions* opts, CaptureFrameAndPersonInfo* returnFrameImgAndFaceInfo) {
	LOG_API_INVOKE("getFrameFromCapture", "%p, %p", opts, returnFrameImgAndFaceInfo);

	if(!returnFrameImgAndFaceInfo)
		return 0;

	if(!opts) {
		CaptureRequestOptions _opts;
		opts = &_opts;
	}

	auto format = opts->imageFormat;
	auto ret = returnFrameImgAndFaceInfo;
	auto retImage = &(ret->image);

	cv::Mat rawFrame;
	if(!Capture_Read(&rawFrame))
		return -1;

	retImage->w = rawFrame.cols;
	retImage->h = rawFrame.rows;
	retImage->format = format;

	// Copy Image
	retImage->data = copyBGRMat2DataArray(rawFrame,
		opts->disableMallocImageDataArray ? nullptr : retImage->data,
		&(retImage->dataLength),
		format == CAPTURE_IMAGE_FORMAT_BGR ? COPY_RULE_KEEP_BGR : COPY_RULE_BGR_TO_RGB);

	// DUMP_INT(retImage->w, "frame.width");
	// DUMP_INT(retImage->h, "frame.height");
	// DUMP_INT(rawFrame.channels(), "frame.channels()");
	// DUMP_INT(retImage->dataLength, "copied frame size");

	if(opts->disableFaceDetection) {
		ret->personCount = 0;
		return 0;
	}

#ifdef FOR_ARM
#else
	std::vector<cv::Rect> results;
	if(!detectFace(rawFrame, results, false)) {
		LOG_FATAL("detectFace return false!");
		return -1;
	}
	int personCount = results.size();
	ret->personCount = personCount;
	//DUMP_INT(personCount, "ret->personCount");
	if(personCount) {
		if(!(ret->persons))
			ret->persons = (CapturePersonInfo*) malloc(sizeof(CapturePersonInfo) * personCount);
		int i = 0;
		for(const cv::Rect rect : results) {
			auto person = &(ret->persons[i]);

			person->x0 = rect.x;
			person->y0 = rect.y;
			person->x1 = rect.x + rect.width;
			person->y1 = rect.y + rect.height;
			i++; //person += sizeof(CapturePersonInfo);
		}
	}

#endif

	return 0;
}
 */


static const Capture_FrameRequestOpts face_get_frame_DefaultRequestOpts;

int face_init(GlobalConfig* config) {

	LOG_API_INVOKE("init", "%p", config);

	// Initialize capture
	if(Capture_Init() < 0)
		return API_CANNOT_INIT_CAPTURE;

	if(FrameBuffer_init() < 0)
		return API_CANNOT_INIT_BUFFER;

#ifndef FOR_ARM
	// TODO to config
	initFaceHaarCascade("./resources/haarcascade_frontalface_alt2.xml");
#endif

	return 0;
}

/**
 * @param opts
 * @param resultInfo
 * @param resultData this is a pointer pointed to a uchar(unsigned char) array
 * @return int
 */
int face_get_frame(
	const Capture_FrameRequestOpts* opts,
	API_OUT Capture_FrameImageInfo* resultInfo,
	API_OUT_NON_NULL ucharArray* resultData) {

	LOG_API_INVOKE("get_frame", "%p, %p, %p", opts, resultInfo, resultData);

	if(!resultData) return API_EMPTY_POINTER;
	if(!opts) opts = &face_get_frame_DefaultRequestOpts;

	int lockId = -1;
	cv::Mat* buffer = FrameBuffer_giveMeBuffer(&lockId);
	if(!buffer) return API_ERROR_TERRIBLE;

	lockFrameAccess(lockId);
	#define _23_unlock_return(code) unlockFrameAccess(lockId), (code);

	if(!Capture_Read(buffer))
		return _23_unlock_return(API_READ_FRAME_FAILED);

	int width = buffer->cols;
	int height = buffer->rows;

	if(!buffer->isContinuous())
		return _23_unlock_return(API_FRAME_IS_NOT_CONTINUOUS);
	if(width <= 0 || height <= 0 || !buffer->data)
		return _23_unlock_return(API_FRAME_IS_EMPTY);
#ifndef NDEBUG
	if(buffer->channels() != 3 || buffer->elemSize() != 3)
		return _23_unlock_return(API_FRAME_IS_NOT_U8C3);
#endif

	if(resultInfo) {
		/// TODO: If convert to RGB
		resultInfo->isRGB = 0;
		resultInfo->w = width;
		resultInfo->h = height;
		resultInfo->frameId = lockId;
		resultInfo->size = width * height * 3;
	}
	*resultData = buffer->data;

	return _23_unlock_return(API_OK);
}

int face_detect(
	int frameId,
	int maxResultCapacity,
	API_OUT int* resultCount,
	API_OUT Detect_FaceInfo** results) {

	LOG_API_INVOKE("detect", "%d, %d, %p, %p", frameId, maxResultCapacity, resultCount, results);

	return 0;
}

int face_update(DB_Modification* modification) {

	LOG_API_INVOKE("update", "%p", modification);
	if(!modification) return API_EMPTY_POINTER;

	return 0;
}

/**
 * Debug API to
 * @param ping
 * @return ping+1
 */
int face_ping_pong(int ping){
	LOG_API_INVOKE("ping_pong", "%d", ping);
	return ping + 1;
}
