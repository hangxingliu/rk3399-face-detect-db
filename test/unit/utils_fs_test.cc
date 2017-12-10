/// TestEntry: utils_fs_module
/// With: ../src/utils/fs.cc
/// With: ../src/log/methods.cc
/// ----------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/stat.h>

#include "../utilities/index.hpp"
#include "../../src/utils/utils.hpp"

char buf[128];

int main() {
	char result[128];

	joinPath("./", "abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 0");

	joinPath(".", "abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 1");

	joinPath(".", "/abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 2");

	joinPath("./", "/abc", result);
	if(testCompareStrings(result, "./abc") != 0) return 1;
	testPassed("compare 3");

	joinPath(nullptr, "/abc", result);
	if(testCompareStrings(result, "/abc") != 0) return 1;
	testPassed("compare 4");

	struct stat st;
	const char* createDir = "./test-env/a/b/cccddd";
	if(stat(createDir, &st) == 0) {
		if(rmdir(createDir) != 0) {
			sprintf(buf, "remove test folder failed: %s", createDir);
			return testFailed(buf);
		}
	}
	if(!mkdirRecursively(createDir)){
		sprintf(buf, "create test folder failed: %s", createDir);
		return testFailed(buf);
	}
	testPassed("create test folder recursively!");

	const char* testFile = "./test-env/reg.file";
	sprintf(buf, "touch %s", testFile);
	if(system(buf) != 0){
		sprintf(buf, "create test file failed: %s", testFile);
		return testFailed(buf);
	}
	if(!isPathAFile(testFile)) {
		sprintf(buf, "isPathAFile return false with a existed file: %s", testFile);
		return testFailed(buf);
	}
	testPassed("isPathAFile(file) => true");

	if(isPathAFile("./test-env/i-dont-existed"))
		return testFailed("isPathAFile return true with a non-existed path!");
	testPassed("isPathAFile(non-existed) => false");


	if(isPathAFile(createDir))
		return testFailed("isPathAFile return true with a directory path!");
	testPassed("isPathAFile(directory) => false");


	return testDone("Fs utils test");
}
