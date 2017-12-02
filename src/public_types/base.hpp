#ifndef PUBLIC_TYPES_BASE_HPP
#define PUBLIC_TYPES_BASE_HPP 1

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uchar;

// =================
//   Main  >>>>>>>

/**
 * AnyType struct as a placeholder in api function
 */
typedef struct AnyType {
	void* anyValue;
} AnyType;


#ifdef __cplusplus
}
#endif

#endif
