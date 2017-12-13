/// TestEntry: utils_memory_module
/// With: ../src/log/methods.cc
/// ----------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/stat.h>

#include "../utilities/index.hpp"
#include "../../src/utils/memory.hpp"

bool revoked = false;
uint revokedIndex = 0;

typedef struct Point {
	int x, y;
} Point;
void Point_toString(const Point* p, char* str) {
	sprintf(str, "Point{x: %d, y: %d}", p->x, p->y);
}

void revokeCallback(uint index) {
	revokedIndex = index;
	revoked = true;
	printf("Revoke log: %u\n", index);
}

int main() {
	const int maxAllocateItem = 5;
	const size_t maxAllocate = sizeof(Point) * maxAllocateItem;

	MemoryManager<Point> mm(1024, maxAllocate);
	mm.registerRevokeCallback(revokeCallback);
	testPassedFormat("create a memory manager with max allocate %zu", maxAllocate);

	for(int i = 0 ; i < maxAllocateItem ; i ++ )
		mm.allocate((uint) (i + 100) );
	if(revoked != false)
		return testFailed("Wrong revoke before memory limited size");
	testPassedFormat("allocate %d item without revoking", maxAllocateItem);

	mm.allocate(1000);
	if(!revoked || revokedIndex != (0 + 100) )
		return testFailed("Revoking is missing before allocate new item");
	testPassedFormat("revoke %u before allocate new item", revokedIndex);
	revoked = false;

	mm.allocate(1000);
	if(revoked)
		return testFailed("Wrong revoke before allocate item allocated");
	testPassedFormat("reuse allocated item %u", 1000);
	revoked = false;

	auto list = mm.getItemList();
	for(uint i = 0, i2 = mm.getItemListCapacity(); i < i2 ; i ++ ) {
		bool ok = false;
		if(i == 1000 || i == 101 || i == 102 || i == 103 || i == 104) {
			if(list[i])
				ok = true;
		} else if(!list[i]) {
			ok = true;
		}
		if(!ok) {
			testFailedFormat("item %u is wrong", i);
			return -1;
		}
	}
	testPassed("all item in memory manager list is correct");

	return testDone("Fs utils test");
}
