#ifndef __ALLOCATOR_INTERFACE_H__
#define __ALLOCATOR_INTERFACE_H__

#include "../types.h"
#include "../utils.h"


#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifndef ALLOCATOR_ATTRIBUTE
#define ALLOCATOR_ATTRIBUTE extern
#endif//ALLOCATOR_ATTRIBUTE

#include "contexts/arena_context.h"
#include "contexts/heap_context.h"
#include "contexts/buffer_context.h"

typedef enum Alignment { ALLIGN_1 = 1, ALLIGN_2 = 2, ALLIGN_4 = 4, ALLIGN_8 = 8, ALLIGN_16 = 16, ALLIGN_32 = 32, ALLIGN_64 = 64, } Alignment;

typedef struct Allocator {
  union {
    Arena_Allocator_Context arena;
    Buffer_Allocator_Context buff;
    Heap_Allocation_Context heap;
  };

  struct {
    usize allocated;
    usize peak;
    usize allocations;
    usize deallocations;
    usize failed;
  } metrics;

  void* (*allocate)(struct Allocator* a, usize len, enum Alignment alignment);
  void* (*reallocate)(struct Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment);
  void  (*deallocate)(struct Allocator* a, void* data, usize len, enum Alignment alignment);
  bool (*owns)(struct Allocator *alloc, const void *ptr);
  void (*reset)(struct Allocator *alloc);
  void (*deinit)(struct Allocator *alloc);
} Allocator;

ALLOCATOR_ATTRIBUTE void* allocate(Allocator* a, usize len);
ALLOCATOR_ATTRIBUTE void* allocate_align(Allocator* a, usize len, enum Alignment alignment);

/// Attempt to expand or shrink memory, allowing relocation.
///
/// `len` must equal the length requested from the most recent
/// successful call to `allocate` or `reallocate`. `alignment` must
/// equal the same value that was passed as the `alignment` parameter to
/// the original `allocate` call.
///
/// A non-`NULL` return value indicates the resize was successful. The
/// allocation may have same address, or may have been relocated. In either
/// case, the allocation now has size of `new_len`. A `NULL` return value
/// indicates that the resize could not be done.
///
/// `new_len` must be greater than zero. It controlls the behavior of `remap`
/// following the rules:
///   - `new_len` = `len`: do nothing;
///   - `new_len` < `len`: truncate the memory region;
///   - `new_len` > `len`: grows the memory region. When the memory could not
///     be grown in place a new region is allocated and `len` bytes are copyed
///     from the old memory location to the first `len` bytes of the new memory
///     location;
ALLOCATOR_ATTRIBUTE void* reallocate(Allocator* a, void* data, usize len, usize new_len);
ALLOCATOR_ATTRIBUTE void* reallocate_align(Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment);

ALLOCATOR_ATTRIBUTE void deallocate(Allocator* a, void* data, usize len);
ALLOCATOR_ATTRIBUTE void deallocate_align(Allocator* a, void* data, usize len, enum Alignment alignment);

ALLOCATOR_ATTRIBUTE bool allocator_owns(Allocator* a, const void* ptr);

ALLOCATOR_ATTRIBUTE void allocator_reset(Allocator* a);

ALLOCATOR_ATTRIBUTE void allocator_deinit(Allocator* a);


ALLOCATOR_ATTRIBUTE void* allocator_mock_allocate(Allocator* a, usize len);
ALLOCATOR_ATTRIBUTE void* allocator_mock_allocate_align(Allocator* a, usize len, enum Alignment alignment);
ALLOCATOR_ATTRIBUTE void* allocator_mock_reallocate(Allocator* a, void* data, usize len, usize new_len);
ALLOCATOR_ATTRIBUTE void* allocator_mock_reallocate_align(Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment);
ALLOCATOR_ATTRIBUTE void  allocator_mock_deallocate(Allocator* a, void* data, usize len);
ALLOCATOR_ATTRIBUTE void  allocator_mock_deallocate_align(Allocator* a, void* data, usize len, enum Alignment alignment);
ALLOCATOR_ATTRIBUTE bool  allocator_mock_owns(Allocator* a, const void* ptr);
ALLOCATOR_ATTRIBUTE void  allocator_mock_reset(Allocator* a);
ALLOCATOR_ATTRIBUTE void  allocator_mock_deinit(Allocator* a);




#include <stdalign.h>
#include <stddef.h>

#define DEFAULT_ALIGN_RESOLVE(SIZE) (((int)(SIZE) + alignof(max_align_t) - 1) / alignof(max_align_t) * alignof(max_align_t))
#define ALIGN_RESOLVE(SIZE, ALIGN) (((int)(SIZE) + ALIGN - 1) / ALIGN * ALIGN)
static inline usize allign_resolve(usize len, enum Alignment alignment) {
  return (len + alignment - 1) / alignment * alignment;
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_INTERFACE_H__


// #define ALLOCATOR_INTERFACE_IMPLEMENTATIONS
#ifdef ALLOCATOR_INTERFACE_IMPLEMENTATIONS
#ifndef __ALLOCATOR_INTERFACE_IMP__
#define __ALLOCATOR_INTERFACE_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

// ALLOCATOR MOCK UNUSED
// #define __$_AMU(var, ...) (void)var; _$_AMU(__VA_ARGS__)
// #define _$_AMU(var, ...) (void)var; __$_AMU(__VA_ARGS__)

ALLOCATOR_ATTRIBUTE void* allocator_mock_allocate(Allocator* a, usize len) {
  UNUSED(a, len);
  return (void*)0;
}

ALLOCATOR_ATTRIBUTE void* allocator_mock_allocate_align(Allocator* a, usize len, enum Alignment alignment) {
  UNUSED(a, len, alignment);
  return (void*)0;
}

ALLOCATOR_ATTRIBUTE void* allocator_mock_reallocate(Allocator* a, void* data, usize len, usize new_len) {
  UNUSED(a, data, len, new_len);
  return (void*)0;
} 

ALLOCATOR_ATTRIBUTE void* allocator_mock_reallocate_align(Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment) {
  UNUSED(a, data, len, new_len, alignment);
  return (void*)0;
} 

ALLOCATOR_ATTRIBUTE void  allocator_mock_deallocate(Allocator* a, void* data, usize len) {
  UNUSED(a, data, len);
  return;
} 

 
ALLOCATOR_ATTRIBUTE void  allocator_mock_deallocate_align(Allocator* a, void* data, usize len, enum Alignment alignment) {
  UNUSED(a, data, len, alignment);
  return;
} 


ALLOCATOR_ATTRIBUTE bool  allocator_mock_owns(Allocator* a, const void* ptr) {
  UNUSED(a, ptr);
  return false;
} 


ALLOCATOR_ATTRIBUTE void  allocator_mock_reset(Allocator* a) {
  UNUSED(a);
  return;
} 


ALLOCATOR_ATTRIBUTE void  allocator_mock_deinit(Allocator* a) {
  UNUSED(a);
  return;
} 


// ========================= API =========================


#include <stddef.h>
#include <stdalign.h>

#ifdef __STDC__
#define ALIGNMENT alignof(max_align_t)
#else
#define ALIGNMENT ALLIGN_1
#endif


ALLOCATOR_ATTRIBUTE void* allocate(Allocator *a, size_t size) {
  return a->allocate(a, size, ALIGNMENT);
}

ALLOCATOR_ATTRIBUTE void* allocate_align(Allocator *a, size_t size, enum Alignment alignment) {
  return a->allocate(a, size, alignment);
}

ALLOCATOR_ATTRIBUTE void* reallocate(Allocator *a, void* ptr, size_t old_size, size_t new_size) {
  return a->reallocate(a, ptr, old_size, new_size, ALIGNMENT);
}

ALLOCATOR_ATTRIBUTE void* reallocate_align(Allocator *a, void* ptr, size_t old_size, size_t new_size, enum Alignment alignment) {
  return a->reallocate(a, ptr, old_size, new_size, alignment);
}

ALLOCATOR_ATTRIBUTE void deallocate(Allocator *a, void* ptr, size_t size) {
  a->deallocate(a, ptr, size, ALIGNMENT);
}

ALLOCATOR_ATTRIBUTE void deallocate_align(Allocator *a, void* ptr, size_t size, enum Alignment alignment) {
  a->deallocate(a, ptr, size, alignment);
}

ALLOCATOR_ATTRIBUTE bool allocator_owns(Allocator *a, const void *ptr) {
  return a->owns(a, ptr);
}
ALLOCATOR_ATTRIBUTE void allocator_reset(Allocator *a) {
  a->reset(a);
}

ALLOCATOR_ATTRIBUTE void allocator_deinit(Allocator* a) {
  a->deinit(a);
}

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_INTERFACE_IMP__
#undef ALLOCATOR_INTERFACE_IMPLEMENTATIONS
#endif//ALLOCATOR_INTERFACE_IMPLEMENTATIONS

