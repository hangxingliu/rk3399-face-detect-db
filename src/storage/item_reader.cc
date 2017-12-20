#include<stdio.h>

#include "./storage.hpp"

#include "../types/base.hpp"
#include "../types/api_error_no.hpp"
#include "../types/database.hpp"

#include "../utils/memory.hpp"
#include "../utils/timer.hpp"

static FILE* fs = nullptr;

/// 0: don't use
static bool IR_living[DB_MAX_ITEM];
static MemoryManager<DB_BaseUserItem> memory(DB_MAX_ITEM, DB_MAX_ITEM_SIZE_IN_MEMORY);

static uint IR_length = 1; //< with one item(index: 0)
static uint IR_livingCount = 0;
uint ItemReader_getLivingCount() { return IR_livingCount; }

void ItemReader_resetAllLivingStatus() {
	for(uint i = 0; i < DB_MAX_ITEM ; i++ )
		IR_living[i] = false;
}

int ItemReader_init(FILE* _fs, uint expectlivingCount) {
	fs = _fs;

	BlankSpaceManager_init();
	ItemReader_resetAllLivingStatus();

	IR_living[0] = false;

	IR_length = 1;
	IR_livingCount = 0;

	/// string buffer to output/log
	char str[256];
	#define INNER_EXCEPTION(...) sprintf(str, __VA_ARGS__ ), LOG_FATAL(str), API_DB_INNER_EXCEPTION

	DB_BaseUserItem bufferItem;
	long location = DB_getHeadSize();

	fseek(fs, location, SEEK_SET);

	const int bufferItemSize = sizeof(DB_BaseUserItem);
	const int jumpToNextSize = DB_ITEM_SIZE - bufferItemSize;
	while(IR_livingCount < expectlivingCount) {

		if(ftell(fs) != location)
			return INNER_EXCEPTION("Could not seek point to %ld for item %u",
				location, IR_length);

		if(fread(&bufferItem, bufferItemSize, 1, fs) != 1)
			return INNER_EXCEPTION("read item %u failed! (expect all living item count: %u)",
				IR_length, IR_livingCount);
		if(bufferItem.itemIndex != IR_length)
			return INNER_EXCEPTION("wrong itemIndex: %u in item. expect itemIndex: %u",
				bufferItem.itemIndex, IR_length);
		if(!DB_validateUserItem(&bufferItem))
			return INNER_EXCEPTION("wrong hash of item");

		if( (IR_living[IR_length] = bufferItem.live) == true ) {
			if(IR_length < DB_MAX_ITEM_IN_MEMORY)
				memcpy(memory.allocate(IR_length), &bufferItem, bufferItemSize);
			IR_livingCount++;
		} else {
			BlankSpaceManager_addFragment(IR_length, true);
		}
		IR_length++;
		location+=DB_ITEM_SIZE;

		fseek(fs, jumpToNextSize, SEEK_CUR);
	}
	BlankSpaceManager_setContinuousSpaceStart(IR_length);

	return 0;
}

bool ItemReader__getItemFromDisk(uint itemIndex, DB_BaseUserItem* result) {
	LOG_DEBUG_F("ItemReader__getItemFromDisk(%u, %p)", itemIndex, result);

	long location = DB_getItemOffset(itemIndex);
	fseek(fs, location, SEEK_SET);
	if(ftell(fs) != location) {
		char str[256];
		sprintf(str, "Could not seek fs pointer to %ld", location);
		LOG_WARN(str);
		return false;
	}
	return fread(result, sizeof(DB_BaseUserItem), 1, fs) == 1;
}

bool ItemReader_getItem(uint itemIndex, DB_BaseUserItem* result) {
	if(itemIndex < 0 || itemIndex >= IR_length)
		return false;
	if(!IR_living[itemIndex])
		return false;

	auto item = memory.at(itemIndex);
	if(!item)
		return ItemReader__getItemFromDisk(itemIndex, result);

	memcpy(result, item, sizeof(DB_BaseUserItem));
	return true;
}

bool ItemReader_findItemByUserId(const char* userId, DB_BaseUserItem* result) {
	DB_BaseUserItem itemBuffer;
	DB_BaseUserItem* item;

	auto list = memory.getItemList();
	for( uint itemIndex = 0 ; itemIndex < IR_length ; itemIndex ++ ) {
		if(!IR_living[itemIndex]) continue;
		item = list[itemIndex];

		if(!item) {
			if(!ItemReader__getItemFromDisk(itemIndex, &itemBuffer))
				return false;
			item = &itemBuffer;
		}

		if(strcmp(item->userId, userId) == 0) {
			memcpy(result, item, sizeof(DB_BaseUserItem));
			return true;
		}
	}
	return false;
}

bool ItemReader_findItemByFeatures(
	FF_FaceFeatures features,
	DB_FeaturesComparer comparer,
	float* score,
	DB_BaseUserItem* result) {

	_timer_init(t1, t2);double ms = 0.0f;

	DB_BaseUserItem itemBuffer;
	DB_BaseUserItem* item;

	float maxScore = 0.0f;
	uint maxItemIndex = 0;

	auto list = memory.getItemList();
	for( uint itemIndex = 0 ; itemIndex < IR_length ; itemIndex ++ ) {
		if(!IR_living[itemIndex]) continue;
		item = list[itemIndex];
		if(!item) {
			if(!ItemReader__getItemFromDisk(itemIndex, &itemBuffer))
				return false;
			item = &itemBuffer;
		}

		_timer_set(t1);
		float s = (*comparer)(features, item->features);
		_timer_set(t2);
#ifndef RELEASE
		ms += _timer_calc(t2, t1);
#endif

		if(s < 0.5f) continue;
		if(s > 0.8f) {
			*score = s;
			memcpy(result, item, sizeof(DB_BaseUserItem));

			_timer_log(ms, "compare features");
			return true;
		} else if(s > maxScore) {
			maxScore = s;
			maxItemIndex = item->itemIndex;
		}
	}

	if(maxItemIndex > 0 && maxScore >= 0.5f) {
		*score = maxScore;

		item = list[maxItemIndex];
		if(!item) {
			if(!ItemReader__getItemFromDisk(maxItemIndex, &itemBuffer))
				return false;
			item = &itemBuffer;
		}
		memcpy(result, item, sizeof(DB_BaseUserItem));

		_timer_log(ms, "compare features");
		return true;
	}
	return false;
}


bool ItemReader_iterateItem(DB_Iterator iterator) {
	if(!iterator)
		return false;

	DB_BaseUserItem itemBuffer;
	DB_BaseUserItem* item;

	auto list = memory.getItemList();
	for( uint itemIndex = 0 ; itemIndex < IR_length ; itemIndex ++ ) {
		if(!IR_living[itemIndex]) continue;
		item = list[itemIndex];

		if(!item) {
			if(!ItemReader__getItemFromDisk(itemIndex, &itemBuffer))
				return false;
			item = &itemBuffer;
		}

		if(!(*iterator)(item))
			break;
	}
	return true;
}


/**
 * Don't invoke this method manual. It will be invoke by method "ItemWrite_xxx"
 * @param itemIndex
 * @param newItem
 * @return bool
 */
bool ItemReader__updateItemCacheInMemory(uint itemIndex, DB_BaseUserItem* newItem) {
	LOG_DEBUG_F("ItemReader__updateItemCacheInMemory(%u, %p)", itemIndex, newItem);

	if(!newItem)
		return false;

	if(itemIndex < 0 || itemIndex >= DB_MAX_ITEM)
		return false;

	if(itemIndex >= IR_length)
		IR_length = itemIndex + 1;

	bool oldLiving = IR_living[itemIndex];
	DB_Boolean newLiving = newItem->live;

	// delete
	if(newLiving == DB_False) {
		LOG_DEBUG_F("ItemReader__updateItemCacheInMemory: DELETE Operation");
		IR_living[itemIndex] = false;
		IR_livingCount -= oldLiving ? 1 : 0;
		return true;
	}
#ifndef RELEASE
	if(!oldLiving) {
		LOG_DEBUG("ItemReader__updateItemCacheInMemory: INSERT Operation");
	} else {
		LOG_DEBUG("ItemReader__updateItemCacheInMemory: MODIFY Operation");
	}
#endif

	IR_livingCount += oldLiving ? 0 : 1;
	// new
	IR_living[itemIndex] = true;

	auto item = memory.at(itemIndex);
	if(!item)
		item = memory.allocate(itemIndex);

	memcpy(item, newItem, sizeof(DB_BaseUserItem));

	LOG_DEBUG("ItemReader__updateItemCacheInMemory: update success!");
	return true;
}
