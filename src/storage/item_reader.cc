#include<stdio.h>

#include "./storage.hpp"

#include "../types/base.hpp"
#include "../types/api_error_no.hpp"
#include "../types/database.hpp"

static FILE* fs = nullptr;
static int startPoint = 0;

static long IR_itemSizeL = DB_ITEM_SIZE;

static uint IR_indexes[DB_MAX_ITEM];
static bool IR_living[DB_MAX_ITEM];
static DB_BaseUserItem* IR_items[DB_MAX_ITEM];
static int IR_length = 0;

void ItemReader_disposeAllItemInMemory() {
	LOOP_TIMES(i, IR_length) { if(IR_items[i]) { free(IR_items[i]); } }
}

int ItemReader_init(FILE* _fs, int livingCount) {
	fs = _fs;
	startPoint = DB_getHeadSize();

	BlankSpaceManager_init();
	ItemReader_disposeAllItemInMemory();

	IR_length = 0;

	/// string buffer to output/log
	char str[256]; str;
	#define INNER_EXCEPTION(...) sprintf(str, __VA_ARGS__ ), LOG_FATAL(str), API_DB_INNER_EXCEPTION

	unsigned int itemIndex = 1;
	DB_BaseUserItem bufferItem;
	long location = startPoint;

	fseek(fs, startPoint, SEEK_SET);

	const int bufferItemSize = sizeof(DB_BaseUserItem);
	const int jumpToNextSize = DB_ITEM_SIZE - bufferItemSize;
	while(IR_length < livingCount) {

		if(ftell(fs) != location)
			return INNER_EXCEPTION("Could not seek point to %ld", location);

		if(fread(&bufferItem, bufferItemSize, 1, fs) != 1)
			return INNER_EXCEPTION("read living item %d (itemIndex: %u) failed! (expect all living item count: %d)",
				IR_length, itemIndex, livingCount);
		if(bufferItem.itemIndex != itemIndex)
			return INNER_EXCEPTION("wrong itemIndex: %u in item. expect itemIndex: %u",
				bufferItem.itemIndex, itemIndex);
		if(!DB_validateUserItem(&bufferItem))
			return INNER_EXCEPTION("wrong hash of item");

		if(bufferItem.live) {
			IR_indexes[IR_length] = bufferItem.itemIndex;
			IR_living[IR_length] = true;
			if(IR_length < DB_MAX_ITEM_IN_MEMORY) {
				IR_items[IR_length] = (DB_BaseUserItem*) malloc(bufferItemSize);
				memcpy(IR_items[IR_length], &bufferItem, bufferItemSize);
			} else {
				IR_items[IR_length] = nullptr;
			}
			IR_length++;
		} else {
			BlankSpaceManager_addFragment(itemIndex);
		}
		itemIndex++;
		location+=DB_ITEM_SIZE;

		fseek(fs, jumpToNextSize, SEEK_CUR);
	}
	BlankSpaceManager_setContinuousSpaceStart(itemIndex);

	return 0;
}

bool ItemReader__getItemFromDisk(uint itemIndex, DB_BaseUserItem* result) {
	long location = startPoint + (itemIndex - 1) * IR_itemSizeL;
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
	int pos = -1;
	LOOP_TIMES(i, IR_length) {
		if(IR_indexes[i] == itemIndex) {
			pos = i;
			break;
		}
	}
	if(pos < 0)
		return false;
	if(!IR_living[pos]) //dead
		return false;
	if(!IR_items[pos])
		return ItemReader__getItemFromDisk(itemIndex, result);

	memcpy(result, &(IR_items[pos]), sizeof(DB_BaseUserItem));
	return true;
}

bool ItemReader_findItemByUserId(char* userId, DB_BaseUserItem* result) {
	int pos = -1;
	DB_BaseUserItem itemBuffer;
	DB_BaseUserItem* item;
	LOOP_TIMES(i, IR_length) {
		if(!IR_living[i]) continue;
		item = IR_items[i];
		if(!item) {
			if(!ItemReader__getItemFromDisk(IR_indexes[i], &itemBuffer))
				return false;
			item = &itemBuffer;
		}
		if(strcmp(item->userId, userId) == 0) {
			pos = i;
			memcpy(result, item, sizeof(DB_BaseUserItem));
			return true;
		}
	}
	return false;
}

