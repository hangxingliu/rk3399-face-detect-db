#include<stdio.h>
#include<string.h>

#ifndef DB_CLIENT_BASE_HEPP
#define DB_CLIENT_BASE_HEPP

#ifndef OUT
	#define OUT
#endif

#define RED "\x1b[0;31m"
#define RED_BOLD "\x1b[1;31m"
#define GREEN "\x1b[0;32m"
#define GREEN_BOLD "\x1b[1;32m"
#define BLUE "\x1b[0;34m"
#define BLUE_BOLD "\x1b[1;34m"
#define YELLOW "\x1b[0;33m"
#define YELLOW_BOLD "\x1b[1;33m"
#define CYAN "\x1b[0;36m"
#define CYAN_BOLD "\x1b[1;36m"

#define RESET "\x1b[0m"


typedef unsigned int uint;
typedef unsigned char uchar;

#define MAX_COMMAND_LENGTH 1024
class UserCommand {
public:
	char rawInput[MAX_COMMAND_LENGTH];

	char command[MAX_COMMAND_LENGTH];
	char parameters[MAX_COMMAND_LENGTH];

	UserCommand() {
		rawInput[0] = 0;
		command[0] = 0;
		parameters[0] = 0;
	}
	bool isEmptyCommand() { return command[0] == 0; }
	bool is(const char* cmd) { return strcmp(command, cmd) == 0; }
	bool isStartsWith(const char* cmdPrefix) {
		size_t lenPrefix = strlen(cmdPrefix),
			lenCommand = strlen(command);
		return lenCommand < lenPrefix ? false : (strncmp(cmdPrefix, command, lenPrefix) == 0);
	}
};

#endif // DB_CLIENT_BASE_HEPP
