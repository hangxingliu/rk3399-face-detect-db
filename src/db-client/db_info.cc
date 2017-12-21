#include<stdio.h>
#include "./index.hpp"

#define DB_HEAD_SIZE 20
#define DB_ITEM_SIZE 65536

typedef struct UserItem {
	unsigned int itemIndex = 0;
	char live = 0;
	unsigned char reserved[7];
	unsigned int hash[4];
	int priority = 0;
	char userId[64];

	int featureLen = 0;
	float featureData[6000];

	unsigned char addonReserved[1024];
} UserItem;

void readDatabaseHeaderInfo(FILE* file) {
	long oldLoc = ftell(file);
	long loc = 0;

	fseek(file, 0, SEEK_SET);
	loc = ftell(file);
	if(loc != 0) {
		printf("%sCould not move file cursor to 0, but: %ld%s\n", RED_BOLD, loc, RESET);
		return;
	}

	char buffer[DB_HEAD_SIZE];
	size_t expectLen = sizeof(buffer);
	size_t readLen = fread(buffer, 1, sizeof(buffer), file);
	if(readLen != expectLen) {
		printf("%sRead head info failed! expect length: %zu, but actual: %zu%s\n",
			RED_BOLD, expectLen, readLen, RESET);

		fseek(file, oldLoc, SEEK_SET);
		return;
	}

	int version = 0;
	memcpy(&version, buffer+8, sizeof(int));

	int count = 0, countRepeat = 0;
	memcpy(&count, buffer+12, sizeof(int));
	memcpy(&countRepeat, buffer+16, sizeof(int));

	printf(CYAN);
	printf("Database head information:\n");
	printf("  Header Sign : %d(%c) %d(%c) %d(%c) %d(%c) %d %d %d %d\n",
		buffer[0],buffer[0],buffer[1],buffer[1],buffer[2],buffer[2],buffer[3],buffer[3],
		buffer[4],buffer[5],buffer[6],buffer[7]);
	printf("  Version     : %d\n", version);

	if(count != countRepeat)
		printf(RED);
	printf("  Item Count  : %d %s %d\n", count, count == countRepeat ? "==" : "!=", countRepeat);
	if(count != countRepeat)
		printf(CYAN);

	printf(RESET);


	fseek(file, oldLoc, SEEK_SET);
}

void readUserItemByCommand(FILE* file, const char* parameters) {
	long oldLoc = ftell(file);
	int pLen = strlen(parameters);
	if(pLen > 0) {
		uint itemIndex = -1;
		if(sscanf(parameters, "%u", &itemIndex) != 1)
			return;
		if(itemIndex <= 0)
			return;

		long newLoc = DB_ITEM_SIZE;
		newLoc *= itemIndex - 1;
		newLoc += DB_HEAD_SIZE;
		fseek(file, newLoc, SEEK_SET);

		long actualLoc = ftell(file);
		if(actualLoc != newLoc) {
			printf("%sCould not read user item %u, expect offset: %ld, but actual: %ld.%s\n",
				RED_BOLD, itemIndex, newLoc, actualLoc, RESET);
			return;
		}
	}

	char itemBytes[DB_ITEM_SIZE];
	size_t readLen = fread(itemBytes, 1, sizeof(itemBytes), file);
	if(readLen != sizeof(itemBytes)) {
		printf("%sCould not read user item, expect size: %zu, actual size: %zu.%s\n",
			RED_BOLD, sizeof(itemBytes), readLen, RESET);

		fseek(file, oldLoc, SEEK_SET);
		return;
	}

	UserItem item;
	memcpy(&item, &itemBytes, sizeof(UserItem));

	if(item.live != 0) {
		printf(CYAN);
		printf("Living Item:\n");
	} else {
		printf(YELLOW);
		printf("Died Item:\n");
	}

	printf("  ItemIndex  : %u\n", item.itemIndex);
	printf("  Hash       : %d %d %d %d\n", item.hash[0], item.hash[1], item.hash[2], item.hash[3]);
	printf("  Priority   : %d\n", item.priority);
	printf("  UserId     : \"%s\"\n", item.userId);
	printf("  FeatureLen : %d\n", item.featureLen);
	printf(RESET);

	fseek(file, oldLoc, SEEK_SET);
}
