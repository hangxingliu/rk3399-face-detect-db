/// Database Structures
/// Each item:
/// { DB_BaseUserItem(Fixed Size) DB_BaseUserItem_LastMatched(Dynamic Size)  ..0.. }
/// Each item max size: 64Kb

#include <time.h>

#include "./base.hpp"
#include "./firefly_face_sdk.hpp"

#ifndef PUBLIC_TYPES_DATABASE_HPP
#define PUBLIC_TYPES_DATABASE_HPP

#define DB_True 1
#define DB_False 0
typedef unsigned char DB_Boolean;

// size: 64 * 1024 = 65536 = 64Kb
#define DB_ITEM_SIZE 65536
#define DB_MAX_ITEM 30000
/// TODO
#define DB_MAX_ITEM_IN_MEMORY 1000
#define DB_MAX_ITEM_SIZE_IN_MEMORY (DB_MAX_ITEM_IN_MEMORY * sizeof(DB_BaseUserItem))


typedef struct DB_BaseUserItem_LastMatched {
	/// 0: disable
	int version = 0;

	long timestamp = 0;

	float compareScore = 0.0f;

	float quality = 0.0f;

	/** chinese: 置信度 */
	float confidence = 0.0f;

	int yaw = 0;
	int pitch = 0;
	int roll = 0;

	int landmarkSize = 0;
	float landmarksX[5];
	float landmarksY[5];

	int faceImageSize = 0;
	unsigned char* faceImage = nullptr;

	static DB_BaseUserItem_LastMatched getVersion1() {
		DB_BaseUserItem_LastMatched result;
		result.version = 1;
		result.timestamp = (long) time(NULL);
		return result;
	}

} DB_BaseUserItem_LastMatched;

// 4 + 1 + 7 + 4 * 4 + 4 + 64 + 24004 + 1024 = 25124
typedef struct DB_BaseUserItem {
	/// itemIndex => fileLocation
	/// 16 + sizeof(DB_BaseUserItem) * (index - 1 )
	/// itemIndex of first item is 1
	unsigned int itemIndex = 0;
	/**
	 * is this item live ?
	 * (live is DB_False means this item has been deleted or non-init)
	 */
	DB_Boolean live = DB_False;

	/** reserved space */
	unsigned char reserved[7];

	/** the hash value calculated from following content to avoiding dirty data */
	unsigned int hash[4];

	int priority = 0;

	char userId[USERID_LENGTH];

	/** length:  6000 * 4 + 4 = 24004 = 24Kb */
	FF_FaceFeatures features;

	unsigned char addonReserved[1024];

	DB_BaseUserItem() {
		memset(this->reserved, 0, 7);
		memset(this->userId, 0, USERID_LENGTH);
		memset(this->addonReserved, 0, 1024);
		memset(this->hash, 0, 4);
	}

} DB_BaseUserItem;

typedef bool(*DB_Iterator)(DB_BaseUserItem* item);

#endif // DATABASE_HPP
