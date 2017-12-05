#include "./database.hpp"
#include "./firefly_face_sdk.hpp"

#ifndef PUBLIC_TYPES_CAPTURE_HPP
#define PUBLIC_TYPES_CAPTURE_HPP

#define MAX_PERSON_IN_ONE_CAPTURE_FRAME 5

typedef enum {
	/// Channel order: Red Green Blue
	CAPTURE_IMAGE_FORMAT_RGB = 1,
	/// Channel order: Blue Green Red (**OpenCV default**)
	CAPTURE_IMAGE_FORMAT_BGR = 2
} CaptureImageFormat;


/**
 * @see getFrameFromCapture()
 */
typedef struct CaptureRequestOptions {

	CaptureImageFormat imageFormat = CAPTURE_IMAGE_FORMAT_RGB;

	/** face detection will be happen in each request if this value equals **1** */
	int disableMotionDetection = 0;

	/** set this value to 1 if you want **allocate memory of `image.data` by yourself** */
	int disableMallocImageDataArray = 0;

	int disableFaceDetection = 0;

} CaptureRequestOptions;


typedef struct CaptureFrameImage {
	CaptureImageFormat format;
	unsigned char* data = nullptr;
	int dataLength = 0;
	int w = 0;
	int h = 0;
} CaptureFrameImage;


typedef struct CapturePersonInfo_Pointf {
	float x = 0.0f, y = 0.0f;
} CapturePersonInfo_Pointf;


typedef struct CapturePersonInfo {

	/// Face rect: (x0, y0)~(x1, y1)
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
	float confidence;
	float quality;
	int yaw, pitch, roll;
	CapturePersonInfo_Pointf landmarks[5];

	/** 0: could not get face features */
	int featureLength = 0;
	float features[FF_FACE_FEATURE_LEN];

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

	/**
	 * Max: 5 MAX_PERSON_IN_ONE_CAPTURE_FRAME
	 * @see MAX_PERSON_IN_ONE_CAPTURE_FRAME
	 */
	int personCount = 0;
	CapturePersonInfoArray persons = nullptr;

} CaptureFrameAndFaceInfo;


typedef struct PhysicalCaptureDevice {
	bool available;
	int id;
	char devicePath[32];
	char cardName[32];
	char driver[16];
} PhysicalCaptureDevice;

#endif // CAPTURE_HPP
