#define AL_IMPLEMENTATIONS
#define MAP_IMPLEMENTATIONS

#include "include/std.h"
#include <include/al.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: fix
#define CLEANUP(function) __attribute__((cleanup(function)))

void mfree(void* x) {
  printf("freed: %p\n", x);
  free(x);
}

int main(void) {
  defer_block(printf(FG_GREEN "SAIDA\n" FG_RESET)) printf(FG_RED "\n\nENTRADA\n" FG_RESET);

  defer_block(printf(FG_GREEN "defer\n" FG_RESET)) {
    printf(FG_YELLOW "\naction: 1\n" FG_RESET);
    printf(FG_YELLOW "\naction: 2\n" FG_RESET);
    printf(FG_YELLOW "\naction: 3\n" FG_RESET);
  }

  return 0;
}
