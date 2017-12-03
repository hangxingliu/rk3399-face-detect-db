#ifndef CONFIG_CONSTANT_HPP
#define CONFIG_CONSTANT_HPP 1

const int LOG_TYPE_NONE = 0;
const int LOG_TYPE_STDOUT = 1;
const int LOG_TYPE_FILE = 2;
const int LOG_TYPE_ALL = LOG_TYPE_STDOUT | LOG_TYPE_FILE;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GlobalConfigReadOnly {

	int eachFaceItemSizeInDB = 0;

} GlobalConfigReadOnly;

typedef struct GlobalConfig {

	#define MAX_LENGTH_OF_WORKSPACE_PATH 256
	/**
	 * **REQUIRED** the workspace path for database environment.
	 * Max length is 256
	 */
	char* workspacePath;

	/**
	 * Recommend memory size for database
	 * @default 0 (automatic)
	 * @todo
	 */
	int recommendMemorySize = 0;
	/**
	 * Init database file size(minimum file size)
	 * @default 4 * 1024 * 1024 (4MB)
	 */
	long initDatabaseFileSize = 4 * 1024 * 1024l;

	/**
	 * Max database file size
	 * @default 3 * 1024 * 1024 * 1024 (3GB)
	 */
	long maxDatabaseFileSize = 3 * 1024 * 1024 * 1024l;

	/**
	 * Read only configurations
	 * **You can not set this value unless modify sources!**
	 */
	GlobalConfigReadOnly readOnly;

} GlobalConfig;



#ifdef __cplusplus
}
#endif

#endif // CONFIG_CONSTANT_HPP
