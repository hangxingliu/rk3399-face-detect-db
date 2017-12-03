#include "stdio.h"
#include "string.h"

#include "api.hpp"
#include "../public_types/base.hpp"
#include "../public_types/capture.hpp"
#include "../public_types/database_operation.hpp"
#include "../config/types.hpp"
#include "../config/methods.hpp"

/**
 * Init database program (**It may take a long time**)
 * @param overwriteConfig configurations based on return object of method `getDefaultGlobalConfig`
 * @return status code 0: success
 */
int init(GlobalConfig* overwriteConfig) {
	printf("CALL: init()\n");
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
	printf("CALL: getFrameFromCapture()\n");
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
