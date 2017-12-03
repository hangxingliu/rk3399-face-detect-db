#ifndef PUBLIC_TYPES_BASE_HPP
#define PUBLIC_TYPES_BASE_HPP 1

#define LOOP(var,from,to,step) for(int var = from ; var < to ; var += step)
#define LOOP_TIMES(var, times) LOOP(var, 0, times, 1)

typedef unsigned char uchar;
typedef unsigned char ubyte;
typedef ubyte* ubytes;
typedef ubyte* ubyteArray;

#endif
