#include "../include/firefly_faceverify.hpp"

#include "../types/base.hpp"
#include "../types/firefly_face_sdk.hpp"
#include "../types/detect.hpp"

Detect_FaceInfo Detect_FaceInfo_create(
	FF_FaceInfo* ffFaceInfo,
	FF_FaceFeatures* ffFaceFeatures,
	bool matched, float matchedScore,
	const char* matchedUserId) {

	Detect_FaceInfo info;

	auto rect = ffFaceInfo->rect;
	info.x0 = rect.left;
	info.x1 = rect.right;
	info.y0 = rect.top;
	info.y1 = rect.bottom;

	info.quality = ffFaceInfo->quality;
	info.confidence = ffFaceInfo->confidence;

	if(ffFaceFeatures) {
		info.featureLength = ffFaceFeatures->len;
		memcpy(info.features, ffFaceFeatures->data, FACE_FEATURE_SIZE);
	} else {
		info.featureLength = 0;
	}

	if(matched) {
		info.matched = 1;
		info.matchedScore = matchedScore;
		memcpy(info.matchedUserId, matchedUserId, USERID_LENGTH);
	} else {
		info.matched = 0;
	}

	return info;
}

/// @todo move this function to correct location/file
void FF_FaceFeatures_create(
	int len, const float* data,
	FF_FaceFeatures* instance) {

	instance->len = len;
	memcpy(instance->data, data, FACE_FEATURE_SIZE);
}
