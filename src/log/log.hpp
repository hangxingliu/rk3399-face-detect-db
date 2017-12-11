#include "./methods.hpp"

#ifndef LOG_HPP
#define LOG_HPP 1

//#define NDEBUG 1
#ifdef NDEBUG
	//Release
	#define DUMP_INT_ARRAY(arr, len, maxLen, name) 0
	#define DUMP_BYTE_ARRAY(arr, len, maxLen, name) 0
	#define DUMP_CHAR_ARRAY(arr, len, maxLen, name) 0
	#define DUMP_INT(value, name) 0
	#define DUMP_FLOAT(value, name) 0
	#define DUMP_STRING(str, name) 0
	#define LOG_DEBUG(ctx) 0
	#define LOG_DEBUG2(prefix, ctx) 0
#else
	#define DUMP_INT_ARRAY(arr, len, maxLen, name) Log_dumpIntArray(arr, len, maxLen, name)
	#define DUMP_BYTE_ARRAY(arr, len, maxLen, name) Log_dumpByteArray(arr, len, maxLen, name)
	#define DUMP_CHAR_ARRAY(arr, len, maxLen, name) Log_dumpFloatArray(arr, len, maxLen, name)
	#define DUMP_INT(value, name) Log_dumpInt(value, name)
	#define DUMP_FLOAT(value, name) Log_dumpFloat(value, name)
	#define DUMP_STRING(str, name) Log_dumpString(str, name)
	#define LOG_DEBUG(ctx) Log_debug(ctx)
	#define LOG_DEBUG2(pf, ctx) Log_debug2(pf, ctx)
#endif

#define LOG_INFO(ctx) Log_info(ctx)
#define LOG_WARN(ctx) Log_warn(ctx)
#define LOG_FATAL(ctx) Log_fatal(ctx)
#define LOG_ERRNO() Log_fatalErrno()

#define LOG_INFO2(pf, ctx) Log_info2(pf, ctx)
#define LOG_WARN2(pf, ctx) Log_warn2(pf, ctx)
#define LOG_FATAL2(pf, ctx) Log_fatal2(pf, ctx)

#endif // LOG_HPP
