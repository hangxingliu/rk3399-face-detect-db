#include<stdio.h>
#include "./index.hpp"

void writeBytes(FILE* file, void* data, int length, const char* name) {
	int wrote = fwrite(data, 1, length, file);
	if(wrote == length) {
		//echo
		fseek(file, ftell(file) - wrote, SEEK_SET);
		int displayLen = wrote > 16 ? 16 : wrote;
		readBytes(file, displayLen, 1, true);

		printf("Wrote %s (%d bytes) successed!\n", name, length);
		return;
	}
	printf("Write %s failed! (expect %d bytes, but %d bytes)\n", name, length, wrote);
}


void writeBytesByCommand(FILE* file, const char* parameters) {

	char type;
	char valueStr[MAX_COMMAND_LENGTH];
	if(sscanf(parameters, "%c %s", &type, valueStr) != 2) {
		puts("Usage: write <type> <value>");
		return;
	}

	if(type == 's') {
		writeBytes(file, valueStr, strlen(valueStr), "string");
		return;
	}
	if(type == 'c'){
		unsigned int value = 0;
		if(strstr(valueStr, "0x"))
			sscanf(valueStr, "0x%x", &value);
		else
			sscanf(valueStr, "%u", &value);

		char c = (char)(value & 0xFF);
		writeBytes(file, &c, 1, "char");
		return;
	}
	if(type == 'i') {
		unsigned int value = 0;
		if(strstr(valueStr, "0x"))
			sscanf(valueStr, "0x%x", &value);
		else
			sscanf(valueStr, "%u", &value);

		writeBytes(file, &value, 4, "integer");
		return;
	}
	if(type == 'f') {
		float value = 0;
		sscanf(valueStr, "%f", &value);

		writeBytes(file, &value, 4, "float");
		return;
	}

	printf("Invalid write type %c\n", type);
}
