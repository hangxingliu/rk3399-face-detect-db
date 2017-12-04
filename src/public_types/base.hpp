#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<memory.h>

#include "../utils/utils.hpp"

#ifndef PUBLIC_TYPES_BASE_HPP
#define PUBLIC_TYPES_BASE_HPP 1

#define LOOP(var,from,to,step) for(int var = from ; var < to ; var += step)
#define LOOP_TIMES(var, times) LOOP(var, 0, times, 1)

#define CREATE_STR(strName, length) char* strName = (char*) malloc(length);
#define DELETE_STR(strName) free(strName)

#define CREATE_STR_FROM_INT(strName, value, withQuote) \
	CREATE_STR(strName, 16); int2String(value, strName, withQuote);

typedef unsigned char uchar;
typedef unsigned char ubyte;
typedef ubyte* ubytes;
typedef ubyte* ubyteArray;

#endif
