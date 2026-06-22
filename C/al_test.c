#include <stdio.h>
#include <stdlib.h>

#include "codebase/al.h"

typedef struct {
  char *items;
  size_t count;
  size_t capacity;
} string;

al_define(int, Array);

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

  Array arr = {0};

  al_expect_literal(int, &arr, ARRAY());

  al_append(&arr, 73);
  al_append(&arr, 73);
  al_append(&arr, 73);
  al_append(&arr, 73);


  al_print_object(&arr);
  al_print(&arr);
  al_expect_literal(int, &arr, ARRAY(73,73,73,73));

  int elements[] = { 1, 2, 3, 4, 5, 6, 7 };
  size_t elements_sz = sizeof(elements)/sizeof(*elements);

  al_append_many(&arr, elements, elements_sz);
  al_expect_literal(int, &arr, ARRAY(73,73,73,73,1,2,3,4,5,6,7));
  al_print(&arr);

  al_insert_many(&arr, 2, elements, elements_sz);
  al_expect_literal(int, &arr, ARRAY(73,73,1,2,3,4,5,6,7,73,73,1,2,3,4,5,6,7));
  al_print(&arr);

  al_insert(&arr, 3, 4);
  al_expect_literal(int, &arr, ARRAY(73,73,1,4,2,3,4,5,6,7,73,73,1,2,3,4,5,6,7));
  al_print(&arr);

  int popped_el = al_pop(&arr);
  AL_ASSERT(7 == popped_el);
  al_expect_literal(int, &arr, ARRAY(73,73,1,4,2,3,4,5,6,7,73,73,1,2,3,4,5,6));
  al_print(&arr);

  int *popped_els = al_pop_many(&arr, 3);
  AL_ASSERT(NULL != popped_els);
  al_expect_literal(int, &arr, ARRAY(73,73,1,4,2,3,4,5,6,7,73,73,1,2,3));
  al_print(&arr);

  al_remove_unordered(&arr, 0);
  al_expect_literal(int, &arr, ARRAY(3,73,1,4,2,3,4,5,6,7,73,73,1,2));
  al_print(&arr);

  al_remove(&arr, 2);
  al_expect_literal(int, &arr, ARRAY(3,73,4,2,3,4,5,6,7,73,73,1,2));
  al_print(&arr);

  al_remove_many(&arr, 4, 3);
  al_expect_literal(int, &arr, ARRAY(3,73,4,2,6,7,73,73,1,2));
  al_print(&arr);

  al_print_object(&arr);

  al_foreach(int, i, &arr) printf("==> [%ld] %d,\n", i_idx, *i);
  else puts("NULLLLLLL1");

  al_free(&arr);

  al_foreach(int, i, &arr) printf("==> [%ld] %d,\n", i_idx, *i);
  else puts("NULLLLLLL2");

  return 0;
}
