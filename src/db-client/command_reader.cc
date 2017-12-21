#include<stdio.h>
#include<string.h>

#include "./index.hpp"

static UserCommand command;
static char tmpStr[MAX_COMMAND_LENGTH];
static char tmpStr2[MAX_COMMAND_LENGTH];

#define isNotBlankChar(c) (c>32&&c<127)
static void strTrim(const char* str, OUT char* newStr) {
	if(!newStr) return;

	int strLen = strlen(str);
	int from = 0, to = strLen - 1;

	if(strLen == 0) {
		newStr[0] = 0;
		return;
	}

	for( ; from < strLen ; from ++  )
		if(isNotBlankChar(str[from]))
			break;
	if(from >= strLen) {
		newStr[0] = 0;
		return;
	}

	for( ; to >= 0 ; to -- )
		if(isNotBlankChar(str[to]))
			break;

	char *write = newStr;
	for( ; from <= to ; from ++ )
		*write++ = str[from];
	*write = 0;
}
static void splitCommandAndParameters(const char* str, OUT char* command, OUT char* parameters) {
	int strLen = strlen(str);
	int index = strcspn(str, " \t");
	memcpy(command, str, index);
	command[index] = 0;
	if(index >= strLen)
		parameters[0] = 0;
	else
		strcpy(parameters, str+index+1);
}

bool readCommand(const char* prefix, UserCommand* result) {
	printf("%s%s > %s", BLUE_BOLD, prefix, BLUE);

	bool ok = !!fgets(result->rawInput, MAX_COMMAND_LENGTH - 1, stdin);
	printf(RESET); // reset color

	if(ok) {
		strTrim(result->rawInput, tmpStr);
		splitCommandAndParameters(tmpStr, result->command, tmpStr2);
		strTrim(tmpStr2, result->parameters);
	}

	return ok;
}
