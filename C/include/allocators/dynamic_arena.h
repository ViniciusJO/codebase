#ifndef __DYNAMIC_ARENA_ALLOCATOR_H__
#define __DYNAMIC_ARENA_ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "../types.h"
#include "../utils.h"
#include "./interface.h"

ALLOCATOR_ATTRIBUTE Allocator dynamic_arena_allocator_init(Allocator* parent_allocator, usize initial_capacity);

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__DYNAMIC_ARENA_ALLOCATOR_H__

// #define DYNAMIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#ifdef DYNAMIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#ifndef __DYNAMIC_ARENA_ALLOCATOR_IMP__
#define __DYNAMIC_ARENA_ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

static Arena_Region* arena_region_create(Allocator* a, usize capacity) {
  Arena_Region* region = (Arena_Region*)allocate(a->arena.child_allocator, sizeof(Arena_Region));
  region->next = NULL;
  region->capacity = capacity;
  region->data = allocate(a->arena.child_allocator, capacity);
  region->next_alloc = region->data;
  region->count = 0;
  return region;
}

static void* arena_region_allocate(Allocator* a, Arena_Region* region, usize size) {
  if((region->capacity - region->count) < size) {
    if(region->next == NULL) {
      usize cap = (size > region->capacity) ? size : region->capacity;
      region->next = arena_region_create(a,cap);
    }
    return arena_region_allocate(a, region->next, size);
  }

  void* data = region->next_alloc;

  region->next_alloc = (char*)data + size;
  // TODO: VERIFY ------vvv
  region->count += size - 1;

  return data;
}

/*static*/ void arena_region_deallocate(Allocator* a, Arena_Region* region) {
  if(region->next) arena_region_deallocate(a, region->next);
  deallocate(a->arena.child_allocator, region->data, region->capacity);
  deallocate(a->arena.child_allocator, region, sizeof(Arena_Region));
}


static void* arena_allocate(Allocator* a, usize size, Alignment alignment) {
  usize aligned = allign_resolve(size, alignment);
  if(a->arena.region == NULL) a->arena.region = arena_region_create(a, MAX(aligned, a->arena.region_default_capacity));
  void* ret = arena_region_allocate(a, a->arena.region, aligned);
  if(ret == NULL) a->metrics.failed++;
  else {
    a->metrics.allocated += aligned;
    a->metrics.allocations++;
    if(a->metrics.peak < a->metrics.allocated)
      a->metrics.peak = a->metrics.allocated;
  }
  return ret;
}

// TODO: implement
static void arena_deallocate(Allocator* a, void* ptr, usize size, Alignment alignment) {
  (void)a;
  (void)ptr;
  (void)size;
  (void)alignment;
  return;
}

ALLOCATOR_ATTRIBUTE Allocator dynamic_arena_allocator_init(Allocator* parent_allocator, usize initial_capacity) {
  return (Allocator){
    .arena = (Arena_Allocator_Context){
      .child_allocator = parent_allocator,
      .region = NULL,
      .region_default_capacity = initial_capacity,
    },
    .allocate =  arena_allocate,
    // TODO: implement
    .reallocate = allocator_mock_reallocate_align,
    .deallocate =  arena_deallocate,

    // TODO: implement
    .owns = allocator_mock_owns,
    .reset = allocator_mock_reset,
    .deinit = allocator_mock_deinit,
  };
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__DYNAMIC_ARENA_ALLOCATOR_IMP__
#undef DYNAMIC_ARENA_ALLOCATOR_IMPLEMENTATIONS
#endif//DYNAMIC_ARENA_ALLOCATOR_IMPLEMENTATIONS

