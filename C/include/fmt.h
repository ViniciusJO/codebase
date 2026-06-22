#ifndef __FMT_H__
#define __FMT_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>

/* helper: expand cv-qualified variants */
#define QUALIFIED_TYPES(T, F) \
    T: F, /*\
    const T: F, \
    volatile T: F, \
    const volatile T: F,*/

#define VARIANTS(T, F) \
  QUALIFIED_TYPES(T, F) \
  QUALIFIED_TYPES(_Atomic T, F) \

#define FMT(x) _Generic(x,             \
    VARIANTS(_Bool, "%d")                \
                                         \
    VARIANTS(char, "%c")               \
    VARIANTS(signed char, "%hhd")        \
    VARIANTS(unsigned char, "%hhu")      \
                                         \
    VARIANTS(short, "%hd")               \
    VARIANTS(unsigned short, "%hu")      \
                                         \
    VARIANTS(int, "%d")                  \
    VARIANTS(unsigned int, "%u")         \
                                         \
    VARIANTS(long, "%ld")                \
    VARIANTS(unsigned long, "%lu")       \
                                         \
    VARIANTS(long long, "%lld")          \
    VARIANTS(unsigned long long, "%llu") \
                                         \
    VARIANTS(float, "%f")                \
    VARIANTS(double, "%f")               \
    VARIANTS(long double, "%Lf")         \
                                         \
    VARIANTS(char*, "%s")                \
                                         \
    default: "%p"                        \
)

__attribute__((format(printf, 1, 2)))
char *format(const char *fmt, ...);

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FMT_H__

#define FMT_IMPLEMENTATIONS
#ifdef FMT_IMPLEMENTATIONS
#ifndef __FMT_IMP__
#define __FMT_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdarg.h>

__attribute__((format(printf, 1, 2)))
char *format(const char *fmt, ...) {
  char *buf;
  size_t buflen;
  FILE *out = open_memstream(&buf, &buflen);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(out, fmt, ap);
  va_end(ap);
  fclose(out);
  return buf;
}

__attribute__((format(printf, 2, 3)))
void fmt_append(char *buf, const char *fmt, ...) {
  char *new_buf;
  size_t buflen;
  FILE *out = open_memstream(&new_buf, &buflen);

  fprintf(out, "%s", buf);
  free(buf);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(out, fmt, ap);
  va_end(ap);
  fclose(out);
  buf = new_buf;
}


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FMT_IMP__
#undef FMT_IMPLEMENTATIONS
#endif//FMT_IMPLEMENTATIONS

