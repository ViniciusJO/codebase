#include <stdio.h>

typedef struct {
  int teste;
  char *name;
} ex_struct;

void myfunc_(ex_struct s);
#define myfunc(...) myfunc_((ex_struct){ __VA_ARGS__ })
void myfunc_(ex_struct s) {
  printf("%d, %s\n\n", s.teste, s.name);
}

#define var(...) do {\
  printf(#__VA_ARGS__"\n");\
} while(0)

#define vari(ARGS...) do {\
  printf(#ARGS"\n");\
} while(0)

#define A(B) __typeof__(B)

#ifndef __clang__
#define __DEFER(COUNTER) \
  auto void DEFER_##COUNTER##_FUNCTION(void*); \
  int DEFER_##COUNTER##_VAR __attribute__((__cleanup__(DEFER_##COUNTER##_FUNCTION))); \
  auto void DEFER_##COUNTER##_FUNCTION(void*)
#define __DEFER_(N) __DEFER(N)
#define defer __DEFER_(__COUNTER__)
#else
#define defer
#error Defer not implementable in clang
// We need the "blocks" extension
// typedef void (^const __df_t)(void);
//
// [[maybe_unused]]
// static inline
// void __df_cb(__df_t* __fp) {
//   (*__fp)();
// }
// #define __DEFER__(V) [[gnu::cleanup(__df_cb)]] __df_t V = ^void(void)
//
// #define defer __DEFER(__COUNTER__)
// #define __DEFER(N) __DEFER_(N)
// #define __DEFER_(N) __DEFER__(__DEFER_VARIABLE_ ## N)
#endif

#define DEBUG_PRINT(msg) \
    printf("[%s] %s\n", __FUNCTION__, msg)

void test(void) {
  DEBUG_PRINT("ABCD");
}

typedef struct {
  int a, b, c;
} Test;

int main(void) {
  defer { printf("TEST %d\n", 9); };

  Test t = { .a = 9, .b = 10, .a = (-150, 98) };

  defer { printf("TEST %d\n", t.b); };

  var(3, 4, .test = 5, 6);
  vari(3, 4, .test = 5, 6);

  myfunc(.name = "NOME", .teste = t.a);
  defer { DEBUG_PRINT("__"); };

  DEBUG_PRINT("1234");

  test();

  return 0;
}
