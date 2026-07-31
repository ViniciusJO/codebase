#ifndef __TYPES_H__
#define __TYPES_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

// NULL
#undef NULL
#ifdef __cplusplus
#if !defined(__MINGW32__) && !defined(_MSC_VER)
#define NULL __null
#else
#define NULL 0
#endif
#else
#define NULL ((void*)0)
#endif



#if defined __x86_64__ && !defined __ILP32__
# define __WORDSIZE	64
#else
#define __WORDSIZE	32
#define __WORDSIZE32_SIZE_ULONG		0
#define __WORDSIZE32_PTRDIFF_LONG	0
#endif

#define __WORDSIZE_TIME64_COMPAT32 1

#ifdef __x86_64__
/* Both x86-64 and x32 use the 64-bit system call interface.  */
# define __SYSCALL_WORDSIZE		64
#endif

typedef __SIZE_TYPE__ usize;
typedef __PTRDIFF_TYPE__ isize;

/* Fixed-size types, underlying types depend on word size and compiler.  */
typedef signed char i8;
typedef unsigned char u8;
typedef signed short int i16;
typedef unsigned short int u16;
typedef signed int i32;
typedef unsigned int u32;
#if __WORDSIZE == 64
typedef signed long int i64;
typedef unsigned long int u64;
#else
__extension__ typedef signed long long int i64;
__extension__ typedef unsigned long long int u64;
#endif


// #ifndef __cplusplus
// #if !defined bool
// enum bool { false, true };
// #endif
// #endif//__cplusplus


#include <stdbool.h>

// __extension__;

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__TYPES_H__
