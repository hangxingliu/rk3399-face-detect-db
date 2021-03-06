#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>

#include "../utils/utils.hpp"

#ifndef PUBLIC_TYPES_BASE_HPP
#define PUBLIC_TYPES_BASE_HPP 1

#define USERID_LENGTH 64
#define FACE_FEATURE_ARRAY_LENGTH 6000
#define FACE_FEATURE_SIZE (FACE_FEATURE_ARRAY_LENGTH * sizeof(float))

// =================================

#ifndef API_OUT
	#define API_OUT
#endif
#ifndef API_OUT_NON_NULL
	#define API_OUT_NON_NULL
#endif
#ifndef NON_NULL
	#define NON_NULL
#endif
#ifndef NEED_TO_FREE
	#define NEED_TO_FREE
#endif



#define LOOP(var,from,to,step) for(int var = from ; var < to ; var += step)
#define LOOP_TIMES(var, times) LOOP(var, 0, times, 1)

typedef unsigned char ubyte;
typedef ubyte* ubytes;
typedef ubyte* ubyteArray;
typedef uchar* ucharArray;
typedef unsigned int uint;

#endif
