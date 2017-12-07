#include "./global.hpp"
#include "../types/api_error_no.hpp"

/** Global configurations instance */
GlobalConfig Config;

/** 0: Config has not init, 1: initialized.  @see initGlobalConfig() */
int configHasInitialized = 0;

int Config_initGlobalConfig(GlobalConfig* mergeInto) {
	int status = Config_mergeConfig(&Config, mergeInto);
	if(status != 0)
		return status;

	return 0;
}

static bool Config_copyString(const char* from, char* to, int max) {
	int i = 0;
	while(*from) {
		if(i++ >= max)
			return false;
		*to++ = *from++;
	}
	*to = 0;
	return true;
}

int Config_mergeConfig(GlobalConfig* mergeTo, GlobalConfig* overwrite) {
	if(!mergeTo || !overwrite)
		return 0;

	if(overwrite->workspacePath)
		if(!Config_copyString(overwrite->workspacePath, mergeTo->workspacePath, MAX_LENGTH_OF_CONFIG_PATH))
			return API_WORKSPACE_TOO_LONG;

	if(overwrite->resourcesPath)
		if(!Config_copyString(overwrite->resourcesPath, mergeTo->resourcesPath, MAX_LENGTH_OF_CONFIG_PATH))
			return API_RESOURCES_TOO_LONG;

	if(overwrite->initDatabaseFileSize)
		mergeTo->initDatabaseFileSize = overwrite->initDatabaseFileSize;

	if(overwrite->maxDatabaseFileSize)
		mergeTo->maxDatabaseFileSize = overwrite->maxDatabaseFileSize;

	return 0;
}

