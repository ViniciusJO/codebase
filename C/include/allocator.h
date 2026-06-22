#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef enum Alignment {
  ALLIGN_1 = 1,
  ALLIGN_2 = 2,
  ALLIGN_4 = 4,
  ALLIGN_8 = 8,
  ALLIGN_16 = 16,
  ALLIGN_32 = 32,
  ALLIGN_64 = 64,
} Allignment;

typedef __SIZE_TYPE__ size_t;

typedef struct Allocator {
    /// Return a pointer to `len` bytes with specified `alignment`, or return
    /// `null` indicating the allocation failed.
    ///
    /// `ret_addr` is optionally provided as the first return address of the
    /// allocation call stack. If the value is `0` it means no return address
    /// has been provided.
    void* (*alloc)(size_t len, enum Alignment alignment);

    /// Attempt to expand or shrink `memory` in place.
    ///
    /// `len` must equal the length requested from the most recent
    /// successful call to `alloc`, `resize`, or `remap`. `alignment` must
    /// equal the same value that was passed as the `alignment` parameter to
    /// the original `alloc` call.
    ///
    /// A result of `true` indicates the resize was successful and the
    /// allocation now has the same address but a size of `new_len`. `false`
    /// indicates the resize could not be completed without moving the
    /// allocation to a different address.
    ///
    /// `new_len` must be greater than zero.
    ///
    bool (*resize)(void* const memory, size_t len, size_t new_len, enum Alignment alignment);

    /// Attempt to expand or shrink memory, allowing relocation.
    ///
    /// `len` must equal the length requested from the most recent
    /// successful call to `alloc`, `resize`, or `remap`. `alignment` must
    /// equal the same value that was passed as the `alignment` parameter to
    /// the original `alloc` call.
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
    ///
    void* (*remap)(void* memory, size_t len, size_t new_len, enum Alignment alignment);

    /// Free and invalidate the `memory` region.
    ///
    /// `len` must equal the length requested from the most recent
    /// successful call to `alloc`, `resize`, or `remap`. `alignment` must
    /// equal the same value that was passed as the `alignment` parameter to
    /// the original `alloc` call.
    ///
    void (*free)(void* memory, size_t len, enum Alignment alignment);
} Allocator;


Allocator allocator_create_heap(Allocator back);
Allocator allocator_create_arena(Allocator back);
Allocator allocator_create_buff_alloc(void* buff, size_t len);


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_H__

// #define ALLOCATOR_LIBC
#ifdef  ALLOCATOR_LIBC
#ifndef __ALLOCATOR_IMP__
#define __ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include <stdlib.h>

static void* __libc_alloc(size_t len, enum Alignment alignment) {
  (void)alignment;
  return calloc(1, len);
}

// TODO: fix to spec
static bool __libc_resize(void* const memory, size_t len, size_t new_len, enum Alignment alignment) {
  (void)alignment;
  (void)len;
  void* new_mem = realloc(memory, new_len);
  if(NULL == new_mem) return false;
  // memory = new_mem;
  return true;
}

// TODO: fix to spec
static void* __libc_remap(void* memory, size_t len, size_t new_len, enum Alignment alignment) {
  (void)alignment;
  (void)len;
  return realloc(memory, new_len);
}

static void __libc_free(void* memory, size_t len, enum Alignment alignment) {
  (void)len;
  (void)alignment;
  free(memory);
  memory = NULL;
}

static Allocator libc_allocator = (Allocator){
  .alloc  = __libc_alloc,
  .resize = __libc_resize,
  .remap  = __libc_remap,
  .free   = __libc_free,
};



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_IMP__
#undef  ALLOCATOR_LIBC
#endif//ALLOCATOR_LIBC

//#define ALLOCATOR_IMPLEMENTATIONS
#ifdef ALLOCATOR_IMPLEMENTATIONS
#ifndef __ALLOCATOR_IMP__
#define __ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_IMP__
#undef ALLOCATOR_IMPLEMENTATIONS
#endif//ALLOCATOR_IMPLEMENTATIONS

