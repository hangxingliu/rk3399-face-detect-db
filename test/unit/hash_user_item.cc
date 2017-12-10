/// TestEntry: hash_user_item
/// With: ../../src/storage/hash.cc
/// With: ../../src/log/methods.cc
/// ----------------------

#include <stdio.h>
#include <string.h>

#include "../utilities/index.hpp"
#include "../../src/types/database.hpp"
#include "../../src/storage/storage.hpp"

typedef unsigned int uint;

#define compareHash(hash, v0, v1, v2, v3)\
	(hash[0] == v0 && hash[1] == v1 && hash[2] == v2 && hash[3] == v3)

char buf[256];
void dumpHash(char* str, uint* hash) {
	sprintf(str, "{%d, %d, %d, %d}", hash[0], hash[1], hash[2], hash[3]);
}
bool testHash(uint* hash, uint v0, uint v1, uint v2, uint v3) {
	if(compareHash(hash, v0, v1, v2, v3)) {
		sprintf(buf, "hash equals {%d, %d, %d, %d}", v0, v1, v2, v3);
		return testPassed(buf), true;
	} else {
		char buf2[128];
		dumpHash(buf2, hash);
		sprintf(buf, "hash %s(actual) != {%d, %d, %d, %d}(expect)",
			buf2, v0, v1, v2, v3);
		return testFailed(buf), false;
	}
}


int main() {
	uint hash[4] = {0,0,0,0};

	DB_BaseUserItem item;
	DB_calcUserItemHash(&item, hash);
	if(!testHash(hash, 0, 1, 2, 3)) return -1;

	item.live = DB_True;
	DB_calcUserItemHash(&item, hash);
	if(!testHash(hash, 1, 1, 2, 3)) return -1;

	item.priority = 100;
	DB_calcUserItemHash(&item, hash);
	if(!testHash(hash, 1, 101, 2, 3)) return -1;




	return testDone("sizeof structures test");
}
