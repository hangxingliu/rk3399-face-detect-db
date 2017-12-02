#include<stdio.h>
#include<strings.h>
#include<memory.h>
#include "./utils/index.hpp"

#define SIZE 128

int main() {
	if(sizeof(long) == 8) {
		testPassed("sizeof(long) == 8");
	} else {
		char desc[SIZE];
		sprintf(desc, "sizeof(long) != 8 but %zu", sizeof(long));
		return testFailed(desc);
	}

	if(sizeof(float) == 4) {
		testPassed("sizeof(float) == 4");
	} else {
		char desc[SIZE];
		sprintf(desc, "sizeof(float) != 4 but %zu", sizeof(long));
		return testFailed(desc);
	}

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



	return testDone("C++ test");
}
