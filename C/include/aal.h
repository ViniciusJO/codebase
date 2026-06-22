#ifndef __AAL_H__
#define __AAL_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stddef.h>

struct _IO_FILE;
typedef struct _IO_FILE FILE;

/* Standard streams.  */
extern FILE *stdin;		  /* Standard input stream.        */
extern FILE *stdout;		/* Standard output stream.       */
extern FILE *stderr;		/* Standard error output stream. */
/* C89/C99 say they're macros.  Make them happy.  */
#define stdin stdin
#define stdout stdout
#define stderr stderr

typedef struct {
  size_t count;
  size_t capacity;
} __AAL_Header;

#ifndef MAX
#define MAX(A, B) (((A) > (B)) ? (A) : (B))
#endif

#ifndef AAL_ASSERT
#include <assert.h>
#define AAL_ASSERT assert
#endif//AAL_ASSERT

#ifndef AAL_INIT_CAP
#define AAL_INIT_CAP 256
#endif//AAL_INIT_CAP

#ifndef AAL_REALLOC
extern void *realloc(void *__ptr, size_t __size) __THROW __attribute_warn_unused_result__ __attribute_alloc_size__ ((2));
#define AAL_REALLOC realloc
#endif//AAL_REALLOC
      
#ifndef AAL_FREE
extern void free(void *__ptr) __THROW;
#define AAL_FREE free
#endif//AAL_FREE

#ifndef AAL_MEMCPY
extern void *memcpy(void *__restrict __dest, const void *__restrict __src, size_t __n) __THROW __nonnull ((1, 2));
#define AAL_MEMCPY memcpy
#endif//AAL_MEMCPY

#ifndef AAL_FPRINTF
extern int fprintf(FILE *__restrict stream, const char *__restrict format, ...) __nonnull ((1));
#define AAL_FPRINTF fprintf
#endif//AAL_FPRINTF

#ifndef AL_FFLUSH
#define AL_FFLUSH fflush
#endif//AL_FFLUSH

#ifndef AAL_EXIT
extern void exit(int status);
#define AAL_EXIT exit
#endif//AAL_EXIT

#define aal_header(aal) (AAL_ASSERT((aal) != NULL), (__AAL_Header*)(aal) - 1)

#define aal_reserve(aal, expected_capacity) do {                                                                 \
  if(NULL == aal) {                                                                                              \
    __AAL_Header* header = (__AAL_Header*)AAL_REALLOC(NULL, sizeof(*(aal))*AAL_INIT_CAP + sizeof(__AAL_Header)); \
    header->count = 0;                                                                                           \
    header->capacity = AAL_INIT_CAP;                                                                             \
    (aal) = (void*)(header+1);                                                                                   \
    break;                                                                                                       \
  }                                                                                                              \
  __AAL_Header* header = aal_header(aal);                                                                        \
  while(header->capacity < expected_capacity) {                                                                  \
    header = (__AAL_Header*)AAL_REALLOC(                                                                         \
        header,                                                                                                  \
        sizeof(*(aal))*header->capacity*2 + sizeof(__AAL_Header)                                                 \
    );                                                                                                           \
    header->capacity *= 2;                                                                                       \
  }                                                                                                              \
  (aal) = (void*)(header+1);                                                                                     \
} while (0)

// #define aal_resize(al, new_size) do {                                                                         \
//   aal_reserve((al), new_size);                                                                                \
//   (al)->capacity = (new_size);                                                                                \
// } while (0)

#define aal_append(aal, x) do {                                                                                  \
  if((aal) == NULL) aal_reserve((aal), AAL_INIT_CAP);                                                            \
  __AAL_Header* header = aal_header(aal);                                                                        \
  aal_reserve((aal), header->count+1);                                                                           \
  (aal)[header->count++] = x;                                                                                    \
} while(0)

#define aal_append_many(aal, new_items, new_items_count) do {                                                    \
  if((aal) == NULL) aal_reserve((aal), AAL_INIT_CAP);                                                            \
  __AAL_Header* header = aal_header(aal);                                                                        \
  aal_reserve((aal), (header)->count + (new_items_count));                                                       \
  header = aal_header(aal);                                                                                      \
  AAL_MEMCPY(                                                                                                    \
    (aal) + header->count,                                                                                       \
    (new_items),                                                                                                 \
    (new_items_count)*sizeof(*aal)                                                                               \
  );                                                                                                             \
  header->count += (new_items_count);                                                                            \
} while (0)

#define aal_insert(aal, pos, x) do {                                                                             \
  if((aal) == NULL) aal_reserve((aal), AAL_INIT_CAP);                                                            \
  __AAL_Header* header = aal_header(aal);                                                                        \
  size_t position = ((pos) < 0) ? header->count + (pos) : (size_t)(pos);                                         \
  if(position > header->count) {                                                                                 \
    AAL_FPRINTF(                                                                                                 \
        stderr,                                                                                                  \
        "position %d out of bounds [%ld, %ld]\n",                                                                \
        (pos), -header->count, header->count                                                                     \
    );                                                                                                           \
    AAL_EXIT(1);                                                                                                     \
  }                                                                                                              \
  aal_reserve((aal), header->count+1);                                                                           \
  if(position < header->count)                                                                                   \
    AAL_MEMCPY(&(aal)[position+1], &(aal)[position], (header->count - position)*sizeof(*aal));                   \
  (aal)[position] = x;                                                                                           \
  header->count = MAX(header->count+1, position);                                                                \
} while(0)

#define aal_insert_many(aal, pos, new_items, new_items_count) do {                                               \
  if((aal) == NULL) aal_reserve((aal), AAL_INIT_CAP);                                                            \
  __AAL_Header* header = aal_header(aal);                                                                        \
  size_t position = ((pos) < 0) ? header->count + (pos) : (size_t)(pos);                                         \
  if(position > header->count) {                                                                                 \
    AAL_FPRINTF(                                                                                                 \
        stderr,                                                                                                  \
        "position %d out of bounds [%ld, %ld]\n",                                                                \
        (pos), -header->count, header->count                                                                     \
    );                                                                                                           \
    AAL_EXIT(1);                                                                                                     \
  }                                                                                                              \
  aal_reserve((aal), header->count+new_items_count);                                                             \
  if(position < header->count)                                                                                   \
    AAL_MEMCPY(&(aal)[position+new_items_count], &(aal)[position], (header->count - position)*sizeof(*aal));     \
  AAL_MEMCPY(&(aal)[position], new_items, new_items_count*sizeof(*aal));                                         \
  header->count += new_items_count;                                                                              \
} while(0)

#define aal_first(aal) (aal)[(AAL_ASSERT(aal_header(aal)->count > 0), 0)]
#define aal_last(aal)  (aal)[(AAL_ASSERT(aal_header(aal)->count > 0), aal_header(aal)->count-1)]

#define aal_pop(aal) (aal_header(aal)->count--, (aal)[aal_header(aal)->count])
#define aal_pop_many(aal, _count) (aal_header(aal)->count -= _count, (aal)[aal_header(aal)->count])

#define aal_remove_unordered(aal, pos) do {                                                                      \
  __AAL_Header *header = aal_header(aal);                                                                        \
  size_t position = ((pos) < 0) ? header->count + (pos) : (size_t)(pos);                                         \
  if(position > header->count) {                                                                                 \
    AAL_FPRINTF(                                                                                                 \
        stderr,                                                                                                  \
        "position %d out of bounds [%ld, %ld]\n",                                                                \
        (pos), -header->count, header->count                                                                     \
    );                                                                                                           \
    AAL_EXIT(1);                                                                                                     \
  }                                                                                                              \
  AAL_ASSERT(position < header->count);                                                                                 \
  (aal)[position] = aal_last(aal);                                                                                      \
  header->count--;                                                                                               \
} while(0)

#define aal_remove(aal, pos) do {                                                                                \
  if((aal) == NULL) {                                                                                            \
    aal_reserve((aal), AAL_INIT_CAP);                                                                            \
    break;                                                                                                       \
  }                                                                                                              \
  __AAL_Header* header = aal_header(aal);                                                                        \
  size_t position = ((pos) < 0) ? header->count + (pos) : (size_t)(pos);                                         \
  if(position > header->count) {                                                                                 \
    AAL_FPRINTF(                                                                                                 \
        stderr,                                                                                                  \
        "position %d out of bounds [%ld, %ld]\n",                                                                \
        (pos), -header->count, header->count                                                                     \
    );                                                                                                           \
    AAL_EXIT(1);                                                                                                     \
  }                                                                                                              \
  AAL_MEMCPY(&(aal)[position], &(aal)[position+1], (header->count - position)*sizeof(*aal));                     \
  header->count--;                                                                                               \
} while(0)

#define aal_remove_many(aal, pos, _count) do {                                                                   \
  if((aal) == NULL) {                                                                                            \
    aal_reserve((aal), AAL_INIT_CAP);                                                                            \
    break;                                                                                                       \
  }                                                                                                              \
  __AAL_Header *header = aal_header(aal);                                                                        \
  size_t position = ((pos) < 0) ? header->count + (pos) : (size_t)(pos);                                         \
  if(position > header->count) {                                                                                 \
    AAL_FPRINTF(                                                                                                 \
        stderr,                                                                                                  \
        "position %d out of bounds [%ld, %ld]\n",                                                                \
        (pos), -header->count, header->count                                                                     \
    );                                                                                                           \
    AAL_EXIT(1);                                                                                                     \
  }                                                                                                              \
  size_t quant = (header->count - position < _count) ? header->count - position : _count;                        \
  AAL_MEMCPY(&(aal)[position], &(aal)[position+_count], (header->count - position - quant)*sizeof(*aal));        \
  header->count -= _count;                                                                                       \
} while(0)

#define aal_len(aal) aal_header(aal)->count
#define aal_cap(aal) aal_header(aal)->capacity

#define aal_free(aal) do { AAL_FREE(aal_header(aal)); aal = NULL; } while(0)


#define aal_foreach(Type, it, aal)                                                                               \
  if(NULL != aal)                                                                                                \
  for(size_t it##_idx = 0; 0 == it##_idx;)                                                                       \
  for (Type *it = (aal); it##_idx < aal_len(aal); ++it##_idx, ++it)


#define aal_print(aal) do {                                                                                      \
  if(NULL == aal) { \
    AAL_FPRINTF(stdout, #aal "[0/0]: NULL\n");                                                       \
    break; \
  }\
  size_t len = aal_len(aal);                                                                                     \
  size_t cap = aal_cap(aal);                                                                                     \
  AAL_FPRINTF(stdout, #aal "[%lu/%lu]:\n", len, cap);                                                            \
  for(size_t i = 0; i < len; i++)                                                                                \
    AAL_FPRINTF(stdout, "  [%lu] = %d\n", i, aal[i]);                                                            \
} while(0)

#define aal_print_object(aal) do { \
  if(NULL == aal) { \
    AAL_FPRINTF(stdout, #aal " = NULL\n");                                                       \
    break; \
  }\
  AAL_FPRINTF(stdout, #aal " {\n  .count = %ld,\n  .capacity = %ld\n  .items = ", aal_len(aal), aal_cap(aal)); \
  aal_foreach(int, i, aal) \
    AAL_FPRINTF(stdout, "%s\n    [%ld] %d,%s", \
        0 == i_idx ? "{" : "", i_idx, *i, aal_len(aal)-1 == i_idx ? "\n  }" : "" \
    ); \
  else AAL_FPRINTF(stdout, "NULL"); \
  AAL_FPRINTF(stdout, "\n}\n"); \
} while(0)





#ifndef ARRAY
#define ARRAY(...) { __VA_ARGS__ }
#endif//ARRAY

#define aal_expect_literal(element_type, aal, expected) do { \
  element_type expected_elements[] = expected; \
  size_t expected_size = sizeof(expected_elements)/sizeof(*expected_elements); \
  AAL_ASSERT(aal_len(aal) == expected_size); \
  aal_foreach(element_type, aael_i, aal) \
    AAL_ASSERT((aal)[aael_i_idx] == expected_elements[aael_i_idx]); \
} while(0)


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__AAL_H__

