#include <stdio.h>
#include <string.h>

#include "./utils/index.hpp"
#include "../src/utils/utils.hpp"

char buf[128];

int main() {
	char result[128];

	joinPath("./", "abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 0");

	joinPath(".", "abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 1");

	joinPath(".", "/abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 2");

	joinPath("./", "/abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 3");

	joinPath(nullptr, "/abc", result);
	if(testCompareStrings(result, "/abc") != 0) return 1;
	testPassed("compare 4");

	return testDone("Fs utils test");
}
