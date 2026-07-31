#include <stdbool.h>
#include <stdio.h>
#include <codebase/ANSI.h>

#define MAP_IMPLEMENTATIONS
#include <codebase/map.h>

static void test_entry(map m, map_entry e, bool delete_test) {
  map_entry* me = map_insert(m, ME(e.key, &e));
  if(me == NULL) {
    fflush(stdout);
    fflush(stderr);
    fprintf(stderr, FG_RED "[ERROR] " FG_RESET "cannot insert %s\n", e.key);
    return;
  }
  printf("%p " FG_YELLOW "->" FG_RESET " map[\"%s\"]\n", me, me->key);

  map_entry* s = map_get(m, me->key);
  assert(s != NULL);
  assert(s->value == me->value);
  assert(strcmp(s->key, me->key) == 0);

  if(delete_test) {
    map_delete(m, me->key);

    s = map_get(m, me->key);
    assert(s == NULL);
  }
}

int main(void) {
  map m = {0};
  int el = 7;
  char* elx = "test string";

  test_entry(m, (map_entry){ .key = "map", .value = &el }, true);
  test_entry(m, ME("key", &elx), true);
  test_entry(m, ME("test", &elx), true);
  test_entry(m, ME("name", &elx), true);
  test_entry(m, ME("zxy", &elx), true);

  printf("\n");
  map_dump(m, printf);
  printf("\n");

  test_entry(m, ME("key_one", &elx), false);
  test_entry(m, ME("username", &elx), false);
  test_entry(m, ME("nickname", &elx), false);
  test_entry(m, ME(".", &elx), false);
  test_entry(m, ME("", &elx), false);


  printf("\n");
  map_dump(m, printf);
  printf("\n");

  test_entry(m, ME("john", &elx), false);
  test_entry(m, ME("ane", &el), false);
  test_entry(m, ME("james", &elx), false);
  test_entry(m, ME("carl", &el), false);

  test_entry(m, ME("carl", &elx), false);
  test_entry(m, ME("carl", &elx), false);
  test_entry(m, ME("carl", &elx), false);
  test_entry(m, ME("carl", &elx), false);
  test_entry(m, ME("carl", &elx), false);

  printf("\n");
  map_dump(m, printf);
  printf("\n");

  return 0;
}
