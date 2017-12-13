#include<stdio.h>

#include "./storage.hpp"

#include "../types/base.hpp"
#include "../types/api_error_no.hpp"
#include "../types/database.hpp"

#define IR_SEEK(fs, num) fseek(fs, num, SEEK_SET)
#define IR_WRITE_INT(fs, ptr) (fwrite(ptr, sizeof(int), 1, fs) == 1)
#define IR_WRITE_ITEM(fs, ptr) (fwrite(ptr, sizeof(DB_BaseUserItem), 1, fs) == 1)

#define IR_COPY_ITEM(to, from) memcpy(to, from ,sizeof(DB_BaseUserItem))

#define IR_LOG_FATAL(max, ...) char _buff[max];snprintf(_buff, max, __VA_ARGS__);LOG_FATAL(_buff)
#define IR_LOG_INFO(max, ...) char _bufi[max];snprintf(_bufi, max, __VA_ARGS__);LOG_INFO(_bufi);
static bool IR__fatalMoveFilePtr(uint itemIndex) {
	long address = DB_getItemOffset(itemIndex);
	IR_LOG_FATAL(128, "Could not move file ptr to item {Index: %u, Adress: %ld}",
		itemIndex, address);
	return false;
}
static bool IR__fatalMoveFilePtrToCount(uint relatedItemIndex, long offset) {
	IR_LOG_FATAL(128, "Could not move file ptr to living count(address: %ld) related itemIndex: %u",
		offset, relatedItemIndex);
	return false;
}
static bool IR__fatalUpdateLivingCount(int newCount, long offset) {
	IR_LOG_FATAL(128, "Could not update living count to %d in offset %ld",
		newCount, offset);
	return false;
}
static bool IR__fatalFlush(const char* forWhat) {
	IR_LOG_FATAL(128, "Could not flush database for %s", forWhat);
	return false;
}
static bool IR__fatalNullPtr(const char* funcName, const char* ptrName) {
	IR_LOG_FATAL(128, "NULL Pointer \"%s\" in function \"%s\"", ptrName, funcName);
	return false;
}


/**
 * @private private underlying method to operate database file
 * @param lCount  positive number means new living count number, -1 means keep original number
 * @param itemIndex  itemIndex of item you want to updating (insert/update)
 * @param item
 * @return bool
 */
bool ItemWriter__update(int lCount, uint itemIndex, DB_BaseUserItem* item) {
	FILE* fs = DB_getFileStream();
	if(!fs)
		return LOG_FATAL("ItemWriter__update: database file is not open yet"), false;

	/// @todo create log file can rolled back to avoiding dirty data

	// get offset of item
	long itemOffset = DB_getItemOffset(itemIndex);
	// test is offset avaliable.
	IR_SEEK(fs, itemOffset);
	if(ftell(fs) != itemOffset)
		return IR__fatalMoveFilePtr(itemIndex), false;

	int lCountOffset = DB_getLivingCountOffset();
	int lCountRepeatOffset = DB_getLivingCountRepeatOffset();

	// if living count need change
	if(lCount >= 0) {
		IR_SEEK(fs, lCountRepeatOffset);
		if(ftell(fs) != lCountRepeatOffset)
			return IR__fatalMoveFilePtrToCount(itemIndex, lCountRepeatOffset);
		// modify count repeat variable
		if(!IR_WRITE_INT(fs, &lCount))
			return IR__fatalUpdateLivingCount(lCount, lCountRepeatOffset);
		// flush count
		if(DB_flushToDisk() != 0)
			return IR__fatalFlush("living count repeat");
	}

	// seek to offset of item
	IR_SEEK(fs, itemOffset);
	// ftell check done above
	if(!IR_WRITE_ITEM(fs, item)) {
		IR_LOG_FATAL(128, "Write item {itemIndex: %u} to offset: %ld failed!",
			item->itemIndex, ftell(fs));
		return false;
	}
	if(DB_flushToDisk() != 0)
		return IR__fatalFlush("item");
	IR_LOG_INFO(128, "Wrote item {itemIndex: %u} to database file", item->itemIndex);

	// if living count need change
	if(lCount >= 0) {
		IR_SEEK(fs, lCountOffset);
		if(ftell(fs) != lCountOffset)
			return IR__fatalMoveFilePtrToCount(itemIndex, lCountOffset);
		// modify count repeat variable
		if(!IR_WRITE_INT(fs, &lCount))
			return IR__fatalUpdateLivingCount(lCount, lCountOffset);
		// flush count
		if(DB_flushToDisk() != 0)
			return IR__fatalFlush("living count");

		IR_LOG_INFO(128, "Update double living count variable in database file to %d",
			lCount);
	}

	/// update fragment blank space
	/// @todo check security (don't add fragment space duplicated (it'is dangerous))
	if(item->live == false)
		BlankSpaceManager_addFragment(item->itemIndex, false);

	return ItemReader__updateItemCacheInMemory(itemIndex, item);

}


bool ItemWriter_newItem(DB_BaseUserItem* newItem) {
	if(!newItem)
		return IR__fatalNullPtr("ItemWriter_newItem", "newItem");

	/// @todo check is overwrite ?
	uint oldLivingCount = ItemReader_getLivingCount();
	if(oldLivingCount >= DB_MAX_ITEM) {
		LOG_FATAL("Could not insert new item! ItemReader_getLivingCount() >= DB_MAX_ITEM");
		return false;
	}

	return ItemWriter__update(oldLivingCount+1, newItem->itemIndex, newItem);
}

bool ItemWriter_modifyItem(DB_BaseUserItem* modifyTo) {
	if(!modifyTo)
		return IR__fatalNullPtr("ItemWriter_modifyItem", "modifyTo");

	/// @todo check is item living ?
	return ItemWriter__update(-1, modifyTo->itemIndex, modifyTo);
}

bool ItemWriter_deleteItem(DB_BaseUserItem* oldItem) {
	if(!oldItem)
		return IR__fatalNullPtr("ItemWriter_deleteItem", "oldItem");

	uint oldLivingCount = ItemReader_getLivingCount();
	if(oldLivingCount <= 0) {
		LOG_FATAL("Could not delete! ItemReader_getLivingCount() <= 0");
		return false;
	}
	/// @todo check count number in database file ?
	/// @todo check is item living ?
	///
	DB_BaseUserItem item;
	IR_COPY_ITEM(&item, oldItem);

	item.live = DB_False;
	DB_calcUserItemHash(&item, item.hash);

	return ItemWriter__update(oldLivingCount-1, item.itemIndex, &item);
}
