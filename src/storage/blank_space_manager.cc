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

bool BlankSpaceManager_addFragment(uint itemIndex) {
	if(fragmentLength >= DB_MAX_ITEM)
		return false;
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

