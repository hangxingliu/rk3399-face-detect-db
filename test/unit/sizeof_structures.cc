/// TestEntry: sizeof_structures
/// ----------------------

#include <stdio.h>
#include <string.h>

#include "../utilities/index.hpp"
#include "../../src/types/database.hpp"

char buf[128];

int sizeofFailed(const char* name, size_t expect, size_t actual) {
	sprintf(buf, "sizeof(%s) != %zu but %zu", name, expect, actual);
	return testFailed(buf);
}
int sizeofPassed(const char* name, size_t expect) {
	sprintf(buf, "sizeof(%s) is %zu", name, expect);
	return testPassed(buf);
}

int main() {
	size_t result = 0;


	const int sizeofBaseUserItem = 25124;
	result = sizeof(DB_BaseUserItem);
	if(result != sizeofBaseUserItem)
		return sizeofFailed("DB_BaseUserItem", sizeofBaseUserItem, result);
	sizeofPassed("DB_BaseUserItem", result);

	return testDone("sizeof structures test");
}
