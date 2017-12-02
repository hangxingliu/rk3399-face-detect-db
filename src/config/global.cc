#include "types.hpp"

/** Global configurations instance */
GlobalConfig Config;

/** 0: Config has not init, 1: inited.  @see initGlobalConfig() */
int configHasInitialized = 0;
