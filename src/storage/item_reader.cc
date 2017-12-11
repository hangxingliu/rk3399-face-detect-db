#include<stdio.h>

#include "./storage.hpp"

#include "../types/base.hpp"
#include "../types/api_error_no.hpp"
#include "../types/database.hpp"

static FILE* fs = nullptr;

static long IR_itemSizeL = DB_ITEM_SIZE;

/// 0: don't use
static bool IR_living[DB_MAX_ITEM];
static DB_BaseUserItem* IR_items[DB_MAX_ITEM];
static uint IR_length = 1; //< with one item(index: 0)
static uint IR_livingCount = 0;
uint ItemReader_getLivingCount() { return IR_livingCount; }

void ItemReader_disposeAllItemInMemory() {
	LOOP_TIMES(i, IR_length) { if(IR_items[i]) { free(IR_items[i]); } }
}

int ItemReader_init(FILE* _fs, int expectlivingCount) {
	fs = _fs;

	BlankSpaceManager_init();
	ItemReader_disposeAllItemInMemory();

	IR_living[0] = false;
	IR_items[0] = nullptr;

	IR_length = 1;
	IR_livingCount = 0;

	/// string buffer to output/log
	char str[256]; str;
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

		if(bufferItem.live) {
			IR_living[IR_length] = true;
			if(IR_length < DB_MAX_ITEM_IN_MEMORY) {
				IR_items[IR_length] = (DB_BaseUserItem*) malloc(bufferItemSize);
				memcpy(IR_items[IR_length], &bufferItem, bufferItemSize);
			} else {
				IR_items[IR_length] = nullptr;
			}
			IR_livingCount++;
		} else {
			IR_living[IR_length] = false;
			IR_items[IR_length] = nullptr;
			BlankSpaceManager_addFragment(IR_length);
		}
		IR_length++;
		location+=DB_ITEM_SIZE;

		fseek(fs, jumpToNextSize, SEEK_CUR);
	}
	BlankSpaceManager_setContinuousSpaceStart(IR_length);

	return 0;
}

bool ItemReader__getItemFromDisk(uint itemIndex, DB_BaseUserItem* result) {
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

	if(!IR_items[itemIndex])
		return ItemReader__getItemFromDisk(itemIndex, result);

	memcpy(result, &(IR_items[itemIndex]), sizeof(DB_BaseUserItem));
	return true;
}

bool ItemReader_findItemByUserId(char* userId, DB_BaseUserItem* result) {
	DB_BaseUserItem itemBuffer;
	DB_BaseUserItem* item;
	for( uint itemIndex = 0 ; itemIndex < IR_length ; itemIndex ++ ) {
		if(!IR_living[itemIndex]) continue;
		item = IR_items[itemIndex];

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


/**
 * Don't invoke this method manual. It will be invoke by method "ItemWrite_xxx"
 * @param itemIndex
 * @param newItem
 * @return bool
 */
bool ItemReader__updateItemCacheInMemory(uint itemIndex, DB_BaseUserItem* newItem) {
	if(itemIndex < 0 || itemIndex >= IR_length)
		return false;

	bool oldLiving = IR_living[itemIndex];
	bool newLiving = newItem->live;

	// delete
	if(newLiving == false) {
		IR_living[itemIndex] = false;
		IR_livingCount -= oldLiving ? 1 : 0;
		return true;
	}
	if(!newItem)
		return false;

	IR_livingCount += oldLiving ? 0 : 1;
	// new
	IR_living[itemIndex] = true;
	memcpy(IR_items[itemIndex], newItem, sizeof(DB_BaseUserItem));
	return true;
}
