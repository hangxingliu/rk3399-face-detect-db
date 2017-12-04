#include "./config.hpp"
#include "../public_types/config.hpp"

/** Global configurations instance */
GlobalConfig Config;

/** 0: Config has not init, 1: initialized.  @see initGlobalConfig() */
int configHasInitialized = 0;

int Config_initGlobalConfig() {
	return 0;
}

int Config_getDefaultGlobalConfig(GlobalConfig* config) {

	return 0;
}

int Config_cloneGlobalConfig(GlobalConfig* src, GlobalConfig* dest) {

	return 0;
}

