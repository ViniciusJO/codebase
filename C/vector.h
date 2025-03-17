#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define vector_insert(vector, element, position) do { \
  if((vector).length + 1 > (vector).capacity) vector_grow((vector)); \
  memmove((vector).data + (position) + 1, (vector).data + (position), ((vector).length++ - (position))*sizeof(*(vector).data)); \
  (vector).data[(position)] = (element); \
} while(0)

// TODO: NEED TEST
#define vector_remove_return(vector, element, position) do { \
  if(0 == (vector).length || (position) < 0 || (position) + 1 >= (vector)->length) break; \
  if((element)) *(element) = (vector).data[(position)]; \
  memmove((vector).data+(position)+1, (vector).data+(position), ((vector).length - (position) + 1)*sizeof(*(vector).data)); \
  (vector).length--; \
  vector_shrink((vector)); \
} while(0)

#define vector_remove(vector, position) do { \
  if(0 == (vector).length || (position) < 0 || (position) + 1 >= (vector)->length) break; \
  memmove((vector).data+(position)+1, (vector).data+(position), ((vector).length - (position) + 1)*sizeof(*(vector).data)); \
  (vector).length--; \
  vector_shrink((vector)); \
} while(0)
// NEED TEST

#define vector_foreach(vector, it_name, body) for(size_t it_name = 0; it_name < (vector)->length; it_name++) body

#define vector_print(vector, element_printing_format) do { \
  printf("["); \
  for(size_t i = 0; i < (vector)->length; i++) { \
    printf(element_printing_format, (vector)->data[i]); \
    if(i != (vector)->length-1) printf(", "); \
  } \
  printf("] -> %ld/%ld\n", (vector)->length, (vector)->capacity); \
} while(0)

#endif //__VECTOR_H__
