#include<stdio.h>
#include "./index.hpp"

#define printSeekHelp(cmd) printf("Usage: %c$NUMBER / %c0x$HEX_NUMBER\n", cmd, cmd);

void seekByCommand(FILE* file, char cmd, char* params) {
	int value = -1;
	int len = strlen(params);
	if(len < 1) {
		printSeekHelp(cmd);
		return;
	}

	int sscanfResult = -1;
	if(strstr(params, "0x"))
		sscanfResult = sscanf(params, "0x%x", &value);
	else
		sscanfResult = sscanf(params, "%d", &value);

	if(sscanfResult != 1 || value <= -1) {
		printSeekHelp(cmd);
		return;
	}

	long now = ftell(file);
	switch(cmd){
	case '=':
		fseek(file, value, SEEK_SET);
		return;
	case '-':
		if(value > now)
			value = (int) now;
		fseek(file, now - value, SEEK_SET);
		return;
	default: // '+':
		fseek(file, value, SEEK_CUR);
		return;
	}
}
