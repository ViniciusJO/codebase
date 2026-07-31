#include <stdlib.h>
#include <stdio.h>
#include <codebase/aal.h>

int main(void) {
  int *array = NULL;

  aal_append(array, 150);
  aal_append(array, -1);
  aal_append(array, 0);
  aal_append(array, 22);
  aal_append(array, -5);

  array[2] = 10;

  aal_print(array);

  aal_insert(array, 3, 88);
  aal_insert(array, 1, -123456789);
  aal_insert(array, 7, -9);
  aal_insert(array, -1, -321);

  int ar[] = { 1, 2, 3, 4 };
  size_t ar_sz = sizeof(ar)/sizeof(*ar);
  aal_append_many(array, ar, ar_sz);
  aal_print(array);

  aal_remove(array, 2);
  aal_remove(array, -1);
  aal_remove(array, -4);
  aal_print(array);

  aal_pop_many(array, 3);
  aal_print(array);

  aal_remove_unordered(array, 3);
  aal_print(array);

  aal_remove(array, 3);
  aal_print(array);

  aal_insert_many(array, 1, ar, ar_sz);
  aal_print(array);

  aal_remove_many(array, 1, ar_sz);
  aal_print(array);

  int p = aal_pop(array);
  aal_print(array);
  fprintf(stdout, "Popped: %d\n", p);

  aal_foreach(int, i, array)
    printf(" --> [%ld] %d\n", i_idx, *i);
  else puts("NULLLLLLLLL 1");
  



  aal_foreach(int, i, array)
    printf(" --> [%ld] %d\n", i_idx, *i);
  else puts("NULLLLLLLLL 2");

  aal_print_object(array);

  aal_expect_literal(int, array, ARRAY(150,-123456789,10,22));

  aal_free(array);

  fprintf(stdout, "\t\t\t<>%p\n\n", array);

  return 0;
}
