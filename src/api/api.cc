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
#include "../log/log.hpp"


/**
 * Init database program (**It may take a long time**)
 * @param overwriteConfig configurations based on return object of method `getDefaultGlobalConfig`
 * @return status code 0: success
 */
int initDatabase(GlobalConfig* overwriteConfig) {
	LOG_DEBUG("Invoke API init()");

	// Initialize capture
	if(Capture_Init() < 0)
		return -1;

	return 0;
}

/**
 * Get default value of GlobalConfig.
 * You can modify config base on it and init database
 * @param returnConfig the location default config return to
 * @return status code 0: success
 * @see init(GlobalConfig* overwriteConfig)
 */
int getDefaultGlobalConfig(GlobalConfig* returnConfig) {
	printf("CALL: getDefaultGlobalConfig()\n");
	return Config_getDefaultGlobalConfig(returnConfig);
}

int getFrameFromCapture(CaptureRequestOptions* opts, CaptureFrameAndPersonInfo* returnFrameImgAndFaceInfo) {
	LOG_DEBUG("Invoke API getFrameFromCapture()");

	auto ret = returnFrameImgAndFaceInfo;
	auto retImage = ret->image;
	cv::Mat rawFrame;

	if(!Capture_Read(&rawFrame))
		return -1;

	auto frameSize = rawFrame.size();
	retImage.w = frameSize.width;
	retImage.h = frameSize.height;

	DUMP_INT(retImage.w, "frame.width");
	DUMP_INT(retImage.h, "frame.height");
	DUMP_INT(rawFrame.channels(), "frame.channels()");
	DUMP_INT((rawFrame.total() * rawFrame.elemSize()), "frame.data.size");

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
	printf("CALL: debugPingPong()\n");
	return ping + 1;
}
