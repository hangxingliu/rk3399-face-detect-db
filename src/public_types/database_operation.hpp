#include "./database.hpp"
#include "./firefly_face_sdk.hpp"

#ifndef PUBLIC_TYPES_DATABASE_OPERATION_HPP
#define PUBLIC_TYPES_DATABASE_OPERATION_HPP

typedef struct DB_UpdateUserFeatures {
	char UserId[DB_UserIdLength];
	int newFeatureLength;
	float newFeatures[FF_FACE_FEATURE_LEN];
} DB_UpdateUserFeatures;

typedef struct DB_RemoveUser {
	char UserId[DB_UserIdLength];
	/** fill 0 into database */
	DB_Boolean thoroughly;
} DB_RemoveUser;

#endif // PUBLIC_TYPES_DATABASE_OPERATION_HPP
