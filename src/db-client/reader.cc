#include<stdio.h>
#include "./index.hpp"

#define append(str, ...) sprintf(str+strlen(str), __VA_ARGS__)

#define MAX_BYTES_IN_ROW 128

const char* _EOF = " EOF\n";
const char* _NL = "\n";

void readBytes(FILE* file, uint bytesInRow, uint rows, bool followCursor) {
	if(bytesInRow >= MAX_BYTES_IN_ROW)
		return;
	char buffer[MAX_BYTES_IN_ROW];

	char out0[1024];  // byte
	char out1[1024]; // char
	char out2[1024]; // float
	char out3[1024]; // int

	char test[1024];
	uint actualLen = 0, expectLen = 0;
	long returnTo = ftell(file);

	for(uint i = 0 ; i < rows ; i ++ ) {
		long offset = ftell(file);
		uint readLen = fread(buffer, 1, bytesInRow, file);
		actualLen += readLen;
		expectLen += bytesInRow;

		sprintf(out0, "0x%04lx: ", offset);
		sprintf(out1, "%6ld: ", offset);
		sprintf(out2, "   int: ");
		sprintf(out3, " float: ");

		for(uint j = 0 ; j < readLen ; j ++ ) {
			append(out0, "%3d ", buffer[j]);
			if(buffer[j] >= 32 && buffer[j] <= 126)
				append(out1, "  %c ", buffer[j]);
			else
				append(out1, " -- ");

			if(j % 4 == 3) {
				append(out2, "%15d ", *((int*)&(buffer[j-3])) );
				float value = *((float*)&(buffer[j-3]));
				sprintf(test, "%.4f", value);
				if(strlen(test) > 15)
					sprintf(test, "%.4e", value);
				append(out3, "%15s ", test);

				append(out0, "  ");
				append(out1, "  ");
				append(out2, "  ");
				append(out3, "  ");
			}
		}

		const char* newLineChar = readLen < bytesInRow ? _EOF : _NL;
		printf("%s%s", out0, newLineChar);
		printf("%s%s", out1, newLineChar);
		printf("%s%s", out2, newLineChar);
		printf("%s%s\n", out3, newLineChar);

		if(newLineChar == _EOF)
			break;
	}

	if(!followCursor)
		fseek(file, returnTo, SEEK_SET);
}

void readBytesByCommand(FILE* file, const char* parameters, bool followCursor) {
	uint size = 16, rows = 1;
	if(strchr(parameters, 'x')) {
		sscanf(parameters, "%ux%u", &size, &rows);
	} else {
		sscanf(parameters, "%u", &size);
	}
	readBytes(file, size, rows, followCursor);
}
