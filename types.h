
#ifndef TYPES_H
#define TYPES_H

// Unsigned Integer Types

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Signed Integer Types

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

// Size and Pointer Types

typedef uint32_t size_t;
typedef uint32_t uintptr_t;

// Boolean Type

typedef enum
{
    BOOL_FALSE = 0,
    BOOL_TRUE = 1
} bool_t;

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif