#include "./base.hpp"

#ifndef PUBLIC_TYPES_CONFIG_HPP
#define PUBLIC_TYPES_CONFIG_HPP 1

typedef struct GlobalConfigReadOnly {

} GlobalConfigReadOnly;

#define MAX_LENGTH_OF_CONFIG_PATH 255
typedef struct GlobalConfig {

	/**
	 * **REQUIRED** the workspace path for database environment.
	 * Max length is 255
	 */
	char workspacePath[MAX_LENGTH_OF_CONFIG_PATH + 1];

	/**
	 * Init database file size(minimum file size)
	 * default **4 * 1024 * 1024 (4MB)**
	 */
	long initDatabaseFileSize = 4 * 1024 * 1024l;

	/**
	 * Max database file size
	 * default **3 * 1024 * 1024 * 1024 (3GB)**
	 */
	long maxDatabaseFileSize = 3 * 1024 * 1024 * 1024l;


	/**
	 * Read only configurations
	 * **You can not set this value unless modify sources!**
	 */
	GlobalConfigReadOnly readOnly;

	GlobalConfig() {
		strcpy(workspacePath, "./workspace");
	}

	GlobalConfig(const char* _workspacePath) {
		snprintf(workspacePath, MAX_LENGTH_OF_CONFIG_PATH, "%s", _workspacePath);
	}

} GlobalConfig;

#endif // PUBLIC_TYPES_CONFIG_HPP
