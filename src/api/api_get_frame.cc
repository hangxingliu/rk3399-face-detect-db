#include <stdio.h>

#include "./api.hpp"
#include "../global/global.hpp"
#include "../media/media.hpp"

#include "../types/capture.hpp"

#define LOG_FOR_GET_FRAME(...) if(!APITracker_hideGetFrameLog){LOG_DEBUG_F(__VA_ARGS__);}


static int get_frame(
	API_OUT_NON_NULL Capture_FrameImageInfo* result) {

	CHECK_NON_NULL_FOR_API(result, "result");

	int status = 0;
	int bufferId = -1;
	cv::Mat* buffer = FrameBuffer_giveMeBuffer(&bufferId);

	for( lockFrameAccess(bufferId) ;; ) {

		// ====> get frame from opencv
		FrameBuffer_setInvalid(bufferId);

		LOG_FOR_GET_FRAME("getting frame image from capture (opencv) to buffer[%d]...", bufferId);
		status = Capture_Read(buffer) ? 0 : (API_READ_FRAME_FAILED);
		LOG_FOR_GET_FRAME("got frame image from opencv");

		if(status != 0) break;

		FrameBuffer_setValid(bufferId);
		// <====

		status = FrameBuffer_getFrameInfo(false, bufferId, result);
		if(status != 0) break;

		break;
	}

	unlockFrameAccess(bufferId);
	return status;
}


// ==========================================
//    E x p o r t   F u n c t i o n s

static const char* API_NAME = "get_frame";
int face_get_frame(
	API_OUT_NON_NULL Capture_FrameImageInfo* result) {

	// hide log
	if(APITracker_hideGetFrameLog)
		return get_frame(result);

	// display log
	LOG_API_INVOKE(API_NAME, "%p", result);
	APITimer timer(API_NAME);
	int status = get_frame(result);
	timer.click();
	return status;
}
