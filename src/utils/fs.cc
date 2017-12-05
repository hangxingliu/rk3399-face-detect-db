#include<unistd.h>

static inline bool _Fs_testFile(const char* path, int mode) {
	return access(path, mode) == 0;
}

bool isFileReadable(const char* path) { return _Fs_testFile(path, R_OK); }
bool isFileWritable(const char* path) { return _Fs_testFile(path, W_OK); }

