#include "./firefly_face_sdk.hpp"
#include "./database.hpp"
#include "../public_types/base.hpp"

#ifndef PUBLIC_TYPES_CAPTURE_HPP
#define PUBLIC_TYPES_CAPTURE_HPP

#define MAX_FACE_IN_ONE_CAPTURE_FRAME 5

typedef enum {
	/// Channel order: Red Green Blue
	RGB = 1,
	/// Channel order: Blue Green Red (**OpenCV default**)
	BGR = 2
} CaptureImageFormat;

typedef struct CaptureRequestOptions {

	CaptureImageFormat imageFormat;

	/** face detection will be happen in each request if this value equals **1** */
	int disableMotionDetection = 0;

} CaptureRequestOptions;

typedef struct CaptureFrameImage {
	CaptureImageFormat format;
	ubyteArray data;
	int w;
	int h;
} CaptureFrameImage;

typedef struct CapturePersonInfo {
	FF_FaceInfo info;

	/** (0:not/1:got) is program got this feature info from this face */
	int gotFeatures = 0;
	FF_FaceFeatures features;

	/** (0:not/1:matched) is this person info matched a person in database*/
	int matched = 0;
	float matchedScore = 0.0f;
	char matchedUserId[DB_UserIdLength];

	char addonInfoReserved[64];
} CapturePersonInfo;
typedef CapturePersonInfo* CapturePersonInfoArray;

typedef struct CaptureFrameAndPersonInfo {
	CaptureFrameImage image;

	int faceCount = 0;

	/**
	 * **Maybe NULL** it is null when (faceCount == 0) is true
	 * Max length: 5 MAX_FACE_IN_ONE_CAPTURE_FRAME
	 * @see MAX_FACE_IN_ONE_CAPTURE_FRAME
	 */
	CapturePersonInfoArray person;
} CaptureFrameAndFaceInfo;


typedef struct PhysicalCaptureDevice {
	bool available;
	int id;
	char devicePath[32];
	char cardName[32];
	char driver[16];
} PhysicalCaptureDevice;

#endif // CAPTURE_HPP
