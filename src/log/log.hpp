#include "./methods.hpp"

#ifndef LOG_HPP
#define LOG_HPP 1

//#define NDEBUG 1
#ifdef RELEASE
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

/// @todo replace LOG_XX2 to LOG_XX_F
/// @deprecated
#define LOG_INFO2(pf, ctx) Log_info2(pf, ctx)
/// @todo replace LOG_XX2 to LOG_XX_F
/// @deprecated
#define LOG_WARN2(pf, ctx) Log_warn2(pf, ctx)
/// @todo replace LOG_XX2 to LOG_XX_F
/// @deprecated
#define LOG_FATAL2(pf, ctx) Log_fatal2(pf, ctx)

/// Format log methods
#define __LOG_F(_buf, _max, ...) char _buf[_max]; snprintf(_buf, _max, __VA_ARGS__);
#define LOG_FORMAT_INFO(_buf, _max, ...) __LOG_F(_buf, _max, __VA_ARGS__) LOG_INFO(_buf);
#define LOG_FORMAT_WARN(_buf, _max, ...) __LOG_F(_buf, _max, __VA_ARGS__) LOG_WARN(_buf);
#define LOG_FORMAT_FATAL(_buf, _max, ...) __LOG_F(_buf, _max, __VA_ARGS__) LOG_FATAL(_buf);

#define LOG_INFO_F(...) LOG_FORMAT_INFO(__info_buffer, 256, __VA_ARGS__)
#define LOG_WARN_F(...) LOG_FORMAT_WARN(__warn_buffer, 256, __VA_ARGS__)
#define LOG_FATAL_F(...) LOG_FORMAT_FATAL(__fatal_buffer, 256, __VA_ARGS__)

#ifdef RELEASE
	#define LOG_DEBUG_F(...)
#else
	#define LOG_DEBUG_F(...) __LOG_F(__debug_buffer, 256, __VA_ARGS__); LOG_DEBUG(__debug_buffer);
#endif

#endif // LOG_HPP
