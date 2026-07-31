#ifndef __DYNAMIC_HEAP_ALLOCATOR_H__
#define __DYNAMIC_HEAP_ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "../types.h"
#include "../utils.h"
#include "./interface.h"

ALLOCATOR_ATTRIBUTE Allocator dynamic_heap_allocator_init();


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__DYNAMIC_HEAP_ALLOCATOR_H__

// #define DYNAMIC_HEAP_ALLOCATOR_IMPLEMENTATIONS
#ifdef DYNAMIC_HEAP_ALLOCATOR_IMPLEMENTATIONS
#ifndef __DYNAMIC_HEAP_ALLOCATOR_IMP__
#define __DYNAMIC_HEAP_ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include <stdlib.h>

static void* dynamic_heap_allocate(Allocator* a, usize size, Alignment alignment) {
  (void)a;
  usize aligned = allign_resolve(size, alignment);

  void* ret = malloc(aligned);
  if(ret == NULL) a->metrics.failed++;
  else {
    a->metrics.allocated += aligned;
    a->metrics.allocations++;
    if(a->metrics.peak < a->metrics.allocated)
      a->metrics.peak = a->metrics.allocated;
  }
  return ret;
}

static void* dynamic_reallocate(Allocator* a, void* ptr, usize old_size, usize new_size, Alignment alignment) {
  (void)a;
  (void)old_size;
  usize aligned = allign_resolve(new_size, alignment);
  void* ret = realloc(ptr, aligned);
  if(ret == NULL) a->metrics.failed++;
  else {
    a->metrics.allocated -= old_size;
    a->metrics.allocated += aligned;
    if(a->metrics.peak < a->metrics.allocated)
      a->metrics.peak = a->metrics.allocated;
  }
  return ret;
}

static void dynamic_deallocate(Allocator* a, void* ptr, usize size, Alignment alignment) {
  (void)a;
  (void)size;
  (void)alignment;
  free(ptr);
  {
    a->metrics.allocated -= size;
    a->metrics.deallocations++;
  }
  return;
}

ALLOCATOR_ATTRIBUTE Allocator dynamic_heap_allocator_init() {
  return (Allocator){
    .heap = NULL,
    .allocate =  dynamic_heap_allocate,
    .reallocate = dynamic_reallocate,
    .deallocate =  dynamic_deallocate,

    // TODO: implement
    .owns = allocator_mock_owns,
    .reset = allocator_mock_reset,
    .deinit = allocator_mock_deinit,
  };
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__DYNAMIC_HEAP_ALLOCATOR_IMP__
#undef DYNAMIC_HEAP_ALLOCATOR_IMPLEMENTATIONS
#endif//DYNAMIC_HEAP_ALLOCATOR_IMPLEMENTATIONS

