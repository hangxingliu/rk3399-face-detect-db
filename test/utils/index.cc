#include<stdio.h>

const char* COLOR_GREEN = "\x1b[32m";
const char* COLOR_RED = "\x1b[31m";
const char* COLOR_BLUE = "\x1b[34m";
const char* COLOR_RESET = "\x1b[0m";

int testFailed(const char* description) {
	printf("%s[-] failed: %s%s\n", COLOR_RED, description, COLOR_RESET);
	return 1;
}

int testPassed(const char* description) {
	printf("%s[-] passed: %s%s\n", COLOR_GREEN, description, COLOR_RESET);
	return 0;
}

int testDone(const char* description) {
	printf("%s[+] done: %s%s\n", COLOR_BLUE, description, COLOR_RESET);
	return 0;
}
