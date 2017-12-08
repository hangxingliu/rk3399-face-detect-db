/// TestEntry: random_access_file
/// ----------------------

#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>

const char* DIRNAME = "./test-env";
const char* FILENAME = "./test-env/data.dat";

FILE* file;

int seek(long int offset, int origin) {
	int status = fseek(file, offset, origin);
	if(status < 0) {
		printf("%d %s\n", status, strerror(errno));
	} else {
		printf("0 SUCCESS\n ftell => %ld\n", ftell(file));
	}
	return status;
}


int main() {
	DIR* dir = opendir(DIRNAME);
	if(dir == NULL) { puts("Folder file located in is missing!"); return 1; }

	struct stat fileInfo;
	if(stat(FILENAME, &fileInfo) != 0) {
		//create file
		puts("Creating file ...");
		if(creat(FILENAME, 0644) < 0) {
			puts("Create file failed!");
			return 1;
		}
	}

	file = fopen(FILENAME, "rb+");
	if(!file) { puts("Open file failed!"); return 1;}

	printf("s $offset $origin\n");
	printf("%d:SEEK_SET; %d:SEEK_CUR; %d:SEEK_END;\n", SEEK_SET, SEEK_CUR, SEEK_END);

	char line[1024];

	long offset;
	int origin;
	char buffer[1024];
	int length;
	int realLength;
	int rawLen;

	printf("ftell:%ld > ", ftell(file));
	while(fgets(line, sizeof(line), stdin)) {
		switch(line[0]) {
		case 's':
			sscanf(line+1, "%ld%d", &offset, &origin);
			printf("seek(%ld, %d) => ", offset, origin);
			seek(offset, origin);
			break;
		case 'w':
			rawLen = strlen(line);
			line[rawLen - 1] = 0; // remove new line character

			length=strlen(line+2);
			printf("write(`%s`) => ", line+2);
			realLength = fwrite(line+2, 1, length, file);
			fflush(file);
			printf("(Wrote: %d/All: %d)\n", realLength, length);
			break;
		case 'r':
			length=0;
			sscanf(line+1, "%d", &length);
			if(!length) length = 32;
			printf("read(%d) from %ld =>\n  ", length, ftell(file));
			realLength = fread(buffer, 1, length, file);
			printf("(len: %d): ", realLength);
			buffer[realLength] = 0;
			printf("`%s`\n", buffer);
			break;
		default:
			goto END;
		}
		printf("ftell:%ld > ", ftell(file));
	}

	END:
	fclose(file);
	return 0;
}

