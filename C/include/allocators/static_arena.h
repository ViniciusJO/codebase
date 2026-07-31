#ifndef __STATIC_ARENA_ALLOCATOR_H__
#define __STATIC_ARENA_ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "../types.h"
#include "../utils.h"
#include "./interface.h"

ALLOCATOR_ATTRIBUTE Allocator static_arena_allocator_init(Allocator* parent_allocator, usize initial_capacity);

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__STATIC_ARENA_ALLOCATOR_H__

// #define STATIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#ifdef STATIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#ifndef __STATIC_ARENA_ALLOCATOR_IMP__
#define __STATIC_ARENA_ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


static void* static_arena_allocate(Allocator* a, usize size, Alignment alignment) {
  usize aligned = allign_resolve(size, alignment);
  if(
      a->arena.static_arena.start == NULL ||
      (char*)a->arena.static_arena.end - (char*)a->arena.static_arena.current < (unsigned int)aligned
  ) return NULL;
  void* ret = a->arena.static_arena.current;
  a->arena.static_arena.current = (char*)a->arena.static_arena.current + aligned;
  if(ret == NULL) a->metrics.failed++;
  else {
    a->metrics.allocated += aligned;
    a->metrics.allocations++;
    if(a->metrics.peak < a->metrics.allocated)
      a->metrics.peak = a->metrics.allocated;
  }
  return ret;
}

static bool static_arena_owns(Allocator* a, const void* ptr) {
  return \
    a->arena.static_arena.start <= ptr &&
    a->arena.static_arena.end >= ptr ;
}

static void static_arena_reset(Allocator* a) {
  a->arena.static_arena.current = a->arena.static_arena.start;
  {
    a->metrics.allocated += 0;
    a->metrics.peak += 0;
    a->metrics.allocations = 0;
    a->metrics.deallocations = 0;
    a->metrics.failed = 0;
  }
}

static void static_arena_deinit(Allocator* a) {
  a->arena.static_arena.current = NULL;
  a->arena.static_arena.end = NULL;
  deallocate(a->arena.child_allocator, a->arena.static_arena.start, a->arena.capacity);
  a->arena.static_arena.start = NULL;
  {
    a->metrics.allocated += 0;
    a->metrics.peak += 0;
    a->metrics.allocations = 0;
    a->metrics.deallocations = 0;
    a->metrics.failed = 0;
  }
}

#include <stdalign.h>

ALLOCATOR_ATTRIBUTE Allocator static_arena_allocator_init(Allocator* parent_allocator, usize capacity) {

  void* data =  allocate(parent_allocator, DEFAULT_ALIGN_RESOLVE(capacity));

  return (Allocator){
    .arena = (Arena_Allocator_Context){
      .child_allocator = parent_allocator,
      .static_arena = {
        .start = data,
        .current = data,
        .end = (char*)data + capacity,
      },
      .capacity = capacity,
    },
    .allocate =  static_arena_allocate,
    .reallocate = allocator_mock_reallocate_align,
    .deallocate =  allocator_mock_deallocate_align,
    .owns = static_arena_owns,
    .reset = static_arena_reset,
    .deinit = static_arena_deinit,
  };
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__STATIC_ARENA_ALLOCATOR_IMP__
#undef STATIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#endif//STATIC_ARENA_ALLOCATOR_IMPLEMENTATIONS

