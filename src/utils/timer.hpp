#include<chrono>
#include "../log/log.hpp"

#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP

#ifdef RELEASE
	#define _timer_init(...) 0
	#define _timer_set(...) 0
	#define _timer_add(...) 0
	#define _timer_log(...) 0
#else
	#define _timer_init(v1, v2)\
		std::chrono::high_resolution_clock::time_point v1, v2;
	#define _timer_set(var) var = std::chrono::high_resolution_clock::now();
	#define _timer_calc(v2, v1) \
		std::chrono::duration_cast<std::chrono::microseconds>(v2 - v1).count();
	#define _timer_log(microsecs, forWhat)\
		LOG_DEBUG_F("~%.1lf ms elapsed for %s", (microsecs * 0.001), forWhat);
#endif

#endif // UTILS_TIMER_HPP
