#include <stdio.h>

#include "./api.hpp"

#include "../face/face.hpp"
#include "../global/global.hpp"
#include "../storage/storage.hpp"

#define MAX_VALUE_OF_MAX_RESULT_COUNT 5

static int detect(
	int frameId,
	int maxResultCount,
	API_OUT int* _resultCount,
	API_OUT NEED_TO_FREE Detect_FaceInfoArray* _results) {

	// check is current platform aarch64
	THIS_IS_AN_AARCH64_ONLY_API;

	// check is maxResultCount less than 5
	if(maxResultCount > MAX_VALUE_OF_MAX_RESULT_COUNT)
		return API_INVALID_MAX_RESULT_COUNT;

	int status = 0;
	cv::Mat image;
	FF_FaceFeatures features;
	FF_FaceInfo faces[MAX_VALUE_OF_MAX_RESULT_COUNT];

	status = FrameBuffer_cloneBuffer(frameId, image);
	if(status != 0) return status;


	APITimer timer4detect("Face_detect");
	int count = 0;
	if(!Face_detect(image, maxResultCount, faces, &count))
		return API_FACE_DETECT_FAILED;
	LOG_DEBUG_F("Face_detect: found %d faces", count);
	timer4detect.click();

	if(count == 0) {
		*_resultCount = count;
		return API_OK;
	}

	int biggestItem = Face_getBiggestFaceIndex(faces, count);
	Detect_FaceInfoArray results = (Detect_FaceInfoArray) malloc(count * sizeof(Detect_FaceInfo));
	LOOP_TIMES(i, count) {
		if(i != biggestItem) {
			// it is not biggest face in frame, so it would not be extracted and compared
			results[i] = Detect_FaceInfo_create(&(faces[i]), nullptr, false, 0.0f, "noname");
			continue;
		}

		// biggest face, so extract features and compare

		APITimer timer4extract("Face_extract");
		if(!Face_extract(image, faces[i], &features))
			return (API_FACE_EXTRACT_FAILED);
		timer4extract.click();

		bool matched = false; float matchedScore = 0.0f; const char* name = nullptr;
		DB_BaseUserItem matchedUser;
		if(features.len > 0) { //valid features
			LOG_DEBUG_F("Extracted features with length: %d", features.len);

			APITimer timer4findInDB("ItemReader_findItemByFeatures");
			matched = ItemReader_findItemByFeatures(features, Face_compare, &matchedScore, &matchedUser);
			timer4findInDB.click();

			if(matched) {
				name = matchedUser.userId;
				LOG_DEBUG_F("Matched face in db: \"%s\"(score: %.4f)", name, matchedScore);
			}
		}
		results[i] = Detect_FaceInfo_create(&(faces[i]), &features, matched, matchedScore, name);

		auto dp = &(results[i]); //Dump Pointer
		LOG_INFO_F("Biggest face: <%d, %d>~<%d, %d>; feature float count: %d",
			dp->x0, dp->y0, dp->x1, dp->y1, ((dp->featureLength) >> 2) );
	}

	*_results = results;
	*_resultCount = count;
	return (API_OK);
}

/**
 * @param result a pointer to a Detect_FaceInfo struct to receiving face result
 * @param frameResult
 * @see API_NO_FACE
 * @return 0: detected face. 10001: no face. ...: other error
 */
static int detect_one_face_in_last_frame(
	API_OUT_NON_NULL Detect_FaceInfo* result,
	API_OUT Capture_FrameImageInfo* frameResult) {

	if(!result) return API_EMPTY_POINTER;

	int frameId = FrameBuffer_getLastFrameId();
	int count = 0;
	Detect_FaceInfoArray _results = nullptr;

	int status = face_detect(frameId, 1, &count, &_results);
	if(status != 0) return status;
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


// ==========================================
//    E x p o r t   F u n c t i o n s

static const char* API_NAME_DETECT = "detect";
static const char* API_NAME_DETECT_ONE = "face_detect_one_face_in_last_frame";

int face_detect(
	int frameId,
	int maxResultCount,
	API_OUT int* _resultCount,
	API_OUT NEED_TO_FREE Detect_FaceInfoArray* _results) {

	LOG_API_INVOKE(API_NAME_DETECT, "%d, %d, %p, %p", frameId, maxResultCount, _resultCount, _results);
	APITimer timer(API_NAME_DETECT);

	int status = detect(frameId, maxResultCount, _resultCount, _results);

	timer.click();
	return status;
}

int face_detect_one_face_in_last_frame(
	API_OUT_NON_NULL Detect_FaceInfo* result,
	API_OUT Capture_FrameImageInfo* frameResult) {

	LOG_API_INVOKE(API_NAME_DETECT_ONE, "%p, %p", result, frameResult);
	APITimer timer(API_NAME_DETECT_ONE);

	int status = detect_one_face_in_last_frame(result, frameResult);

	timer.click();
	return status;
}
int face_detect1(
	API_OUT_NON_NULL Detect_FaceInfo* result,
	API_OUT Capture_FrameImageInfo* frameResult) {
	return face_detect_one_face_in_last_frame(result, frameResult);
}
