#include<stdio.h>
#include<string.h>

const char* COLOR_GREEN = "\x1b[32m";
const char* COLOR_RED = "\x1b[31m";
const char* COLOR_BLUE = "\x1b[34m";
const char* COLOR_RESET = "\x1b[0m";

#define __T_PRINT(color, mark, name, ps, ...)\
	char __t_print_str[128];\
	sprintf(__t_print_str, "%s[%c] %s: %s%s\n", color, mark, name, ps, COLOR_RESET);\
	printf(__t_print_str, __VA_ARGS__);

#define __T_FAILED(ps, ...) __T_PRINT(COLOR_RED, '-', "failed", ps, __VA_ARGS__);
#define __T_PASSED(ps, ...) __T_PRINT(COLOR_GREEN, '~', "passed", ps, __VA_ARGS__);
#define __T_DONE(desc) __T_PRINT(COLOR_BLUE, '+', "done", "%s", desc);


int testFailed(const char* description) {
	__T_FAILED("%s", description);
	return 1;
}

int testPassed(const char* description) {
	__T_PASSED("%s", description);
	return 0;
}

int testDone(const char* description) {
	__T_DONE(description);
	return 0;
}

int testCompareStrings(const char* s1, const char* s2) {
	if(strcmp(s1, s2) == 0)
		return 0;
	__T_FAILED("\"%s\" != \"%s\"", s1, s2);
	return 1;
}
