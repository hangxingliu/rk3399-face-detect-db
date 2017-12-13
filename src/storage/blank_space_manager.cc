#include <stdlib.h>

#include "../log/log.hpp"
#include "../types/database.hpp"
#include "../types/base.hpp"


static uint fragmentSpaces[DB_MAX_ITEM];
static uint fragmentLength = 0;
static uint continuousSpace = 1;

void BlankSpaceManager_init() {
	continuousSpace = 1;
	fragmentLength = 0;
}

bool BlankSpaceManager_addFragment(uint itemIndex, bool initializing) {
	if(fragmentLength >= DB_MAX_ITEM)
		return false;

	if(!initializing) {
		// check duplicated
		if(fragmentLength > 0 && fragmentSpaces[fragmentLength - 1] == itemIndex) {
			LOG_FATAL_F("Duplicated fragment space insert operation with itemIndex: %u", itemIndex);
			return false;
		}
		LOG_INFO_F("New fragment space be added. itemIndex: %u", itemIndex);
	}

	fragmentSpaces[fragmentLength++] = itemIndex;
	return true;
}

void BlankSpaceManager_setContinuousSpaceStart(uint itemIndex) {
	continuousSpace = itemIndex;
}

uint BlankSpaceManager_getSpace() {
	if(fragmentLength)
		return fragmentSpaces[--fragmentLength];
	return continuousSpace++;
}

