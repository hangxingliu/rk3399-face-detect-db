/// TestEntry: storage
/// With: ../src/storage/*.cc
/// With: ../src/log/*.cc
/// With: ../src/utils/fs.cc
/// With: ../src/global/configurations.cc
/// ----------------------

#include<stdio.h>
#include<string.h>

#include "../../utilities/index.hpp"

#include "../../../src/log/log.hpp"
#include "../../../src/global/global.hpp"
#include "../../../src/storage/storage.hpp"

const char* ENV = "./test-env/storage/";

void help() {
	puts("Help:");
	puts("  p ${userId} ${priority}");
	puts("  d ${userId}");
	puts("  q/e");
}
void prompt() { printf("repl > "); }


int main() {
#ifndef DEBUG
	testFailed("Storage REPL test only for debug mode");
	return 0;
#else
	LOG_DEBUG("Storage REPL test:");
#endif
	printf("test environment: %s\n", ENV);

	const int PING_PING = 63;
	if(DB_pingPong(PING_PING) != PING_PING+1)
		return testFailed("DB_pingPong(63) != 64");

	GlobalConfig config;
	strcpy(config.workspacePath, ENV);

	if(Config_initGlobalConfig(&config) != 0)
		return testFailed("Init config failed!");
	testPassed("Initialized config");

	if(DB_init() != 0)
		return testFailed("Init database failed!");
	testPassed("Initialized database");

	char line[1024];
	char userId[128];
	while(prompt(), fgets(line, 1024, stdin)) {
		if(strlen(line) < 1) continue;
		if(line[0] == 'q' || line[0] == 'e') break;
		if(line[0] == 'p') { //priority
			int priority = 0;
			sscanf(line+1, "%s %d", userId, &priority);
			printf("DB_updatePriority(%s, %d):\n", userId, priority);
			DB_updatePriority(userId, priority);
		} else if(line[0] == 'd') { //delete
			sscanf(line+1, "%s", userId);
			printf("DB_deleteUser(%s):\n", userId);
			DB_deleteUser(userId);
		} else {
			help();
		}
	}

	if(DB_close() != 0)
		return testFailed("Close database failed!");

	return testDone("Test storage repl");
}
