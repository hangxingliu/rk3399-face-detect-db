#include "stdio.h"
#include "string.h"

#include "opencv2/opencv.hpp"

#include "./api.hpp"
#include "../public_types/base.hpp"
#include "../public_types/capture.hpp"
#include "../public_types/database_operation.hpp"
#include "../public_types/config.hpp"
#include "../config/config.hpp"
#include "../media_handlers/media_handlers.hpp"
#include "../utils/utils.hpp"
#include "../log/log.hpp"

#ifdef NDEBUG
#define LOG_API_INVOKE(name, ps, ...) 0;
#else
#define LOG_API_INVOKE(name, ps, ...)\
	char __api_log__1[64];char __api_log__2[256];\
	sprintf(__api_log__1, "Invoke API %s(%s)", name, ps);\
	sprintf(__api_log__2, __api_log__1, __VA_ARGS__);\
	LOG_DEBUG(__api_log__2);
#endif

/**
 * Init database program (**It may take a long time**)
 * @param overwriteConfig configurations based on return object of method `getDefaultGlobalConfig`
 * @return status code 0: success
 */
int initDatabase(GlobalConfig* overwriteConfig) {
	LOG_API_INVOKE("initDatabase", "%p", overwriteConfig);

	// Initialize capture
	if(Capture_Init() < 0)
		return API_CANNOT_INIT_CAPTURE;

#ifndef FOR_ARM
	// TODO to config
	if(!initFaceHaarCascade("../resources/haarcascade_frontalface_alt2.xml"))
		return -1;
#endif

	return 0;
}

/**
 * @param opts **allow NULL**
 * @param returnFrameImgAndFaceInfo
 * @return
 */
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

/**
 * Alias of updateUserFeaturesInDatabase()
 * @see updateUserFeaturesInDatabase()
 */
int updateUserInDatabase(DB_UpdateUserFeatures* updateDescription) {
	LOOP_TIMES(i, 5) printf("Tip: Hey, you can call method `updateUserFeaturesInDatabase` but not this!");
	return updateUserFeaturesInDatabase(updateDescription);
}
int updateUserFeaturesInDatabase(DB_UpdateUserFeatures* updateDescription) {
	printf("CALL: updateUserFeaturesInDatabase()\n");
	return 0;
}

int removeUserFromDatabase(DB_RemoveUser* removeDescription) {
	printf("CALL: removeUserFromDatabase()\n");
	return 0;
}

/**
 * Debug API to
 * @param ping
 * @return ping+1
 */
int debugPingPong(int ping){
	LOG_API_INVOKE("debugPingPong", "%d", ping);
	return ping + 1;
}
