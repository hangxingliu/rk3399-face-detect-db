#include <stdio.h>

#include "./api.hpp"
#include "../face/face.hpp"
#include "../global/global.hpp"
#include "../storage/storage.hpp"

#include "../types/api_error_no.hpp"
#include "../types/firefly_face_sdk.hpp"


static int update(NON_NULL DB_Modification* m) {
	CHECK_NON_NULL_FOR_API(m, "m");

	FF_FaceFeatures features;

	// Check userId is not empty
	if(!m->userId[0]) {
		LOG_FATAL("modification->userId is empty string!");
		return API_EMPTY_POINTER;
	}

	// Check features are valid and copy them into FF_FaceFeatures features
	if(m->type == DB_MODIFICATION_UPDATE) {
		if(m->featureLength <= 0 || ((unsigned int) m->featureLength) >= FACE_FEATURE_SIZE) {
			LOG_FATAL_F("modification->featureLength = %d is invalid!", m->featureLength);
			return API_INVALID_PARAMTER;
		}
		FF_FaceFeatures_create(m->featureLength, m->features, &features);
	}

	switch(m->type) {

	// despacito operation: echo parrameters
	case DB_MODIFICATION_DESPACITO:
		LOG_INFO_F("Despacito echo: \"%s\", feature length: %d", m->userId, m->featureLength);
		return API_OK;

	// update operation: update user features
	case DB_MODIFICATION_UPDATE:
		return DB_updateFeatures(m->userId, &features);

	// delete operation: delete user
	case DB_MODIFICATION_DELETE:
		return DB_deleteUser(m->userId);

	// unknown operation
	default:
		LOG_FATAL_F("Unknown database operation type: %d", m->type);
		return API_INVALID_PARAMTER;
	}
}

// ==========================================
//    E x p o r t   F u n c t i o n s

static const char* API_NAME = "update";
int face_update(DB_Modification* m) {
	LOG_API_INVOKE(API_NAME, "%p", m);
	APITimer timer(API_NAME);

	int status = update(m);

	timer.click();
	return status;
}
