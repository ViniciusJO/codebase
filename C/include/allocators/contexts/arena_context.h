#ifndef __ARENA_CONTEXT_H__
#define __ARENA_CONTEXT_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "../../types.h"

typedef struct Arena_Region {
  struct Arena_Region* next;
  usize capacity;
  usize count;
  void* data;
  void* next_alloc;
} Arena_Region;

typedef struct {
  struct Allocator* child_allocator;
  union {
    Arena_Region* region;
    struct {
      void *start, *current, *end;
    } static_arena;
  };
  union {
    usize region_default_capacity;
    usize capacity;
  };
} Arena_Allocator_Context;


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ARENA_CONTEXT_H__
