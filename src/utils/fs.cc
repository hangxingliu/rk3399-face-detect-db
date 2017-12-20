#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include "../log/log.hpp"

static inline bool _Fs_testFile(const char* path, int mode) {
	return access(path, mode) == 0;
}

bool isFileReadable(const char* path) { return _Fs_testFile(path, R_OK); }
bool isFileWritable(const char* path) { return _Fs_testFile(path, W_OK); }

bool isPathAFile(const char* path) {
	struct stat st;
	if(stat(path, &st) != 0)
		return false;
	return S_ISREG(st.st_mode);
}


bool mkdirIfNotExist(const char* dir, mode_t mode) {
	struct stat st;
	if(stat(dir, &st) < 0)
		return mkdir(dir, mode) == 0;
	return S_ISDIR(st.st_mode);
}

/// https://stackoverflow.com/questions/2336242/recursive-mkdir-system-call-on-unix
bool mkdirRecursively(const char *dir) {
	char tmp[256];
	char *p = NULL;
	size_t len;

	snprintf(tmp, sizeof(tmp),"%s", dir);
	len = strlen(tmp);
	if(tmp[len - 1] == '/')
		tmp[len - 1] = 0;
	for(p = tmp + 1; *p; p++) {
		if(*p == '/') {
			*p = 0;
			// S_IRWXU: READ WRITE AND EXECUTE FOR OWNER
			if(!mkdirIfNotExist(tmp, S_IRWXU)) {
				LOG_FATAL_F("Create folder failed: \"%s\"", tmp);
				return false;
			}
			*p = '/';
		}
	}
	if(!mkdirIfNotExist(tmp, S_IRWXU)) {
		LOG_FATAL_F("Create folder failed: \"%s\"", tmp)
		return false;
	}
	return true;
}

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

