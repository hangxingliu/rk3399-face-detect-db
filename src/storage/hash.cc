#include "../types/base.hpp"
#include "../types/database.hpp"

#include "../log/log.hpp"

void DB_calcUserItemHash(
	DB_BaseUserItem* item,
	unsigned int* hashResult) {

	int hash[4] = {0,1,2,3};

	hash[0] += item->live;

	LOOP_TIMES(i, 7) { hash[i&3] += item->reserved[i]; }

	hash[1] += item->priority;

	LOOP_TIMES(i, USERID_LENGTH) { hash[i&3] += item->userId[i]; }

	auto features = &(item->features);
	auto len = features->len;
	hash[2] += len;
	LOOP_TIMES(i, len) { hash[i&3] += (int)(features->data[i] * 100); }

	LOOP_TIMES(i, 1024) { hash[i&3] += item->addonReserved[i]; }
	LOOP_TIMES(i, 4) { hashResult[i] = hash[i]; }
}

bool DB_validateUserItem(DB_BaseUserItem* item) {
	unsigned int hash[4];
	bool passed = true;

	DB_calcUserItemHash(item, hash);
	LOOP_TIMES(i, 4) {
		if(hash[i] != item->hash[i]) {
			passed = false;
			break;
		}
	}

	if(!passed) {
		char buf[256];
		sprintf(buf, "Wrong hash in UserItem{index: %d, id: \"%s\"}",
			item->itemNumber, item->userId);
		LOG_WARN(buf);
	}

	return passed;
}
