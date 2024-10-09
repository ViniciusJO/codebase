#include <stdbool.h>

#define ARRAY_LIST_IMPLEMENTATION
#include "arraylist.h"

#define COMPARE_INT_IMPLEMENTATION
#include "compare_int.h"

int printf(const char *, ...);

int main(int argc, char **argv) {
  bool a = true;

  ArrayList *al = arraylist_new(0);
  arraylist_append(al, "abc");
  /*arraylist_append(al, 0);*/
  arraylist_append(al, &argc);
  arraylist_append(al, &a);
  arraylist_append(al, "___-____");
  arraylist_append(al, "....");
  arraylist_append(al, argv[0]);

  for (int i = 0; i < al->length; i++) {
    printf("%s\n", (char *)al->data[i]);
  }

  printf("\n\n%d\n\n", int_compare(&argc, &a));

  return 0;
}
