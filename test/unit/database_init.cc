/// TestEntry: database_init
/// With: ../src/utils/fs.cc
/// With: ../src/log/methods.cc
/// With: ../src/storage/main.cc
/// With: ../src/global/configurations.cc
/// ----------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include<sys/stat.h>

#include "../utilities/index.hpp"
#include "../../src/storage/storage.hpp"

char buf[128];

int main() {

	const char* dir = "./test-env/db";
	const char* file = "./test-env/db/test.db";


	sprintf(buf, "rm %s -rfv", dir);
	if(system(buf) != 0)
		return testFailed("Delete old test database file failed!");

	int status = DB_createDatabaseFile(file, 4096 * 2);
	if(status != 0)
		return testFailed("Create database file");

	testPassed("Created database file");

	unsigned char head[] = {
		'F', 'a', 'c', 'e',
		10, 44, 96, 150,
		1, 0, 0, 0,
		2, 0, 0, 0,
		2, 0, 0, 0
	};
	int count = 0;
	if(DB_validateHead(head, &count) != 0)
		return testFailed("Validate head of database file");
	if(count != 2) {
		sprintf(buf, "Validate head failed! count(%d) != 2", count);
		return testFailed(buf);
	}

	head[3] = 'X';
	if(DB_validateHead(head, &count) == 0)
		return testFailed("Validate result is OK, but head of database file is wrong");

	head[3] = 'e';
	head[9] = 1;
	if(DB_validateHead(head, &count) == 0)
		return testFailed("Validate result is OK, but head of database file is wrong");


	head[9] = 0;
	head[12] = 3;
	if(DB_validateHead(head, &count) == 0)
		return testFailed("Validate result is OK, but head of database file is wrong");

	testPassed("Validate head of database file");


	return testDone("Init database");
}
