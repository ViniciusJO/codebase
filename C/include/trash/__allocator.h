





/*
  // SLAB, ZONE

  Allocator types:
    - arena
    - heap
    - pool
    - page
    // - zone

  Allocation strategies
    - linear
    - stack
    - free list
    - buddy

  Allocation source
    - fixed buffer
    - fixed size
    - heap dynamic
    - page ?
    - linked blocks

*/

#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__

#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifndef ALLOCATOR_ATTRIBUTE
#define ALLOCATOR_ATTRIBUTE
#endif//ALLOCATOR_ATTRIBUTE


typedef enum {
  HEAP, ARENA, BUFFER,

  PAGE, STACK,
  POOL, /* or BLOCK  */
  BUMP, /* or LINEAR */
  SLAB, FREE_LIST, ZONE, BUDDY,
} Allocator_Type;

typedef enum Alignment {
  ALLIGN_1 = 1,
  ALLIGN_2 = 2,
  ALLIGN_4 = 4,
  ALLIGN_8 = 8,
  ALLIGN_16 = 16,
  ALLIGN_32 = 32,
  ALLIGN_64 = 64,
} Alignment;

typedef void* Heap_Allocation_Context;

typedef struct Arena_Region {
  struct Arena_Region* next;
  usize capacity;
  usize count;
  void* data;
  void* next_alloc;
} Arena_Region;

typedef struct {
  struct Allocator* child_allocator;
  Arena_Region* region;
  usize region_default_capacity;
} Arena_Allocator_Context;

typedef struct {
  usize count;
  usize capacity;
  void* data;
} Buffer_Allocator_Context;


// TODO: implement
typedef void* Page_Allocator_Context;
typedef void* Stack_Allocator_Context;
typedef void* Pool_Allocator_Context;
typedef void* Bump_Allocator_Context;
typedef void* Slab_Allocator_Context;
typedef void* Free_list_Allocator_Context;
typedef void* Zone_Allocator_Context;
typedef void* Buddy_Allocator_Context;

typedef struct Allocator {
    Allocator_Type type;
    union {
      Arena_Allocator_Context arena;
      Buffer_Allocator_Context buff;
      Heap_Allocation_Context heap;

      // TODO: implement
      Page_Allocator_Context page;
      Stack_Allocator_Context stack;
      Pool_Allocator_Context pool;
      Bump_Allocator_Context bump;
      Slab_Allocator_Context slab;
      Free_list_Allocator_Context free_list;
      Zone_Allocator_Context zone;
      Buddy_Allocator_Context buddy;
    };
} Allocator;

ALLOCATOR_ATTRIBUTE void* allocate(Allocator* a, usize len, enum Alignment alignment);

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
ALLOCATOR_ATTRIBUTE void* reallocate(Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment);

ALLOCATOR_ATTRIBUTE void deallocate(Allocator* a, void* data, usize len, enum Alignment alignment);


ALLOCATOR_ATTRIBUTE void allocator_deinit(Allocator* a);

ALLOCATOR_ATTRIBUTE Allocator arena_allocator_init(Allocator* child_allocator, usize capacity);
ALLOCATOR_ATTRIBUTE Allocator buffer_allocator_init(void* buffer, usize capacity);
ALLOCATOR_ATTRIBUTE Allocator heap_allocator_init();

// TODO: implement
ALLOCATOR_ATTRIBUTE Allocator page_allocator();
ALLOCATOR_ATTRIBUTE Allocator stack_allocator();
ALLOCATOR_ATTRIBUTE Allocator pool_allocator();
ALLOCATOR_ATTRIBUTE Allocator bump_allocator();
ALLOCATOR_ATTRIBUTE Allocator slab_allocator();
ALLOCATOR_ATTRIBUTE Allocator free_list_allocator();
ALLOCATOR_ATTRIBUTE Allocator zone_allocator();
ALLOCATOR_ATTRIBUTE Allocator buddy_allocator();


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_H__


#define ALLOCATOR_IMPLEMENTATIONS
#ifdef ALLOCATOR_IMPLEMENTATIONS
#ifndef __ALLOCATOR_IMP__
#define __ALLOCATOR_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))

// TODO: implement
static inline usize allign_resolve(usize len, enum Alignment alignment) {
  return len + alignment;
}

static Arena_Region* region_create(Allocator* a, usize capacity) {
  if(a->type != ARENA) return NULL;
  Arena_Region* region = (Arena_Region*)allocate(a->arena.child_allocator, sizeof(Arena_Region), ALLIGN_1);
  region->next = NULL;
  region->capacity = capacity;
  region->data = allocate(a->arena.child_allocator, capacity, ALLIGN_1);
  region->next_alloc = region->data;
  region->count = 0;
  return region;
}

static void* region_allocate(Allocator* a, Arena_Region* region, usize size) {
  if(a->type != ARENA || region == NULL) return NULL;
  if((region->capacity - region->count) < size) {
    if(region->next == NULL) {
      usize cap = (size > region->capacity) ? size : region->capacity;
      region->next = region_create(a,cap);
    }
    return region_allocate(a, region->next, size);
  }

  void* data = region->next_alloc;

  region->next_alloc = (char*)data + size;
  // TODO: VERIFY ------vvv
  region->count += size - 1;

  return data;
}

static void region_deallocate(Allocator* a, Arena_Region* region) {
  if(a->type != ARENA || region == NULL) return;
  if(region->next) region_deallocate(a, region->next);
  deallocate(a->arena.child_allocator, region->data, region->capacity, ALLIGN_1);
  // printf("\n\nchild\n\n");
  deallocate(a->arena.child_allocator, region, sizeof(Arena_Region), ALLIGN_1);
}


static void* arena_allocate(Allocator* a, usize size) {
  if(a->type != ARENA) return NULL;
  if(a->arena.region == NULL) a->arena.region = region_create(a, MAX(size, a->arena.region_default_capacity));
  return region_allocate(a, a->arena.region, size);
}

static void* buffer_allocate(Allocator* a, usize size) {
  if(a->type != BUFFER) return NULL;
  if(a->buff.capacity - a->buff.count < size) return NULL;
  void *data = (char*)a->buff.data + a->buff.count;
  a->buff.count += size - 1;
  return data;
}



// =================================== API ===================================

// TODO:
extern void *malloc (usize __size);
extern void *realloc (void *__ptr, usize __size);
extern void free(void *__ptr);

ALLOCATOR_ATTRIBUTE void* allocate(Allocator* a, usize len, enum Alignment alignment) {
  const usize size = allign_resolve(len, alignment);
  switch(a->type) {
    case ARENA: return arena_allocate(a, size);
    case BUFFER: return buffer_allocate(a, size);
    case HEAP: return malloc(size);

    // TODO: implement
    case PAGE: { } break;
    case STACK: { } break;
    case POOL: { } break;
    case BUMP: { } break;
    case SLAB: { } break;
    case FREE_LIST: { } break;
    case ZONE: { } break;
    case BUDDY: { } break;
  }
  return NULL;
}

// TODO: implement
ALLOCATOR_ATTRIBUTE void* reallocate(Allocator* a, void* data, usize len, usize new_len, enum Alignment alignment) {
  const usize size = allign_resolve(len, alignment);
  const usize new_size = allign_resolve(new_len, alignment);
  switch(a->type) {
    case ARENA: //return arena_reallocate(a, size);
    case BUFFER: //return buffer_reallocate(a, size);
    case HEAP: //return realloc(data, new_size);

    // TODO: implement
    case PAGE: { } break;
    case STACK: { } break;
    case POOL: { } break;
    case BUMP: { } break;
    case SLAB: { } break;
    case FREE_LIST: { } break;
    case ZONE: { } break;
    case BUDDY: { } break;
  }
  return NULL;
}

ALLOCATOR_ATTRIBUTE void deallocate(Allocator* a, void* data, usize len, enum Alignment alignment) {
  switch(a->type) {
    case ARENA: { } break;
    case BUFFER: {
      // if last allocation
      if((char*)data + len == (char*)a->buff.data + a->buff.count)
        a->buff.count -= len;
      return;
    } break;
    case HEAP: {
      free(data);
      return;
    } break;

    // TODO: implement
    case PAGE: { } break;
    case STACK: { } break;
    case POOL: { } break;
    case BUMP: { } break;
    case SLAB: { } break;
    case FREE_LIST: { } break;
    case ZONE: { } break;
    case BUDDY: { } break;
  }
}

ALLOCATOR_ATTRIBUTE void allocator_deinit(Allocator* a) {
  switch(a->type) {
    case ARENA: {
      region_deallocate(a, a->arena.region);
      a->arena.region->capacity = 0;
      a->arena.region->count = 0;
      a->arena.region->data = NULL;
      a->arena.region->next_alloc = NULL;
      a->arena.region->next = NULL;
    } break;
    case BUFFER: {
      a->buff.count = 0;
      return;
    } break;
    case HEAP: { return; } break;

    // TODO: implement
    case PAGE: { } break;
    case STACK: { } break;
    case POOL: { } break;
    case BUMP: { } break;
    case SLAB: { } break;
    case FREE_LIST: { } break;
    case ZONE: { } break;
    case BUDDY: { } break;
  }
}



// ========================= INITS =========================

ALLOCATOR_ATTRIBUTE Allocator arena_allocator_init(Allocator* child_allocator, usize capacity) {
  return (Allocator){
    .type = ARENA,
    .arena = (Arena_Allocator_Context){
      .child_allocator = child_allocator,
      .region = NULL,
      .region_default_capacity = capacity,
    }
  };
}

ALLOCATOR_ATTRIBUTE Allocator buffer_allocator_init(void* buffer, usize capacity) {
  return (Allocator){
    .type = BUFFER,
    .buff =  (Buffer_Allocator_Context){
      .count = 0,
      .capacity = capacity,
      .data = buffer,
    },
  };
}

ALLOCATOR_ATTRIBUTE Allocator heap_allocator_init() {
  return (Allocator){ .type = HEAP, .heap = NULL, };
}

// TODO: implement
ALLOCATOR_ATTRIBUTE Allocator page_allocator();
ALLOCATOR_ATTRIBUTE Allocator stack_allocator();
ALLOCATOR_ATTRIBUTE Allocator pool_allocator();
ALLOCATOR_ATTRIBUTE Allocator bump_allocator();
ALLOCATOR_ATTRIBUTE Allocator slab_allocator();
ALLOCATOR_ATTRIBUTE Allocator free_list_allocator();
ALLOCATOR_ATTRIBUTE Allocator zone_allocator();
ALLOCATOR_ATTRIBUTE Allocator buddy_allocator();


#undef MIN
#undef MAX

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ALLOCATOR_IMP__
#undef ALLOCATOR_IMPLEMENTATIONS
#endif//ALLOCATOR_IMPLEMENTATIONS


