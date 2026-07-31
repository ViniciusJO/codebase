#include <stdalign.h>
#include <stdio.h>

#define STD_IMPLEMENTATIONS
// #define ALLOCATOR_IMPLEMENTATIONS
#include "./include/std.h"

int main(void) {
  // Allocator a = dynamic_heap_allocator_init();
  Allocator gpa = dynamic_heap_allocator_init();
  // Allocator a = static_arena_allocator_init(&gpa, 4096);
  Allocator a = dynamic_arena_allocator_init(&gpa, 4096);

  char *c  = allocate(&a, 1);
  long *l  = allocate(&a, sizeof(long)*2);
  long *l2 = allocate(&a, sizeof(long));

  printf("\n");
  printf("c: %p\nl: %p\nl2: %p\n\n", c, l, l2);
  printf("c_alignment: %lu\nl_alignment: %lu\nl2_alignment: %lu\n\n",
    (uintptr_t)c % 1,
    (uintptr_t)l % 8,
    (uintptr_t)l2 % 8
  );
  printf("l2_from_l: %lu\n\n", (uintptr_t)l2 - (uintptr_t)l);
  printf("allignment: %lu\n\n", alignof(max_align_t));

  allocator_reset(&a);
  allocator_deinit(&a);

  return 0;
}
