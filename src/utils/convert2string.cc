#include<stdio.h>
#include<string.h>

void int2String(int value, char* result, bool withQuote) {
	sprintf(result, withQuote ? "\"%d\"" : "%d", value);
}

void addQuote2String(const char* value, char* result) {
	sprintf(result, "\"%s\"", value);
}
