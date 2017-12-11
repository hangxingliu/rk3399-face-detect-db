/// Encode object to bytes

// File format:
// Face {10} {44} {96} {150} {VERSION(4 bytes)...}  => 12 bytes
// {COUNT(4 bytes)...} => 4 bytes
// {COUNT_REPEAT(4 bytes)...} => 4 bytes
// ITEMS ...
// 0... 0... 0...

// Count: living item count

// Update CountRepeat before update userItem
// Update Count after update userItem
// Fix database if count wrong in opening database automatically

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "./storage.hpp"

#include "../types/base.hpp"
#include "../types/database.hpp"
#include "../types/api_error_no.hpp"

#include "../log/log.hpp"
#include "../utils/utils.hpp"
#include "../global/global.hpp"

#define print2str(...) snprintf(str, sizeof(str), __VA_ARGS__)

const unsigned char DB_FILE_HEAD[] = {
	'F', 'a', 'c', 'e',
	10, 44, 96, 150,
	1, 0, 0, 0, // version
	0, 0, 0, 0, // Count
	0, 0, 0, 0  // Count Repeat
};
const unsigned char* DB_getDefaultHead() { return DB_FILE_HEAD; }

const int DB_HEAD_FLAG_SIZE = 8;
const int DB_HEAD_VERSION_SIZE = 4;
const int DB_HEAD_SIZE = sizeof(DB_FILE_HEAD);

const static long DB__itemSizeL = DB_ITEM_SIZE;

// ============
// getter
int DB_getHeadSize() { return DB_HEAD_SIZE; }
int DB_getLivingCountOffset() { return DB_HEAD_FLAG_SIZE + DB_HEAD_VERSION_SIZE; }
int DB_getLivingCountRepeatOffset() {
	return DB_HEAD_FLAG_SIZE + DB_HEAD_VERSION_SIZE + sizeof(int); }
long DB_getItemOffset(uint itemIndex) {
	return DB_HEAD_SIZE + (itemIndex - 1) * DB__itemSizeL;}



const char* DB_FILE_NAME = "face.db";
const char* DB_RECOVER_DIR = ".recover";

static int DatabaseFd = -1;
static FILE* DatabaseFs = nullptr;

static char DatabaseFile[512];
static char DatabaseRecoverDir[512];

FILE* DB_getFileStream() { return DatabaseFs; }

/**
 * Create database file/directory
 * @param _path database file path
 * @param initializedSize
 * @return int 0: success
 */
int DB_createDatabaseFile(
	const char* _path,
	unsigned long initializedSize) {
	LOG_DEBUG2("createDatabaseFile: ", _path);

	char path[256];
	int pathLen = 0;
	snprintf(path, 256, "%s", _path);
	pathLen = strlen(path);
	for(int i = pathLen ; i >= 0 ; i -- ) {
		if(path[i] != '/') continue;

		path[i] = 0;
		if(!mkdirRecursively(path))
			return API_DB_MKDIRS_FAILED;
		LOG_DEBUG2("Create directory for database file: ", path);
		path[i] = '/';
		break;
	}

	FILE *file;
	file = fopen(path, "wb");
	if(!file)
		return API_DB_CREATE_FILE_FAILED;

	char buffer[4096];
	const int bufferSize = sizeof(buffer);
	size_t wroteAll = 0;
	size_t wrote = 0;

	while(wroteAll < initializedSize) {
		if(wroteAll == 0) {
			memset(buffer, 0, bufferSize);
			memcpy(buffer, DB_FILE_HEAD, sizeof(DB_FILE_HEAD));
		}
		wrote = fwrite(buffer, 1, bufferSize, file);
		if(wrote != bufferSize) {
			fclose(file);

			sprintf(buffer, "Init database failed: expect write block size: %d, but actual: %zu. before bytes: %zu",
				bufferSize, wrote, wroteAll);
			LOG_FATAL(buffer);
			return API_DB_IO_FAILED;
		}
		if(wroteAll == 0) { memset(buffer, 0, bufferSize); }
		wroteAll += bufferSize;
	}

	fflush(file);
	fclose(file);

	sprintf(buffer, "Initialize database file success, wrote %zu bytes.", wroteAll);
	LOG_DEBUG(buffer);

	return API_OK;
}


int DB_validateHead(const unsigned char* head, int* itemCountResult) {
	char str[256];

	// Check flag
	LOOP_TIMES(i, DB_HEAD_FLAG_SIZE) {
		if(head[i] != DB_FILE_HEAD[i]) {
			print2str("database head flag[%d], expect: %u, but actual: %u.",
				i, DB_FILE_HEAD[i], head[i]);
			LOG_FATAL(str);
			return API_DB_INVALID_HEAD;
		}
	}

	auto version = head + DB_HEAD_FLAG_SIZE;
	auto CORRECT_VERSION = DB_FILE_HEAD + DB_HEAD_FLAG_SIZE;
	// Check version
	LOOP_TIMES(i, DB_HEAD_VERSION_SIZE) {
		if(version[i] != CORRECT_VERSION[i]) {
			print2str("database version[%d], expect: %u, but actual: %u.",
				i, CORRECT_VERSION[i], version[i]);
			LOG_FATAL(str);
			return API_DB_INVALID_VERSION;
		}
	}

	// Check double item counts are same
	auto countBasePtr = head + DB_HEAD_FLAG_SIZE + DB_HEAD_VERSION_SIZE;
	int count1 = *countBasePtr;
	int count2 = *(countBasePtr + sizeof(int));
	if(count1 != count2) {
		print2str("database double item counts are not same: %d != %d", count1, count2);
		LOG_FATAL(str);
		return API_DB_INVALID_ITEM_COUNT;
	}
	*itemCountResult = count1;

	return 0;
}

int DB_loadDatabase(const char* path) {
	char str[256];
	int status = 0;

	DatabaseFd = open(path, O_RDWR);
	if(DatabaseFd < 0)
		return API_DB_OPEN_FD_FAILED;
	DatabaseFs = fdopen(DatabaseFd, "wb+");
	if(!DatabaseFs)
		return API_DB_OPEN_FS_FAILED;

	const unsigned int headLen = (unsigned int) sizeof(DB_FILE_HEAD);

	unsigned char fileHead[headLen];
	unsigned int readLen = 0;

	readLen = fread(fileHead, 1, headLen, DatabaseFs);
	if(readLen != headLen) {
		print2str("read database head, expect %u, but actual size: %u.", headLen, readLen);
		LOG_FATAL(str);
		return API_DB_HEAD_IS_SO_SHORT;
	}

	int itemCount = 0;
	status = DB_validateHead(fileHead, &itemCount);
	if(status != 0)
		return status;

	print2str("load database head success, item count: %d", itemCount);
	LOG_INFO(str);

	status = ItemReader_init(DatabaseFs, (uint) itemCount);
	if(status != 0)
		return status;

	return API_OK;
}

int DB_init() {
	char str[256];
	int status = 0;
	auto config = getGlobalConfig();
	if(!config)
		return API_HAS_NOT_INIT_CONFIG;

	joinPath(config->workspacePath, DB_FILE_NAME, DatabaseFile);
	joinPath(config->workspacePath, DB_RECOVER_DIR, DatabaseRecoverDir);


	if(!isPathAFile(DatabaseFile)) {
		print2str("Database file \"%s\" is not a file. creating now ...", DatabaseFile);
		LOG_INFO(str);
		status = DB_createDatabaseFile(config->workspacePath, config->initDatabaseFileSize);
		if( status != 0 )
			return status;
	}

	status = DB_loadDatabase(DatabaseFile);
	if(status != 0 )
		return status;



	return 0;
}

/// @private
static inline int DB__flushToDisk() {
	fflush(DatabaseFs);
	if(fsync(DatabaseFd) != 0) {
		LOG_FATAL("fsync(DatabaseFd)");
		LOG_ERRNO();
		return API_DB_FLUSH_FAILED;
	}
	return 0;
}
/// Check database file stream, then invoke DB__flushToDisk
int DB_flushToDisk() {
	if(DatabaseFd < 0 || !DatabaseFs)
		return LOG_DEBUG("Database file was not opened"), 0;
	return DB__flushToDisk();
}

int DB_close() {
	if(DatabaseFd < 0 || !DatabaseFs)
		return LOG_DEBUG("Database file was not opened"), 0;
	int status = DB__flushToDisk();

	if( fclose(DatabaseFs) != 0 )
		return API_DB_FCLOSE_FAILED;
	return status;
	// close file descriptor is unnecessary because fclose fd together.
	// close(DatabaseFd);
}
