#include<stdio.h>
#include<string.h>

#include "./macro.hpp"

int testFailed(const char* description) {
	testFailedFormat("%s", description);
	return 1;
}

int testPassed(const char* description) {
	testPassedFormat("%s", description);
	return 0;
}

int testDone(const char* description) {
	__T_PRINT(COLOR_BLUE, '+', "done", "%s", description);
	return 0;
}

int testCompareStrings(const char* s1, const char* s2) {
	if(strcmp(s1, s2) == 0)
		return 0;
	testFailedFormat("\"%s\" != \"%s\"", s1, s2);
	return 1;
}
