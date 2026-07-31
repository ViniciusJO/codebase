#ifdef ALLOCATOR_IMPLEMENTATIONS
#define ALLOCATOR_INTERFACE_IMPLEMENTATIONS
#define DYNAMIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#define DYNAMIC_HEAP_ALLOCATOR_IMPLEMENTATIONS
#define FIXED_BUFFER_ALLOCATOR_IMPLEMENTATIONS
#define STATIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#endif

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "./allocators/interface.h"


// INFO: is in fact a stack allocator
#include "./allocators/fixed_buffer.h"

#include "./allocators/dynamic_arena.h"
#include "./allocators/static_arena.h"

#include "./allocators/dynamic_heap.h"


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_H__

