#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int *data;
  size_t length;
  size_t capacity;
} Vector;

// TODO: make all pass by pointers
#define vector_grow(vector) do { \
  if(0 == (vector).capacity) (vector).capacity = 2; \
  else (vector).capacity *= 2; \
  (vector).data = realloc((vector).data, (vector).capacity * sizeof(*(vector).data)); \
} while(0)

#define vector_shrink(vector) do { \
  if((vector).length >= (vector).capacity/2) break; \
  (vector).capacity /= 2; \
  (vector).data = realloc((vector).data, (vector).capacity * sizeof(*(vector).data)); \
} while(0)



#define vector_append(vector, element) do { \
  if((vector).length + 1 > (vector).capacity) vector_grow((vector)); \
  (vector).data[(vector).length++] = (element); \
} while(0)

#define vector_pop_return(vector, element) do { \
  if(0 == (vector).length) break; \
  if((element)) *(element) = (vector).data[--(vector).length]; \
  else (vector).length--; \
  vector_shrink((vector)); \
} while(0)

#define vector_pop(vector) do { \
  if(0 == (vector).length) break; \
  else (vector).length--; \
  vector_shrink((vector)); \
} while(0)

#define vector_shift_return(vector, element) do { \
  if(0 == (vector).length) break; \
  if((element)) *(element) = *(vector).data; \
  memmove((vector).data, (vector).data+1, ((vector).length - 1)*sizeof(*(vector).data)); \
  (vector).length--; \
  vector_shrink((vector)); \
} while(0)

#define vector_shift(vector) do { \
  if(0 == (vector).length) break; \
  memmove((vector).data, (vector).data+1, ((vector).length - 1)*sizeof(*(vector).data)); \
  (vector).length--; \
  vector_shrink((vector)); \
} while(0)

#define vector_unshift(vector, element) do { \
  if((vector).length + 1 > (vector).capacity) vector_grow((vector)); \
  memmove((vector).data + 1, (vector).data, ((vector).length++)*sizeof(*(vector).data)); \
  *(vector).data = (element); \
} while(0)

#define print_vector(vector, element_printing_format) do { \
  printf("["); \
  for(size_t i = 0; i < (vector)->length; i++) { \
    printf(element_printing_format, (vector)->data[i]); \
    if(i != (vector)->length-1) printf(", "); \
  } \
  printf("] -> %ld/%ld\n", (vector)->length, (vector)->capacity); \
} while(0)

#define insert(vector, element, position) do { \
  if((vector).length + 1 > (vector).capacity) vector_grow((vector)); \
  memmove((vector).data + (position) + 1, (vector).data + (position), ((vector).length++ - (position))*sizeof(*(vector).data)); \
  (vector).data[(position)] = (element); \
} while(0)


void debug_vector(Vector *v) {
  printf("[");
  for(size_t i = 0; i < v->length; i++) {
    printf("%d", v->data[i]);
    if(i != v->length-1) printf(", ");
  }
  printf("] -> %ld/%ld\n", v->length, v->capacity);
}

struct Vec {
  double *data;
  size_t length;
  size_t capacity;
};

int main(void) {

  Vector v = {0};

  vector_append(v, 3);
  vector_append(v, 5);
  vector_append(v, 6);
  vector_append(v, 0);
  vector_append(v, 12);
  vector_append(v, 3);
  vector_append(v, 5);
  vector_append(v, 6);
  vector_append(v, 0);
  vector_append(v, 12);

  debug_vector(&v);

  int e;
  vector_pop_return(v, &e);
  printf("OUT: %d\n", e);

  vector_pop(v);
  vector_pop(v);
  vector_pop(v);
  vector_pop(v);
  vector_pop(v);
  vector_pop(v);
  vector_pop(v);
  debug_vector(&v);

  vector_shift(v);
  debug_vector(&v);

  vector_append(v, 6);
  vector_append(v, 6);
  debug_vector(&v);

  vector_pop_return(v, &e);
  printf("OUT: %d\n", e);

  vector_pop_return(v, &e);
  debug_vector(&v);


  vector_unshift(v, 32);
  vector_unshift(v, 0);
  debug_vector(&v);
  insert(v, 15, 1);
  print_vector(&v, "%d");

  struct Vec vec = {0};
  vector_append(vec, 2);
  print_vector(&vec, "%.1lf");

  return 0;
}
