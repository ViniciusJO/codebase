#ifndef __STDC_HOSTED__
#error Library, right now, only works on HOSTED enviroments
#endif

//#define STD_IMPLEMENTATIONS
#ifdef STD_IMPLEMENTATIONS
#ifndef __STD_IMP__
#define __STD_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define ANSI_IMPLEMENTATIONS
#define FLOAT_IMPLEMENTATIONS
#define MATH_IMPLEMENTATIONS

#define ALLOCATOR_IMPLEMENTATIONS

#define MAP_IMPLEMENTATIONS

#define GRAPH_IMPLEMENTATIONS

#define STR_IMPLEMENTATION

#define AP_IMPLEMENTATIONS

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__STD_IMP__
#undef STD_IMPLEMENTATIONS
#endif//STD_IMPLEMENTATIONS

#include "./ansi.h"
#include "./utils.h"
#include "./float.h"
#include "./math.h"

#include "./allocator.h"

#include "./al.h"
#include "./map.h"

#include "./types.h"

#include "./defer.h"

#include "./graph.h"


// TODO: clear warnings
// #include "./str.h"

// TODO: clear warnings
// #include "./ap.h"

// TODO: clear warnings
#include "./vector.h"


