#include "./storage.hpp"

#include "../log/log.hpp"
#include "../types/base.hpp"
#include "../types/database.hpp"

DB_BaseUserItem_LastMatched __emptyUserItem_LastMatched;

char* DB_encodeUserInfo(
	DB_BaseUserItem* userItem,
	DB_BaseUserItem_LastMatched* lastMatchedInfo,
	char* buffer) {

	if(!buffer)
		return LOG_FATAL("Could not encode DB_BaseUserItem to NULL buffer"), nullptr;

	char* p = buffer;
	if(!userItem)
		return LOG_FATAL("Could not encode empty DB_BaseUserItem"), nullptr;

	DB_calcUserItemHash(userItem, userItem->hash);

	memcpy(p, userItem, sizeof(DB_BaseUserItem));
	p += sizeof(DB_BaseUserItem);

	if(!lastMatchedInfo)
		lastMatchedInfo = &__emptyUserItem_LastMatched;

	memcpy(p, lastMatchedInfo, sizeof(DB_BaseUserItem_LastMatched));
	p += sizeof(DB_BaseUserItem_LastMatched);

	// remove image pointer and storage image.
	p -= sizeof(unsigned char*);
	int imageSize = lastMatchedInfo->faceImageSize;
	if(imageSize) {
		uchar* image = lastMatchedInfo->faceImage;
		memcpy(p, image, imageSize);
		p += imageSize;
	}
	// End set zero
	*p = 0;

	return buffer;
}


bool DB_decodeBaseUserInfo(
	char* bytes,
	DB_BaseUserItem* result) {

	if(!bytes)
		return LOG_FATAL("Could not decode BaseUserItem from NULL bytes"), false;
	if(!result)
		return LOG_FATAL("Could not decode BaseUserItem to NULL"), false;

	memcpy(result, bytes, sizeof(DB_BaseUserItem));
	return DB_validateUserItem(result);
}

void DB_decodeLastMatchedInfo() {
	/// TODO
}
