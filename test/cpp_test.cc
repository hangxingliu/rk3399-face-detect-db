#include<stdio.h>
#include<strings.h>
#include<stdlib.h>
#include<memory.h>
#include<time.h>
#include "./utils/index.hpp"

#define SIZE 128

int return1() { return 0, 1; }

int main() {

	char data[SIZE];
	float a = 102.56, b = a, c = 0;
	char* p = (char*)&b;
	// float to bytes
	for(int i = 0 ; i < sizeof(float); i++ ) {
		data[i] = *p;
		p++;
	}
	// bytes to float
	p = (char*)&c;
	for(int i = 0 ; i < sizeof(float); i++ ) {
		*p = data[i];
		p++;
	}
	if(b == c) {
		testPassed("copy float value via char*");
	} else {
		return testFailed("copy float value via char*");
	}


	char buffer[SIZE];
	a = 134.56; b = a; c = 0;
	memcpy(buffer, &b, sizeof(float));
	memcpy(&c, &b, sizeof(float));
	if(b == c) {
		testPassed("copy float value via memcpy");
	} else {
		return testFailed("copy float value via memcpy");
	}

	// Date
	time_t timestamp = time(NULL);
	tm* date = localtime(&timestamp);
	printf("Now timestamp: %lld\n", (long long)timestamp);
	printf("Readable date: %d-%d-%d %d:%d:%d\n",
		1900 + date->tm_year, 1 + date->tm_mon, date->tm_mday,
		date->tm_hour, date->tm_min, date->tm_sec);

	//memcpy and memccpy
	char copyFrom[] = {10, 20, 0, 40, 50, 60, 0, 70};
	char copyTo[SIZE];
	int copySize = sizeof(copyFrom);
//	printf("sizeof copyFrom = %zu\n", sizeof(copyFrom));
	printf("memcpy return pointer points to %d == 10\n", *(char*)memcpy(copyTo, copyFrom, copySize));
	printf("copyTo: [%d, %d, %d, %d, ...]\n", copyTo[0], copyTo[1], copyTo[2], copyTo[3]);
	printf("memccpy return pointer points to %d == 50 (next of item 40)\n",
		*(char*)memccpy(copyTo, copyFrom, 40, copySize));
	printf("memccpy return pointer is %p == 0 (NULL)\n", memccpy(copyTo, copyFrom, 90, copySize));

	int* ip = (int*) malloc(sizeof(int) * 100);
	printf("sizeof(ip) == %zu\n", sizeof(*ip));
	free(ip);

	// string pointer iterate
	const char* str = "HelloWorld!";
	const char* strPoint = str;
	putchar('"');
	while(*strPoint) { putchar(*strPoint++); }
	putchar('"');
	putchar('\n');

	printf("(return 0, 1) == %d\n", return1());

	return testDone("C++ test");
}
