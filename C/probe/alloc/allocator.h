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
#ifndef __ALLOCATOR_LIBC_IMP__
#define __ALLOCATOR_LIBC_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdlib.h>

static void* __libc_alloc(size_t len, enum Alignment alignment) {
  (void)alignment;
  return calloc(1, len);
}

/// libc has no in-place resize primitive; shrinking is declared a success
/// (the excess is simply unused until free), growing returns false since
/// we cannot guarantee the pointer stays stable.
static bool __libc_resize(void* const memory, size_t len, size_t new_len, enum Alignment alignment) {
  (void)memory;
  (void)alignment;
  return new_len <= len;
}

static void* __libc_remap(void* memory, size_t len, size_t new_len, enum Alignment alignment) {
  (void)alignment;
  (void)len;
  return realloc(memory, new_len);
}

static void __libc_free(void* memory, size_t len, enum Alignment alignment) {
  (void)len;
  (void)alignment;
  free(memory);
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
#endif//__ALLOCATOR_LIBC_IMP__
#undef  ALLOCATOR_LIBC
#endif//ALLOCATOR_LIBC

#define ALLOCATOR_IMPLEMENTATIONS
#ifdef ALLOCATOR_IMPLEMENTATIONS
#ifndef __ALLOCATOR_IMPL_IMP__
#define __ALLOCATOR_IMPL_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

extern void *memcpy(void *__restrict dest, const void *__restrict src, size_t n);

// ---------------------------------------------------------------------------
// Shared alignment helper
// ---------------------------------------------------------------------------

static inline char *__alloc_align_ptr(char *p, size_t alignment) {
    return (char *)(((size_t)p + alignment - 1) & ~(size_t)(alignment - 1));
}

// ---------------------------------------------------------------------------
// Buffer allocator  (bump allocator over a user-provided fixed buffer)
//
// State header is placed at the very beginning of the buffer; the usable
// region starts immediately after it.  One active instance is tracked per
// thread via __buff_state.
// ---------------------------------------------------------------------------

typedef struct {
    char *end;     // one past the last byte of the buffer
    char *cursor;  // current bump pointer
} __Buff_State;

static _Thread_local __Buff_State *__buff_state = (void*)0;

static void* __buff_alloc(size_t len, enum Alignment alignment) {
    if (!__buff_state) return (void*)0;
    char *aligned = __alloc_align_ptr(__buff_state->cursor, alignment);
    if (aligned + len > __buff_state->end) return (void*)0;
    __buff_state->cursor = aligned + len;
    return aligned;
}

static bool __buff_resize(void* const memory, size_t len, size_t new_len, enum Alignment alignment) {
    (void)alignment;
    if (!__buff_state) return false;
    // only the most recent allocation can be resized in place
    if ((char*)memory + len != __buff_state->cursor) return false;
    if ((char*)memory + new_len > __buff_state->end) return false;
    __buff_state->cursor = (char*)memory + new_len;
    return true;
}

static void* __buff_remap(void* memory, size_t len, size_t new_len, enum Alignment alignment) {
    if (new_len == len) return memory;
    if (__buff_resize(memory, len, new_len, alignment)) return memory;
    // cannot resize in place — allocate a new region and copy
    void *dest = __buff_alloc(new_len, alignment);
    if (!dest) return (void*)0;
    memcpy(dest, memory, len < new_len ? len : new_len);
    return dest;
}

static void __buff_free(void* memory, size_t len, enum Alignment alignment) {
    (void)alignment;
    if (!__buff_state) return;
    // LIFO free: rewind the cursor when freeing the last allocation
    if ((char*)memory + len == __buff_state->cursor)
        __buff_state->cursor = (char*)memory;
}

/// Create a bump allocator backed by the caller-provided buffer.
///
/// The first `sizeof(__Buff_State)` bytes of `buff` are reserved for internal
/// state; the remaining bytes form the usable allocation region.  The returned
/// allocator is only valid while `buff` is alive.
///
/// Only one buff allocator may be active per thread at a time.
Allocator allocator_create_buff_alloc(void *buff, size_t len) {
    if (!buff || len <= sizeof(__Buff_State)) return (Allocator){0};
    __Buff_State *state = (__Buff_State *)buff;
    state->end    = (char *)buff + len;
    state->cursor = (char *)buff + sizeof(__Buff_State);
    __buff_state = state;
    return (Allocator){
        .alloc  = __buff_alloc,
        .resize = __buff_resize,
        .remap  = __buff_remap,
        .free   = __buff_free,
    };
}

// ---------------------------------------------------------------------------
// Arena allocator  (slab-based bump allocator backed by another allocator)
//
// New slabs are requested from `back` when the current slab is exhausted.
// Individual frees are no-ops; memory is reclaimed only when the backing
// allocator frees all slabs.  One active instance per thread.
// ---------------------------------------------------------------------------

#ifndef __ARENA_DEFAULT_SLAB
#define __ARENA_DEFAULT_SLAB (64 * 1024)
#endif//__ARENA_DEFAULT_SLAB

typedef struct __Arena_Slab {
    struct __Arena_Slab *next;
    size_t capacity; // usable bytes: (allocated block size) - sizeof(__Arena_Slab)
    size_t used;
    // usable data follows at (char *)(slab + 1)
} __Arena_Slab;

typedef struct {
    Allocator back;
    __Arena_Slab *current;
} __Arena_State;

static _Thread_local __Arena_State *__arena_state = (void*)0;

static __Arena_Slab *__arena_new_slab(size_t min_capacity) {
    size_t capacity = __ARENA_DEFAULT_SLAB;
    if (capacity < min_capacity) capacity = min_capacity;
    size_t total = sizeof(__Arena_Slab) + capacity;
    __Arena_Slab *slab = (__Arena_Slab *)__arena_state->back.alloc(total, ALLIGN_8);
    if (!slab) return (void*)0;
    slab->next     = (void*)0;
    slab->capacity = capacity;
    slab->used     = 0;
    return slab;
}

static void* __arena_alloc(size_t len, enum Alignment alignment) {
    if (!__arena_state) return (void*)0;
    char *base    = (char *)(__arena_state->current + 1);
    char *cursor  = base + __arena_state->current->used;
    char *aligned = __alloc_align_ptr(cursor, alignment);
    size_t needed = (size_t)(aligned - base) + len;
    if (needed > __arena_state->current->capacity) {
        // current slab is full — allocate a new one
        __Arena_Slab *slab = __arena_new_slab(len + alignment);
        if (!slab) return (void*)0;
        slab->next = __arena_state->current->next;
        __arena_state->current->next = slab;
        __arena_state->current = slab;
        base    = (char *)(slab + 1);
        aligned = __alloc_align_ptr(base, alignment);
        needed  = (size_t)(aligned - base) + len;
    }
    __arena_state->current->used = needed;
    return aligned;
}

static bool __arena_resize(void* const memory, size_t len, size_t new_len, enum Alignment alignment) {
    (void)alignment;
    if (!__arena_state) return false;
    char *base = (char *)(__arena_state->current + 1);
    // only the last allocation in the current slab can be resized in place
    if ((char*)memory + len != base + __arena_state->current->used) return false;
    size_t offset = (size_t)((char*)memory - base);
    if (offset + new_len > __arena_state->current->capacity) return false;
    __arena_state->current->used = offset + new_len;
    return true;
}

static void* __arena_remap(void* memory, size_t len, size_t new_len, enum Alignment alignment) {
    if (new_len == len) return memory;
    if (__arena_resize(memory, len, new_len, alignment)) return memory;
    void *dest = __arena_alloc(new_len, alignment);
    if (!dest) return (void*)0;
    memcpy(dest, memory, len < new_len ? len : new_len);
    return dest;
}

static void __arena_free(void* memory, size_t len, enum Alignment alignment) {
    // arena semantics: individual frees are no-ops
    (void)memory;
    (void)len;
    (void)alignment;
}

/// Create a slab-based arena allocator backed by `back`.
///
/// The arena requests large slabs from `back` on demand and bump-allocates
/// within them.  Individual `free` calls are no-ops; to reclaim memory the
/// caller must free the slabs through the backing allocator directly.
///
/// Only one arena may be active per thread at a time.
Allocator allocator_create_arena(Allocator back) {
    size_t state_total = sizeof(__Arena_State) + sizeof(__Arena_Slab) + __ARENA_DEFAULT_SLAB;
    __Arena_State *state = (__Arena_State *)back.alloc(state_total, ALLIGN_8);
    if (!state) return (Allocator){0};
    state->back = back;
    __Arena_Slab *first = (__Arena_Slab *)(state + 1);
    first->next     = (void*)0;
    first->capacity = __ARENA_DEFAULT_SLAB;
    first->used     = 0;
    state->current  = first;
    __arena_state = state;
    return (Allocator){
        .alloc  = __arena_alloc,
        .resize = __arena_resize,
        .remap  = __arena_remap,
        .free   = __arena_free,
    };
}

// ---------------------------------------------------------------------------
// Heap allocator  (linked-list free-tracking allocator backed by another allocator)
//
// Each allocation carries a __Heap_Node header followed by a back-pointer
// stored immediately before the returned user pointer, allowing O(1) header
// lookup on resize/free.  One active instance per thread.
//
// Memory layout per allocation:
//   [ __Heap_Node | padding | back_ptr (void*) | <user data> ]
//                                               ^-- returned pointer
// ---------------------------------------------------------------------------

typedef struct __Heap_Node {
    struct __Heap_Node *next;
    struct __Heap_Node *prev;
    size_t block_size;   // total bytes passed to back.alloc (includes header + padding)
    size_t user_size;    // bytes originally requested by the caller
    enum Alignment alignment;
} __Heap_Node;

typedef struct {
    Allocator back;
    __Heap_Node *head;
} __Heap_State;

static _Thread_local __Heap_State *__heap_state = (void*)0;

static inline void *__heap_node_user_ptr(__Heap_Node *node) {
    char *after_node = (char *)(node + 1);
    char *aligned    = __alloc_align_ptr(after_node + sizeof(void*), node->alignment);
    return aligned;
}

static inline __Heap_Node *__heap_node_from_user_ptr(void *ptr) {
    void **back_slot = (void **)ptr - 1;
    return (__Heap_Node *)*back_slot;
}

static void* __heap_alloc(size_t len, enum Alignment alignment) {
    if (!__heap_state) return (void*)0;
    // worst-case padding: alignment bytes after the node + sizeof(void*) for back_ptr
    size_t block = sizeof(__Heap_Node) + alignment + sizeof(void*) + len;
    __Heap_Node *node = (__Heap_Node *)__heap_state->back.alloc(block, ALLIGN_8);
    if (!node) return (void*)0;
    node->block_size = block;
    node->user_size  = len;
    node->alignment  = (enum Alignment)alignment;
    node->prev       = (void*)0;
    node->next       = __heap_state->head;
    if (__heap_state->head) __heap_state->head->prev = node;
    __heap_state->head = node;
    // write the back-pointer slot immediately before the aligned user region
    char *after_node = (char *)(node + 1);
    char *aligned    = __alloc_align_ptr(after_node + sizeof(void*), alignment);
    void **back_slot = (void **)aligned - 1;
    *back_slot = node;
    return aligned;
}

static bool __heap_resize(void* const memory, size_t len, size_t new_len, enum Alignment alignment) {
    (void)alignment;
    (void)len;
    __Heap_Node *node = __heap_node_from_user_ptr((void*)memory);
    if (new_len > node->user_size) return false; // no slack — cannot grow in place
    node->user_size = new_len;
    return true;
}

static void* __heap_remap(void* memory, size_t len, size_t new_len, enum Alignment alignment) {
    if (new_len == len) return memory;
    if (new_len < len && __heap_resize(memory, len, new_len, alignment)) return memory;
    void *dest = __heap_alloc(new_len, alignment);
    if (!dest) return (void*)0;
    memcpy(dest, memory, len < new_len ? len : new_len);
    // unlink and free the old node
    __Heap_Node *node = __heap_node_from_user_ptr(memory);
    if (node->prev) node->prev->next = node->next;
    else            __heap_state->head = node->next;
    if (node->next) node->next->prev = node->prev;
    __heap_state->back.free(node, node->block_size, ALLIGN_8);
    return dest;
}

static void __heap_free(void* memory, size_t len, enum Alignment alignment) {
    (void)len;
    (void)alignment;
    if (!__heap_state || !memory) return;
    __Heap_Node *node = __heap_node_from_user_ptr(memory);
    if (node->prev) node->prev->next = node->next;
    else            __heap_state->head = node->next;
    if (node->next) node->next->prev = node->prev;
    __heap_state->back.free(node, node->block_size, ALLIGN_8);
}

/// Create a heap allocator backed by `back`.
///
/// Each allocation is individually trackable and freely reclaimable via
/// `free`.  `resize` supports only shrinking in place; growing requires a
/// `remap` which may relocate the allocation.
///
/// Only one heap may be active per thread at a time.
Allocator allocator_create_heap(Allocator back) {
    __Heap_State *state = (__Heap_State *)back.alloc(sizeof(__Heap_State), ALLIGN_8);
    if (!state) return (Allocator){0};
    state->back = back;
    state->head = (void*)0;
    __heap_state = state;
    return (Allocator){
        .alloc  = __heap_alloc,
        .resize = __heap_resize,
        .remap  = __heap_remap,
        .free   = __heap_free,
    };
}

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_IMPL_IMP__
#undef ALLOCATOR_IMPLEMENTATIONS
#endif//ALLOCATOR_IMPLEMENTATIONS
