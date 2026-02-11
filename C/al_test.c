#include <stdio.h>

#define AL_LIBC
#include "codebase/arraylist.h"

typedef struct {
  char *items;
  size_t count;
  size_t capacity;
} string;

int main(void) {
  
  string s = {0};

  al_append(&s, 'a');
  al_append(&s, 'a');
  al_append(&s, 'b');
  al_append(&s, 'b');
  al_append(&s, 'o');
  al_append(&s, 'o');
  al_append(&s, 0);

  printf("Hello world, %s\n", s.items);

  return 0;
}
