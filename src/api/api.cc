#include <stdio.h>

#include "opencv2/opencv.hpp"

#include "./api.hpp"

#include "../types/capture.hpp"
#include "../types/detect.hpp"
#include "../types/database_operation.hpp"
#include "../types/api_error_no.hpp"
#include "../types/config.hpp"

#include "../log/log.hpp"
#include "../utils/utils.hpp"
#include "../utils/timer.hpp"
#include "../global/global.hpp"
#include "../media/media.hpp"
#include "../face/face.hpp"
#include "../storage/storage.hpp"

static bool hideGetFrameLog = false;
static bool hideApiInvoke = false;

#define ANALYZE_API_TIME 0
#if ANALYZE_API_TIME
#define _timer_for_api() _timer_init(t1, t2);
#define _timer_bee_go() _timer_set(t1);
#define _timer_hey_ok(s) {_timer_set(t2);double ms=_timer_calc(t2,t1);_timer_log(ms,s);}
#else
#define _timer_for_api()
#define _timer_bee_go()
#define _timer_hey_ok(s)
#endif

#ifdef NDEBUG
#define LOG_API_INVOKE(name, ps, ...) 0;
#else
#define LOG_API_INVOKE(name, ps, ...)\
	if(!hideApiInvoke) {\
		char __api_log__1[64];char __api_log__2[256];\
		sprintf(__api_log__1, "Invoke API %s(%s)", name, ps);\
		sprintf(__api_log__2, __api_log__1, __VA_ARGS__);\
		LOG_DEBUG(__api_log__2);\
	}
#endif

#define INIT_FLAG_NO_CAPTURE 1
#define INIT_FLAG_NO_DATABASE 2

static int initFlagForTest = 0;
void face_set_init_flag(int flag) {
	LOG_API_INVOKE("set_init_flag", "%d", flag);
	initFlagForTest = flag;
}

int face_set_debug_log(
	int apiGetFrameLog,
	int apiInvokeLog) {

	hideGetFrameLog = !apiGetFrameLog;
	hideApiInvoke = !apiInvokeLog;

//	Log_hideDebugTemporary();
//	Log_cancelHideDebugTemporary();
	return API_OK;
}

int face_init(GlobalConfig* config) {

	LOG_API_INVOKE("init", "%p", config);

	int status = 0;
	// Initialize capture
	if(initFlagForTest & INIT_FLAG_NO_CAPTURE) {
		LOG_INFO("Skip initialization for capture (INIT_FLAG_NO_CAPTURE)");
	} else {
		status = Capture_Init();
		if(status != 0) return API_CANNOT_INIT_CAPTURE;
	}

	status = FrameBuffer_init();
	if(status != 0) return API_CANNOT_INIT_BUFFER;

	status = Config_initGlobalConfig(config);
	if(status != 0) return status;

	if(initFlagForTest & INIT_FLAG_NO_DATABASE) {
		LOG_INFO("Skip initialization for database (INIT_FLAG_NO_DATABASE)");
	} else {
		status = DB_init();
		if(status != 0) return status;
	}

#ifndef FOR_ARM
	initFaceHaarCascade("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt2.xml");
#else
	// For ARM release:
	status = Face_init();
	if(status != 0) return status;
#endif

	return 0;
}

int face_get_frame(
	API_OUT_NON_NULL Capture_FrameImageInfo* result) {

	if(!hideGetFrameLog) { LOG_API_INVOKE("get_frame", "%p", result); }

	if(!result) return API_EMPTY_POINTER;

	int bufferId = -1;
	cv::Mat* buffer = FrameBuffer_giveMeBuffer(&bufferId);
	if(!buffer) return API_ERROR_TERRIBLE;
//	DUMP_INT(bufferId, "got buffer to storaging frame image");

	lockFrameAccess(bufferId);
#define _23_unlock_return(code) unlockFrameAccess(bufferId), (code);

	// ====> get frame from opencv
	FrameBuffer_setInvalid(bufferId);
	if(!hideGetFrameLog) { LOG_DEBUG_F("getting frame image from capture (opencv) to buffer[%d]...", bufferId); }
	if(!Capture_Read(buffer))
		return _23_unlock_return(API_READ_FRAME_FAILED);
	if(!hideGetFrameLog) { LOG_DEBUG("got frame image from opencv"); }
	FrameBuffer_setValid(bufferId);
	// <====

	int status = FrameBuffer_getFrameInfo(false, bufferId, result);
	if(status != 0) return _23_unlock_return(status);

	return _23_unlock_return(API_OK);
}

int face_draw_face_rect(
	int frameId,
	NON_NULL Detect_FaceRectAttr* attr,
	NON_NULL Detect_FaceInfo* faceInfo,
	API_OUT NEED_TO_FREE ucharArray* newImageData,
	API_OUT int* _w, API_OUT int* _h) {

	LOG_API_INVOKE("draw_face_rect", "%d, %p, %p, %p", frameId, attr, faceInfo, newImageData);
	if(!attr) return API_EMPTY_POINTER;
	if(!faceInfo) return API_EMPTY_POINTER;

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

int face_detect(
	int frameId,
	int maxResultCount,
	API_OUT int* _resultCount,
	API_OUT NEED_TO_FREE Detect_FaceInfoArray* _results) {

	LOG_API_INVOKE("detect", "%d, %d, %p, %p", frameId, maxResultCount, _resultCount, _results);

#ifndef FOR_ARM
	LOG_FATAL("Detect API could only use on arm64 device! (Firefly face SDK limited)");
	return API_TODO;
#endif
	_timer_for_api();

	int status = 0;
	cv::Mat image;

	_timer_bee_go();
	status = FrameBuffer_cloneBuffer(frameId, image);
	if(status != 0) return status;
	_timer_hey_ok("clone frame buffer");

	FF_FaceInfo* faces = (FF_FaceInfo*) malloc(maxResultCount * sizeof(FF_FaceInfo));
#define freeFacesAndReturn(code) free(faces), (code);

	_timer_bee_go();
	int count = 0;
	if(!Face_detect(image, maxResultCount, faces, &count))
		return freeFacesAndReturn(API_FACE_DETECT_FAILED);
	LOG_INFO_F("Found %d faces", count);
	_timer_hey_ok("Face_detect");

	if(count == 0) {
		*_resultCount = count;
		return freeFacesAndReturn(API_OK);
	}

	int biggestItem = Face_getBiggestFaceIndex(faces, count);
	if(biggestItem < 0 || biggestItem >= count) {
		LOG_FATAL_F("Face_getBiggestFaceIndex return %d, but not in [0, %d)",
			biggestItem, count);
		return freeFacesAndReturn(API_ERROR_TERRIBLE);
	}

	Detect_FaceInfoArray results = (Detect_FaceInfoArray) malloc(count * sizeof(Detect_FaceInfo));
	LOOP_TIMES(i, count) {
		if(i != biggestItem) {
			// it is not biggest face in frame, so it would not be extracted and compared
			results[i] = Detect_FaceInfo_create(&(faces[i]), nullptr, false, 0.0f, "noname");
			continue;
		}

		_timer_bee_go();
		FF_FaceFeatures features;
		if(!Face_extract(image, faces[i], &features))
			return freeFacesAndReturn(API_FACE_EXTRACT_FAILED);
		_timer_hey_ok("Face_extract");

		bool matched = false; float matchedScore = 0.0f; const char* name = nullptr;
		DB_BaseUserItem matchedUser;
		if(features.len > 0) { //valid features
			LOG_DEBUG_F("Extracted features with length: %d", features.len);

			_timer_bee_go();
			matched = ItemReader_findItemByFeatures(features, Face_compare, &matchedScore, &matchedUser);
			_timer_hey_ok("ItemReader_findItemByFeatures");

			if(matched) {
				name = matchedUser.userId;
				LOG_DEBUG_F("Matched face in db: \"%s\"(score: %.4f)", name, matchedScore);
			}
		}
		_timer_bee_go();
		results[i] = Detect_FaceInfo_create(&(faces[i]), &features, matched, matchedScore, name);
		_timer_hey_ok("Detect_FaceInfo_create");

		auto dp = &(results[i]); //Dump Pointer
		LOG_INFO_F("Biggest face: <%d, %d>~<%d, %d>; feature float count: %d",
			dp->x0, dp->y0, dp->x1, dp->y1, ((dp->featureLength) >> 2) );
	}

	*_results = results;
	*_resultCount = count;
	return freeFacesAndReturn(API_OK);
}

/**
 * @param result a pointer to a Detect_FaceInfo struct to receiving face result
 * @param frameResult
 * @see API_NO_FACE
 * @return 0: detected face. 10001: no face. ...: other error
 */
int face_detect_one_face_in_last_frame(
	API_OUT_NON_NULL Detect_FaceInfo* result,
	API_OUT Capture_FrameImageInfo* frameResult) {

	LOG_API_INVOKE("detect_one_face_in_last_frame", "%p, %p", result, frameResult);
	if(!result) return API_EMPTY_POINTER;

	_timer_for_api();

	int frameId = FrameBuffer_getLastFrameId();
	int count = 0;
	Detect_FaceInfoArray _results = nullptr;

	_timer_bee_go();
	int status = face_detect(frameId, 1, &count, &_results);
	if(status != 0) return status;
	_timer_hey_ok("api face_detect");
	if(count < 1) {
		if(_results) free(_results);
		return API_NO_FACE;
	}

	// copy result
	memcpy(result, &(_results[0]), sizeof(Detect_FaceInfo));
	// copy frame info
	if(frameResult)
		status = FrameBuffer_getFrameInfo(true, frameId, frameResult);

	// free teporary memory
	if(_results)
		free(_results);

	return status;
}

int face_detect1(
	API_OUT_NON_NULL Detect_FaceInfo* result,
	API_OUT Capture_FrameImageInfo* frameResult) {
	cowSayDeprecatedAPI();
	LOG_API_INVOKE("detect1", "%p, %p", result, frameResult);
	return face_detect_one_face_in_last_frame(result, frameResult);
}


int face_update(DB_Modification* m) {
	LOG_API_INVOKE("update", "%p", m);

	char userId[USERID_LENGTH + 1];
	FF_FaceFeatures features;

	if(!m)
		return API_EMPTY_POINTER;

	if(!m->userId || !m->userId[0]) {
		LOG_FATAL("modification->userId is null or empty string!");
		return API_EMPTY_POINTER;
	}
	snprintf(userId, USERID_LENGTH, "%s", m->userId);

	if(m->type == DB_MODIFICATION_UPDATE) {
		if(m->featureLength <= 0) {
			LOG_FATAL_F("modification->featureLength = %d is invalid!", m->featureLength);
			return API_INVALID_PARAMTER;
		}
		FF_FaceFeatures_create(m->featureLength, m->features, &features);
	}

	switch(m->type) {
	case DB_MODIFICATION_DESPACITO:
		LOG_INFO_F("Despacito echo: \"%s\", feature length: %d", userId, m->featureLength);
		return API_OK;
	case DB_MODIFICATION_UPDATE:
		return DB_updateFeatures(userId, &features);
	case DB_MODIFICATION_DELETE:
		return DB_deleteUser(userId);
	default:
		LOG_FATAL_F("Unknown database operation type: %d", m->type);
		return API_INVALID_PARAMTER;
	}
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

