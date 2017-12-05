#include<unistd.h>
#include<stdio.h>
#include<string.h>

static inline bool _Fs_testFile(const char* path, int mode) {
	return access(path, mode) == 0;
}

bool isFileReadable(const char* path) { return _Fs_testFile(path, R_OK); }
bool isFileWritable(const char* path) { return _Fs_testFile(path, W_OK); }

void joinPath(const char* p0, const char* p1, char* result) {
	bool isSlash = false;
	if(p0) {
		while(*p0) {
			isSlash = (*p0) == '/';
			*result++ = *p0++;
		}
	}
	if(p1) {
		char c1 = *p1;
		if(c1 && c1 != '/' && !isSlash) *result++ = '/';
		if(c1 == '/' && isSlash) p1++;
		while(*p1)
			*result++ = *p1++;
	}
	*result = 0;
}
