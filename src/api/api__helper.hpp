#include<chrono>

#include "../log/log.hpp"
#include "../compile_options.hpp"

#include "../types/api_error_no.hpp"

#ifndef API__HELPER_HPP
#define API__HELPER_HPP

extern bool APITracker_hideGetFrameLog;
extern bool APITracker_hideApiInvoke;

#define __api_timer_now() std::chrono::high_resolution_clock::now()
typedef std::chrono::high_resolution_clock::time_point	__api_timer_t;
typedef std::chrono::microseconds						__api_timer_microsecs;

#if ANALYZE_API_INVOKE_TIME_CONSUMING
	class APITimer {
		__api_timer_t v1, v2;
		const char* apiName;
	public:
		APITimer(const char* apiName) {
			this->apiName = apiName;
			v1 = __api_timer_now();
		}
		void click(const char* forWhat) {
			v2 = __api_timer_now();
			double microsecs =
				std::chrono::duration_cast<__api_timer_microsecs>(v2 - v1).count();
			LOG_DEBUG_F("~%.1lf ms elapsed for %s", (microsecs * 0.001), forWhat);
			v1 = __api_timer_now();
		}
		void click() { click(apiName); }
	};
#else
	class APITimer {
	public:
		APITimer(const char* apiName) {}
		void click(const char* forWhat) {}
		void click() { }
	};
#endif
#undef __api_timer_now


#ifndef RELEASE
	#define LOG_API_INVOKE(name, parameterFormat, ...)\
		if(!APITracker_hideApiInvoke) {\
			char outputFormat[64];\
			sprintf(outputFormat, "Invoke API %s(%s)", name, parameterFormat);\
			LOG_DEBUG_F(outputFormat, __VA_ARGS__);\
		}
#else
	#define LOG_API_INVOKE(...)
#endif


#ifndef FOR_ARM
	#define THIS_IS_AN_AARCH64_ONLY_API return API_ONLY_FOR_AARCH64;
#else
	#define THIS_IS_AN_AARCH64_ONLY_API
#endif

#define CHECK_NON_NULL_FOR_API(ptr, ptrName)\
	if(!ptr) {\
		LOG_FATAL_F("%s is nullptr!", ptrName);\
		return API_EMPTY_POINTER;\
	}

#endif // API__HELPER_HPP
