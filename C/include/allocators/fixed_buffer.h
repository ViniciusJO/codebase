#ifndef __FIXED_BUFFER_ALLOCATOR_H__
#define __FIXED_BUFFER_ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "../types.h"
#include "../utils.h"
#include "./interface.h"

ALLOCATOR_ATTRIBUTE Allocator buffer_allocator_init(void* buffer, usize capacity);


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FIXED_BUFFER_ALLOCATOR_H__

// #define FIXED_BUFFER_ALLOCATOR_IMPLEMENTATIONS
#ifdef FIXED_BUFFER_ALLOCATOR_IMPLEMENTATIONS
#ifndef __FIXED_BUFFER_ALLOCATOR_IMP__
#define __FIXED_BUFFER_ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

static void* buffer_allocate(Allocator* a, usize size, Alignment alignment) {
  usize aligned = allign_resolve(size, alignment);
  if(a->buff.capacity - a->buff.count < aligned) return NULL;
  void *ret = (char*)a->buff.data + a->buff.count;
  a->buff.count += aligned - 1;
  if(ret == NULL) a->metrics.failed++;
  else {
    a->metrics.allocated += aligned;
    a->metrics.allocations++;
    if(a->metrics.peak < a->metrics.allocated)
      a->metrics.peak = a->metrics.allocated;
  }
  return ret;
}

static void buffer_deallocate(Allocator* a, void* ptr, usize size, Alignment alignment) {
  usize aligned = allign_resolve(size, alignment);
  if((char*)ptr + aligned == (char*)a->buff.data + a->buff.count)
    a->buff.count -= aligned;
  {
    a->metrics.allocated -= aligned;
    a->metrics.deallocations++;
  }
}

ALLOCATOR_ATTRIBUTE Allocator buffer_allocator_init(void* buffer, usize capacity) {
  return (Allocator){
    .buff =  (Buffer_Allocator_Context){
      .count = 0,
      .capacity = capacity,
      .data = buffer,
    },
    .allocate =  buffer_allocate,
    // TODO: implement
    .reallocate = allocator_mock_reallocate_align,
    .deallocate =  buffer_deallocate,

    // TODO: implement
    .owns = allocator_mock_owns,
    .reset = allocator_mock_reset,
    .deinit = allocator_mock_deinit,
  };
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FIXED_BUFFER_ALLOCATOR_IMP__
#undef FIXED_BUFFER_ALLOCATOR_IMPLEMENTATIONS
#endif//FIXED_BUFFER_ALLOCATOR_IMPLEMENTATIONS

