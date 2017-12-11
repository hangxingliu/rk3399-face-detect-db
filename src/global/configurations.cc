#include <stdio.h>

#include "./global.hpp"
#include "../types/api_error_no.hpp"
#include "../types/base.hpp"

#define Config_copyString(from, to, max) snprintf(to, max, "%s", from)

/** Global configurations instance */
static GlobalConfig Config;

bool configHasInitialized = false;

GlobalConfig* getGlobalConfig() {
	if(!configHasInitialized)
		return nullptr;
	return &Config;
}

int Config_initGlobalConfig(GlobalConfig* mergeInto) {
	if(configHasInitialized)
		return API_INIT_CONFIG_AGAIN;
	int status = Config_mergeConfig(&Config, mergeInto);
	if(status == 0)
		configHasInitialized = true;
	return status;
}

int Config_mergeConfig(GlobalConfig* mergeTo, GlobalConfig* overwrite) {
	if(!mergeTo || !overwrite)
		return 0;

	if(overwrite->workspacePath)
		if(!Config_copyString(overwrite->workspacePath, mergeTo->workspacePath, MAX_LENGTH_OF_CONFIG_PATH))
			return API_WORKSPACE_TOO_LONG;

///  TODO: Could not change size now.
//	if(overwrite->initDatabaseFileSize)
//		mergeTo->initDatabaseFileSize = overwrite->initDatabaseFileSize;

//	if(overwrite->maxDatabaseFileSize)
//		mergeTo->maxDatabaseFileSize = overwrite->maxDatabaseFileSize;

	return 0;
}

