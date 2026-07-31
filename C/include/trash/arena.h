#ifndef __ARENA_H__
#define __ARENA_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#ifndef ARENA_ATTRIBUTE
#define ARENA_ATTRIBUTE
#endif//ARENA_ATTRIBUTE

#include <stddef.h>

typedef struct { void *start, *current, *end; } arena;

ARENA_ATTRIBUTE arena arena_new(size_t capacity);
ARENA_ATTRIBUTE void* arena_alloc(arena* self, size_t size);
ARENA_ATTRIBUTE void arena_reset(arena* self);
ARENA_ATTRIBUTE void arena_free(arena* self);


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ARENA_H__

#define ARENA_IMPLEMENTATIONS
#ifdef ARENA_IMPLEMENTATIONS
#ifndef __ARENA_IMP__
#define __ARENA_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdlib.h>
#include <stdalign.h>

#define ARENA_ALIGN(SIZE, ALIGN) (((int)(SIZE) + ALIGN - 1) / ALIGN * ALIGN)



ARENA_ATTRIBUTE arena arena_new(size_t capacity) {
  static size_t alignment = alignof(max_align_t);
  size_t aligned_size = ARENA_ALIGN(capacity, alignment);
  void* data = malloc(aligned_size);
  return (arena){
    .start = data,
    .current = data,
    .end = (char*)data + capacity,
  };
}

void* arena_alloc(arena* self, size_t size) {
  static size_t alignment = alignof(max_align_t);
  size_t aligned_size = ARENA_ALIGN(size, alignment);
  if(
      self == NULL ||
      self->start == NULL ||
      (char*)self->end - (char*)self->current < (unsigned int)aligned_size
  ) return NULL;
  void* ret = self->current;
  self->current = (char*)self->current + aligned_size;
  return ret;
}

void arena_reset(arena* self) {
  self->current = self->start;
}

void arena_free(arena* self) {
  self->current = NULL;
  self->end = NULL;
  free(self->start);
  self->start = NULL;
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__ARENA_IMP__
#undef ARENA_IMPLEMENTATIONS
#endif//ARENA_IMPLEMENTATIONS

