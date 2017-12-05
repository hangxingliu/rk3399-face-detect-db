#include<stdio.h>
#include<strings.h>
#include<stdlib.h>
#include<memory.h>
#include<time.h>
#include "./utils/index.hpp"

#define SIZE 128
char str[SIZE];

int sizeofFailed(const char* name, size_t expect, size_t actual) {
	sprintf(str, "sizeof(%s) != %zu but %zu", name, expect, actual);
	return testFailed(str);
}
int sizeofPassed(const char* name, size_t expect) {
	sprintf(str, "sizeof(%s) is %zu", name, expect);
	return testPassed(str);
}

int main() {

	const size_t POINTER_SIZE = sizeof(void*);

	if(sizeof(long) != 8)
		return sizeofFailed("long", 8, sizeof(long));
	sizeofPassed("long", 8);

	if(sizeof(float) != 4)
		return sizeofFailed("float", 4, sizeof(float));
	sizeofPassed("float", 4);

	char charArrayStatic[] = { 1, 1, 2, 3, 5, 8 };
	char *charArrayDynamic = (char*) malloc(6);

	if(sizeof(charArrayStatic) != 6)
		return sizeofFailed("charArrayStatic", 6, sizeof(charArrayStatic));
	sizeofPassed("charArrayStatic", 6);

	if(sizeof(charArrayDynamic) != POINTER_SIZE)
		return sizeofFailed("charArrayDynamic", POINTER_SIZE, sizeof(charArrayDynamic));
	sizeofPassed("charArrayDynamic", POINTER_SIZE);

	return testDone("C++ malloc and sizeof test");
}
