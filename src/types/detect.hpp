#include "./base.hpp"

#ifndef PUBLIC_TYPES_DETECT_HPP
#define PUBLIC_TYPES_DETECT_HPP

typedef struct Detect_Pointf {
	float x = 0.0f;
	float y = 0.0f;
} Detect_Pointf;


typedef struct Detect_FaceInfo {

	/// Face rect: (x0, y0)~(x1, y1)
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	float confidence;
	float quality;
	int yaw, pitch, roll;
	Detect_Pointf landmarks[5];

	/** 0: could not get face features */
	int featureLength = 0;
	float features[FACE_FEATURE_LENGTH];

	/** (0:not/1:matched) is this person info matched a person in database*/
	int matched = 0;
	float matchedScore = 0.0f;
	char matchedUserId[USERID_LENGTH];

	char addonInfoReserved[64];

} Detect_FaceInfo;


#endif // PUBLIC_TYPES_DETECT_HPP
