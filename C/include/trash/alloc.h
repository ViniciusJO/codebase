typedef __SIZE_TYPE__ size_t;

typedef enum {
    MEMORY_SOURCE_BUFFER,
    MEMORY_SOURCE_HEAP,
    MEMORY_SOURCE_PAGE,
    MEMORY_SOURCE_STACK,
    MEMORY_SOURCE_SHARED,
    MEMORY_SOURCE_DEVICE,
} MemorySource;

typedef enum {
    STRATEGY_GENERAL,
    STRATEGY_ARENA,
    STRATEGY_STACK,
    STRATEGY_POOL,
    STRATEGY_ZONE,
} AllocationStrategy;


typedef enum {
    ALGORITHM_BUMP,
    ALGORITHM_FREE_LIST,
    ALGORITHM_BUDDY,
    ALGORITHM_SLAB,
    ALGORITHM_BITMAP,
    ALGORITHM_SEGREGATED,
} AllocationAlgorithm;


typedef enum { OWNING, NON_OWNING, } Ownership;


typedef enum {
    FIXED_CAPACITY,
    CHUNKED,
    GEOMETRIC,
    ON_DEMAND,
} GrowthPolicy;


typedef enum {
    THREAD_UNSAFE,
    THREAD_MUTEX,
    THREAD_SPINLOCK,
    THREAD_LOCK_FREE,
    THREAD_LOCAL,
} ThreadModel;


typedef struct {
    MemorySource source;
    AllocationStrategy strategy;
    AllocationAlgorithm algorithm;
    Ownership ownership;
    GrowthPolicy growth;
    ThreadModel threading;
} AllocatorProperties;


typedef struct {
    size_t allocated;
    size_t peak;
    size_t allocations;
    size_t frees;
    size_t failed;
} AllocatorStats;


typedef struct {
    const char *name;
    AllocatorProperties properties;
    // AllocatorKind kind;
} AllocatorInfo;


typedef struct Allocator Allocator;

struct Allocator {
  AllocatorInfo *info;
  struct Allocator *parent;

  void *(*allocate)(Allocator *alloc, size_t size, size_t alignment);
  void *(*reallocate)(Allocator *alloc, void *ptr, size_t old_size, size_t new_size, size_t alignment);
  void (*deallocate)(Allocator *alloc, void *ptr, size_t size, size_t alignment);
  bool (*owns)(Allocator *alloc, const void *ptr);
  void (*reset)(Allocator *alloc);
  void (*deinit)(Allocator *alloc);
};

void* allocate(Allocator *alloc, size_t size, size_t allignment) {
  return alloc->allocate(alloc, size, allignment);
}

void reallocate(Allocator *alloc, void* ptr, size_t old_size, size_t new_size, size_t allignment) {
  alloc->reallocate(alloc, ptr, old_size, new_size, allignment);
}

void deallocate(Allocator *alloc, void* ptr, size_t size, size_t allignment) {
  alloc->deallocate(alloc, ptr, size, allignment);
}

bool allocator_owns(Allocator *alloc, const void *ptr) {
  return alloc->owns(alloc, ptr);
}

void allocator_reset(Allocator *alloc) {
  alloc->reset(alloc);
}

void allocator_destroy(Allocator *alloc) {
  alloc->deinit(alloc);
}


// struct A {
//   void *mem;
//   enum { SRC_HEAP, SRC_BUFFER, SRC_PAGE, SRC_MAP, SRC_BACKING } source;
//   bool dynamic;
//   enum { HEAP_ALLOC, ARENA_ALLOC, POOL_ALLOC, STACK_ALLOC } type;
//   struct A* backing;
// };

/*
  - Arrena
    - fixed size
      - buffer
      - heap
      - page
      - map
    - dynamic size
      - heap
      - page
      - map
  - Heap
  - Pool
  - Stack
*/

// static_arena
// dynamic_arena

#include <stdlib.h>
#include <stdalign.h>

#define ALLOCATOR_ALIGN(SIZE, ALIGN) (((int)(SIZE) + ALIGN - 1) / ALIGN * ALIGN)

void* __dynamic_heap_allocate(Allocator* alloc, size_t size, size_t allign) {
  (void)alloc;
  size_t alligned = ALLOCATOR_ALIGN(size, allign);
  void* ret = malloc(alligned);
  return ret;
}

void __dynamic_heap_deallocate(Allocator* alloc, void* ptr, size_t size, size_t allign) {
  (void)alloc;
  (void)size;
  (void)allign;
  free(ptr);
}

void* __dynamic_heap_reallocate(Allocator *alloc, void *ptr, size_t old_size, size_t new_size, size_t alignment) {
  (void)alloc;
  size_t alligned = ALLOCATOR_ALIGN(new_size, alignment);
  return realloc(ptr, alligned);
}

bool __dynamic_heap_owns(Allocator *alloc, const void *ptr) {

}

void __dynamic_heap_reset(Allocator *alloc) {
  (void)alloc;
}

void __dynamic_heap_destroy(Allocator *alloc) {
  (void)alloc;
}

Allocator dynamic_heap = {
  .allocate = __dynamic_heap_allocate,
  .reallocate = __dynamic_heap_reallocate,
  .deallocate = __dynamic_heap_deallocate,
};

/*
  - Fixed buffer
  - Memory maps
  - Generic Heap
*/
