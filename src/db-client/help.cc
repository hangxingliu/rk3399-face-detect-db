#include<stdio.h>
#include<string.h>

bool isLaunchForHelp(int argc, char** argv) {
	for(int i = 0 ; i < argc ; i ++ )
		if(strcmp("--help", argv[i]) == 0 || strcmp("-h", argv[i]) == 0)
			return true;
	return false;
}

void printHelpforShell() {
	puts("usage: db-walker $databaseFileName");
}
void printHelpforInsideREPL() {
	puts("Commands:");
	puts("  e/exit/q/quit:    close and exit");
	puts("  r/read [16x1]:    read 16x1 bytes and display in 1 line");
	puts("  r+/read+ [8x2]:    read 8x2=16 bytes and display in 2lines. then move file cursor forward 16 bytes");
	puts("  w/write <i|c|f|s> <value>:    write value as <int|char|float|string> in file cursor location");
	puts("  +/=/- <offset>:    move file cursor. +:forward/=:set/-:backward");
	puts("  u/user [itemIndex]:    read user item content in cursor or special itemIndex");
	puts("  head:    read database file header information");
	puts("  .:    redo last command");
	puts(" ");
	puts("Examples:");
	puts("  write char 0x41:    write charachter 'A' into file. (same as: `write char 65`)");
	puts("  + 0x10:    move file cursor forward 16 bytes.");
}
