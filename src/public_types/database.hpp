/// Database Structures
/// Each item:
/// { DB_BaseUserItem(Fixed Size) DB_BaseUserItem_LastMatched(Dynamic Size)  ..0.. }
/// Each item max size: 64Kb

#include "./firefly_face_sdk.hpp"

#ifndef PUBLIC_TYPES_DATABASE_HPP
#define PUBLIC_TYPES_DATABASE_HPP

#define DB_True 1
#define DB_False 0
typedef unsigned char DB_Boolean;

#define DB_UserIdLength 64

// size: 64 * 1024 = 65536 = 64Kb
#define DB_BaseUserItemMaxSize 65536

typedef struct DB_BaseUserItem_LastMatched {
	long timestamp;

	float compareScore;

	float quality;

	/** chinese: 置信度 */
	float confidence;

	int yaw;
	int pitch;
	int roll;

	float landmarksX[5];
	float landmarksY[5];

	int faceImageSize;
	unsigned char* faceImage;

} DB_BaseUserItem_LastMatched;

typedef struct DB_BaseUserItem {
	int itemNumber;
	/**
	 * is this item live ?
	 * (live is DB_False means this item has been deleted or non-init)
	 */
	DB_Boolean live = DB_False;

	/** reserved space */
	char reserved[7];

	/** the hash value calacuted from following content to avoiding dirty data */
	int hash[4];

	int priority;

	char userId[DB_UserIdLength];

	/** length:  6000 * 4 + 4 = 24004 = 24Kb */
	FF_FaceFeatures features;

	char addonReserved[1024];

} DB_BaseUserItem;



#endif // DATABASE_HPP
