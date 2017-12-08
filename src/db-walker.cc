#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef OUT
#define OUT
#endif

#define append(str, ...) sprintf(str+strlen(str), __VA_ARGS__)
#define prompt() printf("%s %ld > ", fileName, ftell(file))

typedef unsigned int uint;
typedef unsigned char uchar;

const char* _EOF = " EOF\n";
const char* _NL = "\n";

FILE* file;
char fileName[256];
uchar buffer[1024];

void help_cli() {
	puts("usage: db-walker $databaseFileName");
}
void help_inside() {
	puts("example command:");
	puts("  e:   exit   q:   exit");
	puts("  r:   read 16 bytes");
	puts("  rr:   read 16 bytes and move pointer forward to 16 bytes");
	puts("  r 16x2:   read 32 bytes in 2 rows");
	puts("  rr 16x2:   read 32 bytes in 2 rows and move pointer forward to 32 bytes");
	puts("  wi100:   write 4 bytes of number 100");
	puts("  wf100.4:   write 4 bytes of number 100.4 as type 'float'");
	puts("  wc65:   write char 'A'");
	puts("  wsHelloWorld:   write string \"HelloWorld\"");
	puts("  =10:   move file pointer to 10");
	puts("  +20:   move file pointer forward 20 bytes");
	puts("  -5:   move file pointer backward 5 bytes");
	puts("tip: number could be replaced to hexadecimal number. 0x41 => 65");
}

char parseInput(char* line, OUT char** parameters) {
	int len = strlen(line);
	if(!len) return 0;
	char cmd = line[0];
	char *r = line+1; char *w = line;
	char c;
	bool ignoreBlank = false;
	while((c = *r++) > 0) {
		if(!ignoreBlank && (c == ' ' || c == '\t'))
			continue;
		ignoreBlank = true;
		*w++ = c;
	}
	// trim right
	*w-- = 0;
	while((c = *w) &&
		(c == ' ' || c == '\t' || c == '\n' || c == '\r')){
		*w-- = 0;
	}
	*parameters = line;
	return cmd;
}

void printContent(uint bufferSize = 16, uint bufferRows = 1, bool followCursor = true) {
	char out0[1024]; char out1[1024]; char out2[1024]; char out3[1024];
	char test[1024];
	uint len = 0, expectLen = 0;
	long returnTo = ftell(file);
	for(uint i = 0 ; i < bufferRows ; i ++ ) {
		long offset = ftell(file);
		uint readLen = fread(buffer, 1, bufferSize, file);
		len += readLen;
		expectLen += bufferSize;

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
		const char* nl = readLen < bufferSize ? _EOF : _NL;
		printf("%s%s%s%s%s%s%s%s\n", out0, nl, out1, nl, out2, nl, out3, nl);
		if(nl == _EOF) break;
	}

	if(!followCursor)
		fseek(file, returnTo, SEEK_SET);
}

int seek(char cmd, char* params) {
	int value = -1;
	int len = strlen(params);
	if(len < 1)
		return printf("Usage: %c$NUMBER / %c0x$HEX_NUMBER\n", cmd, cmd), 1;

	bool isHex= len > 2 && params[0] == '0' && params[1] == 'x';
	sscanf(params + (isHex?2:0), isHex ? "%x" : "%d", &value);

	if(value < -1)
		return printf("Invalid move value!"), 1;
	long now = ftell(file);

	switch(cmd){
	case '=':
		return fseek(file, value, SEEK_SET), 0;
	case '-':
		if(value > now)
			value = (int) now;
		return fseek(file, now - value, SEEK_SET), 0;
	default: // '+':
		return fseek(file, value, SEEK_CUR), 0;
	}
}

void read(char* params) {
	bool follow = false;
	if(params[0] == 'r') {
		follow = true;
		params++;
	}

	uint size = 16, rows = 1;
	if(strchr(params, 'x')) {
		sscanf(params, "%dx%d", &size, &rows);
	} else {
		sscanf(params, "%d", &size);
	}
	printContent(size, rows, follow);
}

bool write(void* data, int length, const char* name) {
	int wrote = fwrite(data, 1, length, file);
	if(wrote == length) {
		//echo
		fseek(file, ftell(file) - wrote, SEEK_SET);
		int displayLen = wrote > 16 ? 16 : wrote;
		printContent(displayLen, 1, true);

		return printf("Wrote %s (%d bytes) successed!\n", name, length), true;
	}
	return printf("Write %s failed! (expect %d bytes, but %d bytes)\n", name, length, wrote), false;
}

int write(char* params) {
	int len = strlen(params);
	if(len < 2)
		return printf("Invalid write opreation!"), 1;

	if(params[0] == 's') {
		return write(params+1, len-1, "string"), 0;
	} else if(params[0] == 'c'){
		int value = 0;
		if(strstr(params, "0x")) sscanf(params+1, "0x%x", &value);
		else sscanf(params+1, "%d", &value);
		char c = (char)(value & 0xFF);
		return write(&c, 1, "char"), 0;
	} else if(params[0] == 'i') {
		int value = 0;
		if(strstr(params, "0x")) sscanf(params+1, "0x%x", &value);
		else sscanf(params+1, "%d", &value);
		return write(&value, 4, "integer"), 0;
	} else if(params[0] == 'f') {
		float value = 0;
		sscanf(params+1, "%f", &value);
		return write(&value, 4, "float"), 0;
	} else {
		return printf("Invalid write type %c\n", params[0]), 1;
	}
}

int main(int argc, char** argv) {
	if(argc != 2)
		return help_cli(), 0;
	if(strlen(argv[1]) >= 256)
		return puts("Invalid fileName paramter!"), 1;
	strcpy(fileName, argv[1]);

	file = fopen(fileName, "rb+");
	if(!file)
		return printf("Open db file \"%s\" failed!\n", fileName), 2;
	help_inside();

	char line[1024];
	bool end = false;
	while(!end) {
		prompt();

		fgets(line, sizeof(line), stdin);

		char* param = NULL;
		char cmd = parseInput(line, &param);
		if(!param) { printf("Invalid input!"); continue; }

		switch(cmd) {
		case 'e':
		case 'q':
			end = true;
			break;
		case 'r':
			read(param);
			break;
		case 'w':
			write(param);
			break;
		case '+':
		case '-':
		case '=':
			seek(cmd, param);
			break;
		default:
			help_inside();
		}
	}

	fclose(file);
	return 0;
}
