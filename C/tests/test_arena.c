#include <stdalign.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

#define ARENA_IMPLEMENTATIONS
#include "include/arena.h"

int main(void) {
  arena a = arena_new(1024);

  char *c  = arena_alloc(&a, 1);
  long *l  = arena_alloc(&a, sizeof(long)*2);
  long *l2 = arena_alloc(&a, sizeof(long));

  printf("\n");
  printf("c: %p\nl: %p\nl2: %p\n\n", c, l, l2);
  printf("c_alignment: %lu\nl_alignment: %lu\nl2_alignment: %lu\n\n",
    (uintptr_t)c % 1,
    (uintptr_t)l % 8,
    (uintptr_t)l2 % 8
  );
  printf("l2_from_l: %lu\n\n", (uintptr_t)l2 - (uintptr_t)l);
  printf("allignment: %lu\n\n", alignof(max_align_t));

  arena_free(&a);

  return 0;
}
