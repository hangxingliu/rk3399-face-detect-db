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

	info.featureLength = ffFaceFeatures->len;
	memcpy(info.features, ffFaceFeatures->data, sizeof(float) * info.featureLength);

	if(matched) {
		info.matched = 1;
		info.matchedScore = matchedScore;
		memcpy(info.matchedUserId, matchedUserId, USERID_LENGTH);
	} else {
		info.matched = 0;
	}

	return info;
}
