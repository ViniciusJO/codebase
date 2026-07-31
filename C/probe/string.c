#define ALLOCATOR_LIBC
#include "codebase/allocator.h"

typedef unsigned int usize_t;
typedef signed int isize_t;

typedef struct {
  usize_t len;
  char* buff;
} string;

typedef struct { string a; string b; } string_split_return;

static string whitespaces = (string){ 6, "\t\n\v\f\r " };

#ifndef STRING_DEF
#define STRING_DEF
#endif//STRING_DEF

STRING_DEF usize_t string_cstrlen(char* s);


#define STR(S) (string){ .len = string_cstrlen(S), .buff = (S) }
#define STR_SPREAD(S) (int)(S).len, (S).buff

extern void abort(void);

static inline string ___build_string(char *s) { return STR(s); }
static inline string ___string_echo(string s) { return s; }
static inline string ___string_unrechable(string s) { (void)s; abort(); }

// static inline char*  ___build_cstring(string s) { return s.buff; }
// static inline char*  ___cstring_echo(char* s) { return s; }
// static inline char* ___cstring_unrechable(char* s) { (void)s; abort(); }

#define STR_RESOLVE(S) _Generic((S), char*: ___build_string, string: ___string_echo, default: ___string_unrechable)(S)

#define string_split(s, delim) _string_split(STR_RESOLVE(s), STR_RESOLVE(delim))
#define string_eq(s1,s2) _string_eq(STR_RESOLVE(s1), STR_RESOLVE(s2))
#define string_starts_with(s1,s2) _string_starts_with(STR_RESOLVE(s1), STR_RESOLVE(s2))
#define string_find(s1,s2) _string_find(STR_RESOLVE(s1), STR_RESOLVE(s2))
#define string_contains(s1,s2) _string_contains(STR_RESOLVE(s1), STR_RESOLVE(s2))
#define string_trim(s) _string_trim(STR_RESOLVE(s))
#define string_trim_left(s) _string_trim_left(STR_RESOLVE(s))
#define string_trim_right(s) _string_trim_right(STR_RESOLVE(s))
// #define string_cstr(S) _Generic((S), char*: ___cstring_echo, string: ___build_cstring, default: ___cstring_unrechable)(S)


STRING_DEF string_split_return _string_split(string s, string delim);
STRING_DEF string _string_trim(string s);
STRING_DEF string _string_trim_left(string s);
STRING_DEF string _string_trim_right(string s);
STRING_DEF isize_t _string_find(string s1, string s2);
STRING_DEF _Bool _string_eq(string s1, string s2);
STRING_DEF _Bool _string_starts_with(string s1, string s2);
STRING_DEF _Bool _string_contains(string s1, string s2);
STRING_DEF _Bool _string_contains(string s1, string s2);

char* string_cstr(Allocator alloc, string s);





STRING_DEF usize_t string_cstrlen(char* s) {
  usize_t sz = 0;
  while(*(s+sz) != '\0') sz++;
  return sz;
}

STRING_DEF _Bool _string_eq(string s1, string s2) {
  if(s1.len != s2.len) return 0;
  for(usize_t i = 0; i < s1.len; i++) if(s1.buff[i] != s2.buff[i]) return 0;
  return 1;
}

STRING_DEF _Bool _string_starts_with(string s1, string s2) {
  if(s1.len < s2.len) return 0;
  for(usize_t i = 0; i < s2.len; i++) if(s1.buff[i] != s2.buff[i]) return 0;
  return 1;
}

STRING_DEF isize_t _string_find(string s1, string s2) {
  if(s1.len >= s2.len) {
    for(usize_t i = 0; i < s1.len - s2.len; i++) {
      usize_t j = 0;
      while(j < s2.len && s1.buff[i+j] == s2.buff[j]) j++;
      if(j == s2.len) return i;
    }
  }
  return -1;
}

STRING_DEF string_split_return _string_split(string s, string delim) {
  isize_t found = string_find(s, delim);
  return (string_split_return){
    .a = (string){ .buff = s.buff, .len = found},
    .b = (string){ .buff = s.buff+found+delim.len, .len = s.len - found - delim.len },
  };
}

STRING_DEF _Bool _string_contains(string s1, string s2) {
  if(s1.len == 0 || s2.len == 0) return 0;
  isize_t ret = _string_find(s1,s2);
  return ret >= 0;
}

STRING_DEF string _string_trim(string s) {
  return string_trim_left(string_trim_right(s));
}

STRING_DEF string _string_trim_left(string s) {
  for(usize_t i = 0; i < s.len; i++) {
    string c = (string){1, &s.buff[i]};
    if(string_contains(whitespaces, c)) s.buff++;
    else break;
  }
  return s;
}

STRING_DEF string _string_trim_right(string s) {
  for(isize_t i = s.len - 1; i >= 0; i--) {
    string c = (string){1, &s.buff[i]};
    if(string_contains(whitespaces, c)) s.len--;
    else break;
  }
  return s;
}

void ___memcpy(void* dest, const void * const src, usize_t size) {
  for(usize_t i = 0; i < size; i++)
    ((char*)dest)[i] = ((char*)src)[i];
}

char* string_cstr(Allocator alloc, string s) {
  char* ret = alloc.alloc(s.len+1, ALLIGN_1);
  ___memcpy(ret,s.buff,s.len);
  ret[s.len] = '\0';
  return ret;
}

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char* s1 = "nessa string tem algo";
  char* s2 = " ";
  string s = {0};
  isize_t x = string_find(s1, s2);
  (void)x;
  if(0) return 1;
  printf("\"%s\" contains \"%s\" ? %s\n", s1, s2, string_contains(s1, s2) ? "true" : "false");
  char* example = "  \nex ampl e\t\r\n";
  s = string_trim(example);
  printf("RES: \"%s\"-> trim -> \"%.*s\"\n", example, STR_SPREAD(s));
  printf("\neq: %s\n", string_eq(s, "ex ampl e") ? "true" : "false");

  string_split_return ret = string_split(s1, "ng");

  printf("\n\nret_a: \"%.*s\"\nret_b: \"%.*s\"\n", STR_SPREAD(ret.a), STR_SPREAD(ret.b));

  char* sec = string_cstr(libc_allocator, string_trim_left(ret.b));
  printf("\n\nsec: \"%s\"\n\n", sec);
  libc_allocator.free(sec, ret.b.len+1, ALLIGN_1);

  // TODO: test trim (full, left, right)

  return 0;
}

