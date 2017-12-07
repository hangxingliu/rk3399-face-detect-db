#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "../types/base.hpp"
#include "./methods.hpp"

#define SHORT_DATE_PREFIX 0

/// "mm-dd HH:MM:SS" 14
#define LOG_DATE_PREFIX_LEN 16

time_t Log_prefix_time; ///< @todo thread safe
char Log_prefix_str[LOG_DATE_PREFIX_LEN]; ///< @todo thread safe

char* Log_getLogDatePrefix() {
	time_t timestamp = time(NULL);
	if (timestamp == Log_prefix_time)
		return Log_prefix_str;

	tm* date = localtime(&timestamp);
	sprintf(Log_prefix_str,
	#if SHORT_DATE_PREFIX
		"%02d:%02d:%02d",
	#else
		"%02d-%02d %02d:%02d:%02d",
		1 + date->tm_mon, date->tm_mday,
	#endif
		date->tm_hour, date->tm_min, date->tm_sec);

	Log_prefix_time = timestamp;
	return Log_prefix_str;
}

static inline void Log__log(const char* type, const char* content) {
	printf("%s%s    %s\n", type, Log_getLogDatePrefix(), content);
}
static inline void Log__log2(const char* type, const char* prefix, const char* content) {
	printf("%s%s    %s%s\n", type, Log_getLogDatePrefix(), prefix, content);
}

const char* PREFIX_DEBUG  = "DEBUG    ";
const char* PREFIX_INFO   = "INFO     ";
const char* PREFIX_WARN   = "WARN!    ";
const char* PREFIX_FATAL  = "FATAL!!! ";

void Log_debug(const char* content) { Log__log(PREFIX_DEBUG, content); }
void Log_info(const char* content) { Log__log(PREFIX_INFO, content); }
void Log_warn(const char* content) { Log__log(PREFIX_WARN, content); }
void Log_fatal(const char* content) { Log__log(PREFIX_FATAL, content); }

void Log_debug2(const char* prefix, const char* content) { Log__log2(PREFIX_DEBUG, prefix, content); }
void Log_info2(const char* prefix, const char* content) { Log__log2(PREFIX_INFO, prefix, content); }
void Log_warn2(const char* prefix, const char* content) { Log__log2(PREFIX_WARN, prefix, content); }
void Log_fatal2(const char* prefix, const char* content) { Log__log2(PREFIX_FATAL, prefix, content); }

/** @param name  max length is **64** */
void Log_dumpInt(int value, const char* name) {
	if(name && strlen(name) >= 64)
		return Log_fatal("Log_dumpInt(...,name); strlen(name) >= 64");

	char buffer[128];
	name ? sprintf(buffer, "dump int \"%s\": %d", name, value)
		: sprintf(buffer, "dump int: %d", value);
	Log_debug(buffer);
}
/** @param name  max length is **64** */
void Log_dumpFloat(float value, const char* name) {
	if(name && strlen(name) >= 64)
		return Log_fatal("Log_dumpFloat(...,name); strlen(name) >= 64");

	char buffer[128];
	name ? sprintf(buffer, "dump float \"%s\": %.4f", name, value)
		: sprintf(buffer, "dump float: %.4f", value);
	Log_debug(buffer);
}

void Log_dumpString(const char* str, const char* name) {
	if(name && strlen(name) >= 64)
		return Log_fatal("Log_dumpString(...,name); strlen(name) >= 64");

	char* buffer = (char*) malloc(strlen(str) + 128);
	name ? sprintf(buffer, "dump string \"%s\": %s", name, str)
		: sprintf(buffer, "dump string: %s", str);

	Log_debug(buffer);
	free(buffer);
}

/**
 * @param maxLength   max dump length (you can set this parameter same as `length`)
 * @param name  max length is **64**
 */
void Log_dumpIntArray(int* array, int length, int maxLength, const char* name) {
	char buffer[256];
	char *p = buffer;
	if(name) {
		if(strlen(name) >= 64)
			return Log_fatal("Log_dumpIntArray(...,name); strlen(name) >= 64");
		sprintf(buffer, "dump array<int> \"%s\" with length: %d [", name, length);
	} else {
		sprintf(buffer, "dump array<int> with length: %d [", length);
	}
	Log_debug(buffer);

	if(maxLength <= 0) maxLength = length;

	LOOP_TIMES(i, length) {
		if(i >= maxLength) {
			// \t....
			*p = '\t'; p++; LOOP_TIMES(j, 4) { *p = '.'; p++; } *p = '\0';
			Log_debug(buffer);
			break;
		}
		sprintf(p, "\t%d,", array[i]);
		while(*p) p++;

		if( (i & 3) == 3 ) { // i % 4 == 3
			Log_debug(buffer);
			p=buffer;//reset append location
		}
	}

	Log_debug("]");
}

/**
 * @param maxLength   max dump length (you can set this parameter same as `length`)
 * @param name  max length is **64**
 */
void Log_dumpByteArray(char* array, int length, int maxLength, const char* name) {
	char buffer[256];
	char *p = buffer;
	if(name) {
		if(strlen(name) >= 64)
			return Log_fatal("Log_dumpByteArray(...,name); strlen(name) >= 64");
		sprintf(buffer, "dump array<byte> \"%s\" with length: %d [", name, length);
	} else {
		sprintf(buffer, "dump array<byte> with length: %d [", length);
	}
	Log_debug(buffer);

	if(maxLength <= 0) maxLength = length;

	LOOP_TIMES(i, length) {
		if(i >= maxLength) {
			// \t....
			*p = '\t'; p++; LOOP_TIMES(j, 4) { *p = '.'; p++; } *p = '\0';
			Log_debug(buffer);
			break;
		}
		char c = array[i];
		sprintf(p, "\t%3d (%02x \'%c\'),", c, c & 0xff, c >= 32 && c <= 127 ? c : '.');
		while(*p) p++;

		if( (i & 3) == 3 ) { // i % 4 == 3
			Log_debug(buffer);
			p=buffer;//reset append location
		}
	}

	Log_debug("]");
}

/**
 * @param maxLength   max dump length (you can set this parameter same as `length`)
 * @param name  max length is **64**
 */
void Log_dumpFloatArray(float* array, int length, int maxLength, const char* name) {
	char buffer[256];
	char *p = buffer;
	if(name) {
		if(strlen(name) >= 64)
			return Log_fatal("Log_dumpFloatArray(...,name); strlen(name) >= 64");
		sprintf(buffer, "dump array<float> \"%s\" with length: %d [", name, length);
	} else {
		sprintf(buffer, "dump array<float> with length: %d [", length);
	}
	Log_debug(buffer);

	if(maxLength <= 0) maxLength = length;

	LOOP_TIMES(i, length) {
		if(i >= maxLength) {
			// \t....
			*p = '\t'; p++; LOOP_TIMES(j, 4) { *p = '.'; p++; } *p = '\0';
			Log_debug(buffer);
			break;
		}
		sprintf(p, "\t%.4f,", array[i]);
		while(*p) p++;

		if( (i & 3) == 3 ) { // i % 4 == 3
			Log_debug(buffer);
			p=buffer;//reset append location
		}
	}

	Log_debug("]");
}



