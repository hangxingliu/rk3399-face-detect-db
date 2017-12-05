#include "./firefly_face_sdk.hpp"
#include "./database.hpp"

#ifndef PUBLIC_TYPES_CAPTURE_HPP
#define PUBLIC_TYPES_CAPTURE_HPP

#define MAX_FACE_IN_ONE_CAPTURE_FRAME 5

typedef enum {
	/// Channel order: Red Green Blue
	IMAGE_FORMAT_RGB = 1,
	/// Channel order: Blue Green Red (**OpenCV default**)
	IMAGE_FORMAT_BGR = 2
} CaptureImageFormat;

/**
 * @see getFrameFromCapture()
 */
typedef struct CaptureRequestOptions {

	CaptureImageFormat imageFormat;

	/** face detection will be happen in each request if this value equals **1** */
	int disableMotionDetection = 0;

	/** set this value as 1 if you want **allocate memory of `image.data` by yourself** */
	int disableMallocImageDataArray = 0;

} CaptureRequestOptions;

typedef struct CaptureFrameImage {
	CaptureImageFormat format;
	unsigned char* data;
	int dataLength;
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

/**
 * @see getFrameFromCapture()
 */
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
