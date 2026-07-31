#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#define DECLARE_DYN_ARRAY(type)                                                \
  typedef struct {                                                             \
    type *data;                                                                \
    size_t size;                                                               \
    size_t capacity;                                                           \
  } type##_DynamicArray;                                                       \
                                                                               \
  void init_##type##_array(type##_DynamicArray *arr,                           \
                           size_t initial_capacity) {                          \
    arr->data = (type *)malloc(initial_capacity * sizeof(type));               \
    arr->size = 0;                                                             \
    arr->capacity = initial_capacity;                                          \
  }                                                                            \
                                                                               \
  void resize_##type##_array(type##_DynamicArray *arr, size_t new_capacity) {  \
    arr->data = (type *)realloc(arr->data, new_capacity * sizeof(type));       \
    arr->capacity = new_capacity;                                              \
  }                                                                            \
                                                                               \
  void add_##type##_element(type##_DynamicArray *arr, type element) {          \
    if (arr->size == arr->capacity) {                                          \
      resize_##type##_array(arr, arr->capacity * 2);                           \
    }                                                                          \
    arr->data[arr->size++] = element;                                          \
  }                                                                            \
                                                                               \
  void remove_##type##_element(type##_DynamicArray *arr) {                     \
    if (arr->size > 0) {                                                       \
      arr->size--;                                                             \
    }                                                                          \
  }                                                                            \
                                                                               \
  void free_##type##_array(type##_DynamicArray *arr) {                         \
    free(arr->data);                                                           \
    arr->data = NULL;                                                          \
    arr->size = 0;                                                             \
    arr->capacity = 0;                                                         \
  }

#define DynamicArray(type) type##_DynamicArray

typedef struct {
  void *data;
  size_t size;
  size_t capacity;
} GenericDynamicArray;

void init_array(GenericDynamicArray *arr, size_t initial_capacity) {
  arr->data = (void *)malloc(initial_capacity * sizeof(char));
  arr->size = 0;
  arr->capacity = initial_capacity;
}

void resize_array(GenericDynamicArray *arr, size_t new_capacity) {
  arr->data = (void *)realloc(arr->data, new_capacity * sizeof(char));
  arr->capacity = new_capacity;
}

void add_element(GenericDynamicArray *arr, void *element) {
  if (arr->size == arr->capacity) {
    resize_array(arr, arr->capacity * 2);
  }
  ((char *)arr->data)[arr->size++] = *(char *)element;
}

void remove_element(GenericDynamicArray *arr) {
  if (arr->size > 0) {
    arr->size--;
  }
}

void free_array(GenericDynamicArray *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
}

#define DECLARE_DYN_POINTER_ARRAY(type)                                        \
  typedef struct {                                                             \
    type **data;                                                               \
    size_t size;                                                               \
    size_t capacity;                                                           \
  } type##_DynamicPointerArray;                                                \
                                                                               \
  void init_##type##_pointer_array(type##_DynamicPointerArray *arr,            \
                                   size_t initial_capacity) {                  \
    arr->data = (type **)malloc(initial_capacity * sizeof(type *));            \
    arr->size = 0;                                                             \
    arr->capacity = initial_capacity;                                          \
  }                                                                            \
                                                                               \
  void resize_##type##_pointer_array(type##_DynamicPointerArray *arr,          \
                                     size_t new_capacity) {                    \
    arr->data = (type **)realloc(arr->data, new_capacity * sizeof(type *));    \
    arr->capacity = new_capacity;                                              \
  }                                                                            \
                                                                               \
  void add_##type##_pointer_element(type##_DynamicPointerArray *arr,           \
                                    type *element) {                           \
    if (arr->size == arr->capacity) {                                          \
      resize_##type##_pointer_array(arr, arr->capacity * 2);                   \
    }                                                                          \
    arr->data[arr->size++] = element;                                          \
  }                                                                            \
                                                                               \
  void remove_##type##_pointer_element(type##_DynamicPointerArray *arr) {      \
    if (arr->size > 0) {                                                       \
      arr->size--;                                                             \
    }                                                                          \
  }                                                                            \
                                                                               \
  void free_##type##_pointer_array(type##_DynamicPointerArray *arr) {          \
    free(arr->data);                                                           \
    arr->data = NULL;                                                          \
    arr->size = 0;                                                             \
    arr->capacity = 0;                                                         \
  }

#define DynamicPointerArray(type) type##_DynamicPointerArray

typedef struct {
  void *data;
  size_t size;
  size_t capacity;
} GenericDynamicPointerArray;

void init_pointer_array(GenericDynamicPointerArray *arr, size_t initial_capacity) {
  arr->data = (void **)malloc(initial_capacity * sizeof(void *));
  arr->size = 0;
  arr->capacity = initial_capacity;
}

void resize_pointer_array(GenericDynamicPointerArray *arr, size_t new_capacity) {
  arr->data = (void **)realloc(arr->data, new_capacity * sizeof(void *));
  arr->capacity = new_capacity;
}

void add_pointer_element(GenericDynamicPointerArray *arr, void **element) {
  if (arr->size == arr->capacity) {
    resize_pointer_array(arr, arr->capacity * 2);
  }
  ((char *)arr->data)[arr->size++] = *(char *)element;
}

void remove_pointer_element(GenericDynamicPointerArray *arr) {
  if (arr->size > 0) {
    arr->size--;
  }
}

void free_pointer_array(GenericDynamicPointerArray *arr) {
  for(size_t i = 0; i < arr->size; i++) free(((char**)arr->data)[i]);
  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
}

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // DYN_ARRAY_H

