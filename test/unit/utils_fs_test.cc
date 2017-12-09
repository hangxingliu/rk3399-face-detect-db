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


	return testDone("Fs utils test");
}
