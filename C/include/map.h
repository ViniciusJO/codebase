/**
 * Document:
 *  - creation
 *  - insertion
 *  - retrieval
 *  - deletion
 *
 * Corners:
 *  - reinsertion
 *  
 */

#ifndef __MAP_H__
#define __MAP_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

// TODO: Allocator

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "./ansi.h"
#include "./al.h"

typedef size_t (*hash_func)(char*key);

#ifndef MAP_HASH_FUNC
#define MAP_HASH_FUNC __hash_internal
#endif//MAP_HASH_FUNC

#ifndef MAP_ATTRIBUTES
// TODO: remove default static inline
#define MAP_ATTRIBUTES
#endif//MAP_ATTRIBUTES

#ifndef MAP_SIZE
#define MAP_SIZE 256
#endif//MAP_SIZE

typedef struct {
  char* key;
  void* value;
} map_entry;
typedef AL_DEFINITION(map_entry) map_entry_list;
typedef map_entry_list map[MAP_SIZE];

// #define map_insert_(m,k,v) map_insert(m,(map_entry){.key=k,.value=v})
#define ME(k,v) (map_entry){ .key = k, .value = v }
MAP_ATTRIBUTES map_entry* map_insert(map m, map_entry e);
MAP_ATTRIBUTES map_entry* map_get(map m, char *key);
MAP_ATTRIBUTES void map_delete(map m, char *key);
MAP_ATTRIBUTES void map_dump(map m, int (*printf_fn)(const char*,...));


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__MAP_H__

// #define MAP_IMPLEMENTATIONS
#ifdef MAP_IMPLEMENTATIONS
#ifndef __MAP_IMP__
#define __MAP_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#define MAP_MIN(A,B) ((A) < (B) ? (A) : (B))


MAP_ATTRIBUTES size_t __hash_internal(char* key) {
  const uint32_t FNV_prime = 16777619;
  uint32_t hash = 2166136261;
const size_t n = strlen(key);
  for(size_t i = 0; i < n; i++) {
    hash ^= key[i];
    hash *= FNV_prime;
  }
  return hash;
}

MAP_ATTRIBUTES map_entry* map_insert(map m, map_entry e) {
  const size_t h = MAP_HASH_FUNC(e.key);
  map_entry_list* el = m + (h%MAP_SIZE);
  al_foreach(i, el)
    if(strncmp(i->key, e.key, MAP_MIN(strlen(e.key), strlen(i->key))) == 0)
      return NULL;
  al_append(el, e);
  return el->items + (el->count - 1);
}

MAP_ATTRIBUTES map_entry* map_get(map m, char *key) {
  const size_t h = MAP_HASH_FUNC(key);
  map_entry_list* el = m + (h%MAP_SIZE);
  if(el->count == 0) return NULL;
  al_foreach(i, el) 
    if(strncmp(i->key, key, MAP_MIN(strlen(key), strlen(i->key))) == 0)
      return i;
  return NULL;
}

MAP_ATTRIBUTES void map_delete(map m, char *key) {
  const size_t h = MAP_HASH_FUNC(key);
  map_entry_list* el = m + (h%MAP_SIZE);
  int32_t pos = -1;
  al_foreach(i, el) 
    if(strncmp(i->key, key, MAP_MIN(strlen(key), strlen(i->key))) == 0)
       pos = i_idx;
  if(pos >= 0) al_remove_unordered(el, pos);
}

MAP_ATTRIBUTES void map_dump(map m, int (*printf_fn)(const char*,...)) {
  printf_fn("map: \n");
  size_t count = 0;
  for(size_t i = 0; i < MAP_SIZE; i++) if(m[i].count > 0) al_foreach(e, &m[i]) {
    printf_fn("map[" FG_GREEN "\"%s\"" FG_RESET "]: " FG_CYAN "%p (%llu) " FG_RED "<hash: %d>\n" FG_RESET, e->key, e->value, e->value, MAP_HASH_FUNC(e->key) % MAP_SIZE);
    count++;
  }
  if(count == 0) printf_fn(CURSOR_UP(1) CLEAR_LINE "map: EMPTY\n");
}

MAP_ATTRIBUTES void map_clear(map m) {
  for(size_t i = 0; i < MAP_SIZE; i++)
    if(m[i].items != NULL)
      al_free(m + i);
}


#undef MAP_MIN


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__MAP_IMP__
#undef MAP_IMPLEMENTATIONS
#endif//MAP_IMPLEMENTATIONS

