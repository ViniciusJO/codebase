#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdlib.h>

#ifdef AL_LIBC

#ifndef AL_ASSERT
#include <assert.h>
#define AL_ASSERT assert
#endif//AL_ASSERT

#ifndef AL_REALLOC
#include <stdlib.h>
#define AL_REALLOC realloc
#endif//AL_REALLOC

#ifndef AL_FREE
#include <stdlib.h>
#define AL_FREE free
#endif//AL_FREE

#else

#ifndef AL_ASSERT
#error AL_ASSERT \
  undefined (if libc assert is intended define AL_LIBC, else define your own AL_ASSERT)
#endif//AL_ASSERT

#ifndef AL_REALLOC
#error AL_REALLOC \
  undefined (if libc realloc is intended define AL_LIBC, else define your own AL_REALLOC)
#endif//AL_REALLOC

#ifndef AL_FREE
#error AL_FREE \
  undefined (if libc free is intended, define AL_LIBC, else define your own AL_FREE)
#endif//AL_FREE

#endif


// Initial capacity of an array list
#ifndef AL_INIT_CAP
#define AL_INIT_CAP 256
#endif//AL_INIT_CAP

#define al_reserve(al, expected_capacity) do {                                     \
  if ((expected_capacity) > (al)->capacity) {                                      \
    if ((al)->capacity == 0) (al)->capacity = AL_INIT_CAP;                         \
    while ((expected_capacity) > (al)->capacity) (al)->capacity *= 2;              \
    (al)->items = AL_REALLOC((al)->items, (al)->capacity * sizeof(*(al)->items));  \
    AL_ASSERT((al)->items != NULL);                                                \
  }                                                                                \
} while (0)

// Append an item to an array list
#define al_append(al, item) do {       \
  al_reserve((al), (al)->count + 1);   \
  (al)->items[(al)->count++] = (item); \
} while (0)

#define al_free(al) AL_FREE((al).items)

// Append several items to a dynamic array
#define al_append_many(al, new_items, new_items_count) do {  \
  al_reserve((al), (al)->count + (new_items_count));         \
  memcpy(                                                    \
    (al)->items + (al)->count,                               \
    (new_items),                                             \
    (new_items_count)*sizeof(*(al)->items)                   \
  );                                                         \
  (al)->count += (new_items_count);                          \
} while (0)

#define al_resize(al, new_size) do {  \
  al_reserve((al), new_size);       \
  (al)->count = (new_size);         \
} while (0)

#define al_last(al) (al)->items[(AL_ASSERT((al)->count > 0), (al)->count-1)]
#define al_remove_unordered(al, i) do {        \
  size_t j = (i);                              \
  AL_ASSERT(j < (al)->count);                  \
  (al)->items[j] = (al)->items[--(al)->count]; \
} while(0)

// Foreach over Dynamic Arrays. Example:
// ```c
// typedef struct {
//     int *items;
//     size_t count;
//     size_t capacity;
// } Numbers;
//
// Numbers xs = {0};
//
// al_append(&xs, 69);
// al_append(&xs, 420);
// al_append(&xs, 1337);
//
// al_foreach(int, x, &xs) {
//     // `x` here is a pointer to the current element. You can get its index by taking a difference
//     // between `x` and the start of the array which is `x.items`.
//     size_t index = x - xs.items;
//     nob_log(INFO, "%zu: %d", index, *x);
// }
// ```
#define al_foreach(Type, it, al) \
  for (size_t it##_idx = 0, Type *it = (al)->items; it < (al)->items + (al)->count; ++it##_idx, ++it)

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ARRAYLIST_H__

//#define ARRAYLIST_IMPLEMENTATIONS
#ifdef ARRAYLIST_IMPLEMENTATIONS
#ifndef __ARRAYLIST_IMP__
#define __ARRAYLIST_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ARRAYLIST_IMP__
#undef ARRAYLIST_IMPLEMENTATIONS
#endif//ARRAYLIST_IMPLEMENTATIONS

