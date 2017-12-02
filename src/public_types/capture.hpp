#include "./firefly_face_sdk.hpp"
#include "./database.hpp"

#ifndef PUBLIC_TYPES_CAPTURE_HPP
#define PUBLIC_TYPES_CAPTURE_HPP

typedef struct CapturePersonInfo {
	FF_FaceInfo info;

	/** (0:not/1:got) is program got this feature info from this face */
	int gotFeatures = 0;
	FF_FaceFeatures features;

	/** (0:not/1:matched) is this person info matched a person in database*/
	int matched = 0;
	float matchedScore = 0.0f;
	char macthedUserId[DB_UserIdLength];

	char addonInfoReserved[64];
} CapturePersonInfo;
typedef CapturePersonInfo* CapturePersonInfoArray;

typedef struct CaptureFrameAndPersonInfo {
	FF_Image image;
	int faceCount = 0;
	/** **Maybe NULL** it is null when (faceCount == 0) is ture */
	CapturePersonInfoArray person;
} CaptureFrameAndFaceInfo;

#endif // CAPTURE_HPP
