#include <stdio.h>

typedef enum {
  T_UM,
  T_DOIS,
} TU_Tags;

typedef struct {
  TU_Tags _tag;
  union {
    int um;
    float dois;
  } _value;
} Tu;

#define TU_CONSTRUCTOR(T,V) { ._tag = (T), ._value.dois = (V) }
#define TU_READ(TU) ((TU)->_tag == T_UM ? (TU)->_value.um : (TU)->_value.dois)
#define TU_TAG(TU) (TU)->_tag

// #define TU_LIST 

#pragma GCC poison _tag _value

int main(void) {
  Tu t = TU_CONSTRUCTOR(T_DOIS, 5.3f);
  printf("Hello World#\n%f\n", TU_READ(&t));

  switch(TU_TAG(&t)) {
    case T_UM: break;
    case T_DOIS: break;
  };

  return 0;
}
