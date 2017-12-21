#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./index.hpp"

FILE* file;
char fileName[256];

int main(int argc, char** argv) {
	if(isLaunchForHelp(argc, argv))
		return printHelpforShell(), 0;

	if(argc != 2)
		return printHelpforShell(), 0;

	if(strlen(argv[1]) >= 256)
		return puts("Invalid fileName paramter!"), 1;

	strcpy(fileName, argv[1]);
	file = fopen(fileName, "rb+");
	if(!file)
		return printf("Open db file \"%s\" failed!\n", fileName), 2;
	setvbuf(file, NULL, _IONBF, 0);// FILE IO NO BUFFER

	printHelpforInsideREPL();

	bool end = false;
	char inputPrefix[1024];

	UserCommand lastCmd;
	UserCommand cmd;

	// print database header
	readDatabaseHeaderInfo(file);

	while(!end) {
		memcpy(&lastCmd, &cmd, sizeof(UserCommand));

		snprintf(inputPrefix, sizeof(inputPrefix), "%s %ld", fileName, ftell(file));

		if(!readCommand(inputPrefix, &cmd)) {
			end = true;
			continue;
		}

		if(cmd.is(".")) { //redo
			memcpy(&cmd, &lastCmd, sizeof(UserCommand));
			printf("Redo last command: `%s`\n", cmd.command);
		}

		if(cmd.isEmptyCommand())
			continue;

		if(cmd.is("e") ||
			cmd.is("exit") ||
			cmd.is("q") ||
			cmd.is("quit")) {

			end = true;
			continue;
		}

		if(cmd.is("head")) {

			readDatabaseHeaderInfo(file);
			continue;
		}

		if(cmd.is("u") ||
			cmd.is("user")) {

			readUserItemByCommand(file, cmd.parameters);
			continue;
		}

		if(cmd.is("r") ||
			cmd.is("read")) {

			readBytesByCommand(file, cmd.parameters, false);
			continue;
		}
		if(cmd.is("r+") ||
			cmd.is("read+")) {

			readBytesByCommand(file, cmd.parameters, true);
			continue;
		}


		if(cmd.is("w") ||
			cmd.is("write")) {

			writeBytesByCommand(file, cmd.parameters);
			continue;
		}

		if(cmd.isStartsWith("+") ||
			cmd.isStartsWith("=") ||
			cmd.isStartsWith("-")) {

			if(strlen(cmd.command) == 1)
				seekByCommand(file, cmd.command[0], cmd.parameters);
			else
				seekByCommand(file, cmd.command[0], cmd.command+1);
			continue;
		}

		printHelpforInsideREPL();
	}

	fclose(file);
	return 0;
}
