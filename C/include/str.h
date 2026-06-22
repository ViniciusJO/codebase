/* \file str.h
 *  Single-header string library for C — no libc required.
 *
 *  Provides three string abstractions:
 *    - Zero-terminated C string utilities  (str_ prefix)
 *    - String views — non-owning slices    (sv_  prefix)
 *    - String builders — dynamic char arrays (sb_ prefix)
 *
 *  Usage
 *  -----
 *  In exactly ONE translation unit define STR_IMPLEMENTATION before including:
 *
 *      #define STR_IMPLEMENTATION
 *      #include "str.h"
 *
 *  All other translation units just include normally:
 *
 *      #include "str.h"
 *
 *  Allocator
 *  ---------
 *  The library never calls malloc/free directly. Every operation that
 *  allocates (i.e., the string builder) receives a tStrAllocator. Provide
 *  your own by filling the three function pointers and an optional context.
 */

#ifndef __STR_H__
#define __STR_H__

#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

typedef unsigned long str_size_t;
typedef int str_bool_t;
typedef __SIZE_TYPE__ str_size_t;

#define STR_TRUE 1
#define STR_FALSE 0
#define STR_NULL ((void *)0)

/* Sentinel returned by search functions when nothing is found. */
#define STR_NPOS ((str_size_t) - 1)


// Allocate size bytes. Return STR_NULL on failure.
typedef void *(*str_alloc_fn_t)(void *ctx, str_size_t size);
// Resize a previous allocation from old_size to new_size. Return STR_NULL on failure (original pointer remains valid).
typedef void *(*str_realloc_fn_t)(void *ctx, void *ptr, str_size_t old_size, str_size_t new_size);
// Free a previous allocation.
typedef void (*str_free_fn_t)(void *ctx, void *ptr, str_size_t size);

typedef struct StrAllocator {
  void *ctx; // Opaque context forwarded to every call.
  str_alloc_fn_t alloc;
  str_realloc_fn_t realloc;
  str_free_fn_t free;
} StrAllocator;


// Non-owning view into a string
typedef struct StrView {
  const char
      *data;      // Pointer to first character. May be STR_NULL when len == 0.
  str_size_t len; // Number of characters (not counting any terminator).
} StrView;

// Dynamic character array list. Owns its memory via StrAllocator.
typedef struct StrBuilder {
  char *data;     // Heap buffer.
  str_size_t len; // Current string length (not counting terminator).
  str_size_t cap; // Total allocated bytes (always >= len + 1 when data != STR_NULL).
  StrAllocator allocator;
} StrBuilder;


// Return the number of characters before the null terminator.
str_size_t str_len(const char *s);

// Lexicographic comparison. Returns <0, 0, or >0.
int str_cmp(const char *a, const char *b);

// Lexicographic comparison of at most `n` characters.
int str_ncmp(const char *a, const char *b, str_size_t n);

// Copy the content of `src` into `dst`, writing at most `dst_cap` bytes including the null terminator.
// Returns the number of characters written (not counting the null terminator).
// `dst` is always null-terminated when `dst_cap` > 0.
str_size_t str_copy(char *dst, const char *src, str_size_t dst_cap);

// Find first occurrence of `c` in `s`.
// Returns a pointer to the character, or `STR_NULL` if not found.
// Passing '\0' as `c` returns a pointer to the terminator.
const char *str_find_char(const char *s, char c);

// Find last occurrence of `c` in `s`.
// Returns a pointer to the character, or `STR_NULL` if not found.
const char *str_rfind_char(const char *s, char c);

// Find first occurrence of `needle` inside `haystack`.
// Returns a pointer to the start of the match, or `STR_NULL`.
const char *str_find(const char *haystack, const char *needle);

// Return `STR_TRUE` when `s` is `STR_NULL` or its first byte is '\0'.
str_bool_t str_is_empty(const char *s);


// SV - String view

// Construct a view from the null-terminated string `s`.
StrView sv_from_cstr(const char *s);

// Construct a view from the explicit `data` pointer and length `len`.
StrView sv_from_parts(const char *data, str_size_t len);

// Return the sub-view [ `start`, `end` ). Clamps to `sv.len` silently.
StrView sv_slice(StrView sv, str_size_t start, str_size_t end);

// Return the sub-view [ `start`, `end` ).
StrView sv_cstr_slice(const char* str, str_size_t start, str_size_t end);

// Lexicographic comparison.
// Returns: <0, 0, or >0.
int sv_cmp(StrView a, StrView b);

// Return `STR_TRUE` when both views have the same length and content.
str_bool_t sv_eq(StrView a, StrView b);

// Return `STR_TRUE` when `sv` equals the null-terminated string `s`.
str_bool_t sv_eq_cstr(StrView sv, const char *s);

// Return `STR_TRUE` when `sv` begins with `prefix`.
str_bool_t sv_starts_with(StrView sv, StrView prefix);

// Return `STR_TRUE` when `sv` ends with `suffix`.
str_bool_t sv_ends_with(StrView sv, StrView suffix);

// Find first occurrence of `c`. Returns its index or `STR_NPOS`.
str_size_t sv_find_char(StrView sv, char c);

// Find last occurrence of `c`. Returns its index or `STR_NPOS`.
str_size_t sv_rfind_char(StrView sv, char c);

// Find first occurrence of `needle`. Returns start index or `STR_NPOS`.
str_size_t sv_find(StrView sv, StrView needle);

// Strip leading ASCII whitespace (space, tab, CR, LF, FF, VT).
StrView sv_trim_left(StrView sv);

// Strip trailing ASCII whitespace.
StrView sv_trim_right(StrView sv);

// Strip leading and trailing ASCII whitespace.
StrView sv_trim(StrView sv);

// Split on the first occurrence of `delim`.
// 
//  Returns the token before the delimiter. If `rest` is not `STR_NULL` it is
//  set to the remainder after the delimiter. When the delimiter is absent the
//  entire view is returned and `*rest` is set to an empty view. Suitable for
//  iterating over delimited fields:
// 
//      tStrView line = sv_from_cstr("a,b,c");
//      tStrView rest, token;
//      while (!sv_is_empty(line)) {
//          token = sv_split_next(line, ',', &rest);
//          line  = rest;
//      }
// 
StrView sv_split_next(StrView sv, char delim, StrView *rest);

// Return `STR_TRUE` when the view has zero length.
str_bool_t sv_is_empty(StrView sv);

// Parse an unsigned decimal integer from `sv` into `out`.
// Returns `STR_FALSE` when the view is empty or contains non-digit
// characters. `out` is unchanged on failure.
str_bool_t sv_to_uint(StrView sv, unsigned long long *out);

// Parse a signed decimal integer from `sv` into `out`.
// An optional leading '-' is accepted. Returns `STR_FALSE` on failure.
str_bool_t sv_to_int(StrView sv, long long *out);


// SB - String builder

// Initialise a builder with the given `allocator` and initial capacity.
// `initial_cap` may be 0; allocation is deferred to first append.
// Returns a zeroed builder on allocation failure (check .data).
StrBuilder sb_init(StrAllocator allocator, str_size_t initial_cap);

// Release heap memory. Sets all fields to zero.
void sb_free(StrBuilder *sb);

// Append a single character. Returns `STR_FALSE` on OOM.
str_bool_t sb_append_char(StrBuilder *sb, char c);

// Append a null-terminated string. Returns `STR_FALSE` on OOM.
str_bool_t sb_append_cstr(StrBuilder *sb, const char *s);

// Append a string view. Returns `STR_FALSE` on OOM.
// Safe to call with a view into the builder itself.
str_bool_t sb_append_sv(StrBuilder *sb, StrView sv);

// Append the base-10 decimal representation of `n`.
str_bool_t sb_append_uint(StrBuilder *sb, unsigned long long n);

// Append the base-10 decimal representation of `n` (signed).
str_bool_t sb_append_int(StrBuilder *sb, long long n);

// Append the hexadecimal representation of `n` (no "0x" prefix).
// Pass STR_TRUE for uppercase digits (A-F), STR_FALSE for lowercase.
str_bool_t sb_append_hex(StrBuilder *sb, unsigned long long n, str_bool_t uppercase);

// Append the content formated content to `sb`.
// The appendend content have at max `maxlen` bytes.
// When `maxlen` is `0` do not change `sb`;
// When `maxlen` is `-1` do not care for `maxlen` and append all the content.
str_bool_t sb_append_fmt(StrBuilder *sb, str_size_t maxlen, const char * const fmt, ...);

// Append the content formated (printf style) content to `sb`.
// The appendend content have at max `maxlen` bytes.
// When `maxlen` is `0` do not change `sb`;
// When `maxlen` is `-1` do not care for `maxlen` and append all the content.
__attribute__((format(printf, 3, 4)))
str_bool_t sb_append_fmtf(StrBuilder *sb, str_size_t maxlen, const char * const fmt, ...);

// Reset length to zero, keeping the current allocation.
void sb_clear(StrBuilder *sb);

// Return a non-owning view of the current content.
StrView sb_view(const StrBuilder *sb);

// Null-terminate the buffer in place and return a pointer to it.
// The builder remains valid; the returned pointer is stable until
// the next mutating call. Returns `STR_NULL` on OOM.
char *sb_build_cstr(StrBuilder *sb);



#define STR_IMPLEMENTATION
#ifdef STR_IMPLEMENTATION

/* -------------------------------------------------------------------------
 * Internal memory primitives — static (TU-private), no libc.
 *
 * A compiler may still lower these loops to SIMD/intrinsic sequences;
 * that is fine. The guarantee is that no libc symbol is referenced.
 * ------------------------------------------------------------------------- */

static void str__mem_copy(void *dst, const void *src, str_size_t n) {
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;
  str_size_t i;
  for (i = 0; i < n; ++i)
    d[i] = s[i];
}

/* Handles overlapping regions correctly. Used when src may alias dst. */
static void str__mem_move(void *dst, const void *src, str_size_t n) {
  unsigned char *d = (unsigned char *)dst;
  const unsigned char *s = (const unsigned char *)src;
  if (d < s) {
    /* Non-overlapping forward: delegate to the fast copy path. */
    str__mem_copy(dst, src, n);
  } else if (d > s) {
    str_size_t i = n;
    while (i--)
      d[i] = s[i];
  }
}

static void str__mem_set(void *dst, int c, str_size_t n) {
  unsigned char *d = (unsigned char *)dst;
  str_size_t i;
  for (i = 0; i < n; ++i)
    d[i] = (unsigned char)c;
}

static str_bool_t str__mem_eq(const void *a, const void *b, str_size_t n) {
  const unsigned char *pa = (const unsigned char *)a;
  const unsigned char *pb = (const unsigned char *)b;
  str_size_t i;
  for (i = 0; i < n; ++i)
    if (pa[i] != pb[i])
      return STR_FALSE;
  return STR_TRUE;
}

static str_bool_t str__is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' ||
         c == '\v';
}

/* Grow builder so that cap >= min_cap.  Growth strategy: double from current,
 * starting at 16 when the buffer is still unallocated. */
static str_bool_t str__sb_grow(StrBuilder *sb, str_size_t min_cap) {
  str_size_t new_cap;
  char *new_data;

  if (sb->cap >= min_cap)
    return STR_TRUE;

  new_cap = sb->cap ? sb->cap * 2 : 16;
  while (new_cap < min_cap)
    new_cap *= 2;

  if (sb->data == STR_NULL) {
    new_data = (char *)sb->allocator.alloc(sb->allocator.ctx, new_cap);
    if (new_data != STR_NULL)
      str__mem_set(new_data, 0, new_cap);
  } else {
    new_data = (char *)sb->allocator.realloc(sb->allocator.ctx, sb->data,
                                             sb->cap, new_cap);
  }

  if (new_data == STR_NULL)
    return STR_FALSE;

  sb->data = new_data;
  sb->cap = new_cap;
  return STR_TRUE;
}

/* -------------------------------------------------------------------------
 * Zero-terminated string implementations
 * ------------------------------------------------------------------------- */

str_size_t str_len(const char *s) {
  str_size_t n = 0;
  if (s == STR_NULL) return 0;
  while (s[n] != '\0') ++n;
  return n;
}

int str_cmp(const char *a, const char *b) {
  if (a == STR_NULL && b == STR_NULL)
    return 0;
  if (a == STR_NULL)
    return -1;
  if (b == STR_NULL)
    return 1;

  while (*a && (*a == *b)) {
    ++a;
    ++b;
  }
  return (unsigned char)*a - (unsigned char)*b;
}

int str_ncmp(const char *a, const char *b, str_size_t n) {
  str_size_t i;
  if (n == 0)
    return 0;
  if (a == STR_NULL && b == STR_NULL)
    return 0;
  if (a == STR_NULL)
    return -1;
  if (b == STR_NULL)
    return 1;

  for (i = 0; i < n - 1 && a[i] && a[i] == b[i]; ++i)
    ;
  return (unsigned char)a[i] - (unsigned char)b[i];
}

str_size_t str_copy(char *dst, const char *src, str_size_t dst_cap) {
  str_size_t i = 0;
  if (dst_cap == 0 || dst == STR_NULL)
    return 0;
  if (src != STR_NULL) {
    while (i < dst_cap - 1 && src[i] != '\0') {
      dst[i] = src[i];
      ++i;
    }
  }
  dst[i] = '\0';
  return i;
}

const char *str_find_char(const char *s, char c) {
  if (s == STR_NULL)
    return STR_NULL;
  while (*s != '\0') {
    if (*s == c)
      return s;
    ++s;
  }
  /* Requesting the null terminator itself is well-defined. */
  if (c == '\0')
    return s;
  return STR_NULL;
}

const char *str_rfind_char(const char *s, char c) {
  const char *last = STR_NULL;
  if (s == STR_NULL)
    return STR_NULL;
  while (*s != '\0') {
    if (*s == c)
      last = s;
    ++s;
  }
  if (c == '\0')
    return s;
  return last;
}

const char *str_find(const char *haystack, const char *needle) {
  str_size_t nlen;
  if (haystack == STR_NULL || needle == STR_NULL)
    return STR_NULL;
  nlen = str_len(needle);
  if (nlen == 0)
    return haystack;
  while (*haystack != '\0') {
    if (*haystack == *needle && str__mem_eq(haystack, needle, nlen))
      return haystack;
    ++haystack;
  }
  return STR_NULL;
}

str_bool_t str_is_empty(const char *s) { return s == STR_NULL || *s == '\0'; }

/* -------------------------------------------------------------------------
 * String view implementations
 * ------------------------------------------------------------------------- */

StrView sv_from_cstr(const char *s) {
  StrView sv;
  sv.data = s;
  sv.len = str_len(s);
  return sv;
}

StrView sv_from_parts(const char *data, str_size_t len) {
  StrView sv;
  sv.data = data;
  sv.len = len;
  return sv;
}

StrView sv_slice(StrView sv, str_size_t start, str_size_t end) {
  StrView result;
  if (start > sv.len)
    start = sv.len;
  if (end > sv.len)
    end = sv.len;
  if (end < start)
    end = start;
  result.data = sv.data + start;
  result.len = end - start;
  return result;
}

StrView sv_cstr_slice(const char* str, str_size_t start, str_size_t end) {
  StrView result = sv_from_cstr(str);
  return sv_slice(result, start, end);
}

int sv_cmp(StrView a, StrView b) {
  str_size_t min_len = a.len < b.len ? a.len : b.len;
  str_size_t i;
  for (i = 0; i < min_len; ++i) {
    int diff = (unsigned char)a.data[i] - (unsigned char)b.data[i];
    if (diff != 0)
      return diff;
  }
  if (a.len < b.len)
    return -1;
  if (a.len > b.len)
    return 1;
  return 0;
}

str_bool_t sv_eq(StrView a, StrView b) {
  if (a.len != b.len)
    return STR_FALSE;
  if (a.len == 0)
    return STR_TRUE;
  return str__mem_eq(a.data, b.data, a.len);
}

str_bool_t sv_eq_cstr(StrView sv, const char *s) {
  return sv_eq(sv, sv_from_cstr(s));
}

str_bool_t sv_starts_with(StrView sv, StrView prefix) {
  if (prefix.len > sv.len)
    return STR_FALSE;
  if (prefix.len == 0)
    return STR_TRUE;
  return str__mem_eq(sv.data, prefix.data, prefix.len);
}

str_bool_t sv_ends_with(StrView sv, StrView suffix) {
  if (suffix.len > sv.len)
    return STR_FALSE;
  if (suffix.len == 0)
    return STR_TRUE;
  return str__mem_eq(sv.data + sv.len - suffix.len, suffix.data, suffix.len);
}

str_size_t sv_find_char(StrView sv, char c) {
  str_size_t i;
  for (i = 0; i < sv.len; ++i)
    if (sv.data[i] == c)
      return i;
  return STR_NPOS;
}

str_size_t sv_rfind_char(StrView sv, char c) {
  str_size_t i = sv.len;
  while (i--) {
    if (sv.data[i] == c)
      return i;
  }
  return STR_NPOS;
}

str_size_t sv_find(StrView sv, StrView needle) {
  str_size_t i;
  if (needle.len == 0)
    return 0;
  if (needle.len > sv.len)
    return STR_NPOS;
  for (i = 0; i <= sv.len - needle.len; ++i) {
    if (sv.data[i] == needle.data[0] &&
        str__mem_eq(sv.data + i, needle.data, needle.len))
      return i;
  }
  return STR_NPOS;
}

StrView sv_trim_left(StrView sv) {
  while (sv.len > 0 && str__is_space(sv.data[0])) {
    ++sv.data;
    --sv.len;
  }
  return sv;
}

StrView sv_trim_right(StrView sv) {
  while (sv.len > 0 && str__is_space(sv.data[sv.len - 1]))
    --sv.len;
  return sv;
}

StrView sv_trim(StrView sv) { return sv_trim_left(sv_trim_right(sv)); }

StrView sv_split_next(StrView sv, char delim, StrView *rest) {
  str_size_t pos = sv_find_char(sv, delim);
  StrView token;

  if (pos == STR_NPOS) {
    token.data = sv.data;
    token.len = sv.len;
    if (rest != STR_NULL) {
      rest->data = sv.data + sv.len;
      rest->len = 0;
    }
  } else {
    token.data = sv.data;
    token.len = pos;
    if (rest != STR_NULL) {
      rest->data = sv.data + pos + 1;
      rest->len = sv.len - pos - 1;
    }
  }
  return token;
}

str_bool_t sv_is_empty(StrView sv) { return sv.len == 0; }

str_bool_t sv_to_uint(StrView sv, unsigned long long *out) {
  unsigned long long value = 0;
  str_size_t i;

  if (sv.len == 0)
    return STR_FALSE;

  for (i = 0; i < sv.len; ++i) {
    char c = sv.data[i];
    if (c < '0' || c > '9')
      return STR_FALSE;
    value = value * 10 + (unsigned long long)(c - '0');
  }
  *out = value;
  return STR_TRUE;
}

str_bool_t sv_to_int(StrView sv, long long *out) {
  unsigned long long uval;
  str_bool_t negative = STR_FALSE;

  if (sv.len == 0)
    return STR_FALSE;

  if (sv.data[0] == '-') {
    negative = STR_TRUE;
    sv.data++;
    sv.len--;
    if (sv.len == 0)
      return STR_FALSE;
  }

  if (!sv_to_uint(sv, &uval))
    return STR_FALSE;

  *out = negative ? -(long long)uval : (long long)uval;
  return STR_TRUE;
}

/* -------------------------------------------------------------------------
 * String builder implementations
 * ------------------------------------------------------------------------- */

StrBuilder sb_init(StrAllocator allocator, str_size_t initial_cap) {
  StrBuilder sb;
  sb.data = STR_NULL;
  sb.len = 0;
  sb.cap = 0;
  sb.allocator = allocator;

  if (initial_cap > 0)
    str__sb_grow(&sb, initial_cap);

  return sb;
}

void sb_free(StrBuilder *sb) {
  if (sb->data != STR_NULL)
    sb->allocator.free(sb->allocator.ctx, sb->data, sb->cap);
  sb->data = STR_NULL;
  sb->len = 0;
  sb->cap = 0;
}

str_bool_t sb_append_char(StrBuilder *sb, char c) {
  /* Reserve len + 1 char + 1 spare byte for the null sb_build_cstr writes. */
  if (!str__sb_grow(sb, sb->len + 2))
    return STR_FALSE;
  sb->data[sb->len++] = c;
  return STR_TRUE;
}

str_bool_t sb_append_sv(StrBuilder *sb, StrView sv) {
  str_size_t n = sv.len;
  str_size_t offset = 0;
  str_bool_t self_view = STR_FALSE;

  if (n == 0)
    return STR_TRUE;

  /* Detect aliasing: sv may be a slice of the builder's own buffer.
   * Record the byte offset now — realloc inside str__sb_grow may move the
   * buffer, making the original sv.data pointer stale. */
  if (sb->data != STR_NULL && sv.data >= sb->data &&
      sv.data < sb->data + sb->len) {
    self_view = STR_TRUE;
    offset = (str_size_t)(sv.data - sb->data);
  }

  if (!str__sb_grow(sb, sb->len + n + 1))
    return STR_FALSE;

  str__mem_move(sb->data + sb->len, self_view ? sb->data + offset : sv.data, n);
  sb->len += n;
  return STR_TRUE;
}

str_bool_t sb_append_cstr(StrBuilder *sb, const char *s) {
  return sb_append_sv(sb, sv_from_cstr(s));
}

str_bool_t sb_append_uint(StrBuilder *sb, unsigned long long n) {
  /* Maximum digits for a 64-bit unsigned decimal value. */
  char buf[20];
  str_size_t i = 0;

  if (n == 0) {
    buf[i++] = '0';
  } else {
    while (n > 0) {
      buf[i++] = (char)('0' + (int)(n % 10));
      n /= 10;
    }
    /* Reverse the digit string. */
    str_size_t lo = 0, hi = i - 1;
    while (lo < hi) {
      char tmp = buf[lo];
      buf[lo] = buf[hi];
      buf[hi] = tmp;
      ++lo;
      --hi;
    }
  }
  return sb_append_sv(sb, sv_from_parts(buf, i));
}

str_bool_t sb_append_int(StrBuilder *sb, long long n) {
  if (n < 0) {
    if (!sb_append_char(sb, '-'))
      return STR_FALSE;
    /* Promote to unsigned before negation to avoid signed overflow on
     * LLONG_MIN: -(LLONG_MIN) is undefined, but the unsigned cast is not. */
    return sb_append_uint(sb, (unsigned long long)(-(n + 1)) + 1ULL);
  }
  return sb_append_uint(sb, (unsigned long long)n);
}

str_bool_t sb_append_hex(StrBuilder *sb, unsigned long long n,
                         str_bool_t uppercase) {
  const char *digits_lo = "0123456789abcdef";
  const char *digits_hi = "0123456789ABCDEF";
  const char *digits = uppercase ? digits_hi : digits_lo;
  char buf[16];
  str_size_t i = 0;

  if (n == 0) {
    buf[i++] = '0';
  } else {
    while (n > 0) {
      buf[i++] = digits[n & 0xFULL];
      n >>= 4;
    }
    str_size_t lo = 0, hi = i - 1;
    while (lo < hi) {
      char tmp = buf[lo];
      buf[lo] = buf[hi];
      buf[hi] = tmp;
      ++lo;
      --hi;
    }
  }
  return sb_append_sv(sb, sv_from_parts(buf, i));
}

#include <stdarg.h>
#include <stdbool.h>

str_bool_t sb_append_fmt(StrBuilder *sb, str_size_t maxlen, const char * const fmt, ...) {
  (void)sb;
  (void)maxlen;
  (void)fmt;
  return true;
}

enum SbFmtElementType { SB_TYPE_INT, SB_TYPE_FLOAT, SB_TYPE_CHAR, SB_TYPE_STRING, SB_TYPE_ANY };
enum SbFmtElementAllignment { SB_ALLIGN_LEFT, SB_ALLIGN_CENTER, SB_ALLIGN_RIGHT, SB_ALLIGN_NONE };

typedef struct {
  enum SbFmtElementType type;
  enum SbFmtElementAllignment allignment;
  str_size_t padding;
  char padding_element;
  bool dyn_padding;
  str_size_t precision;
} SbFmtElement;

typedef struct {
  SbFmtElement *items;
  str_size_t capacity;
  str_size_t count;
} SbFmtList;

#define str_al_assert(COND) { \
  (void)(COND); \
}

// TODO: fix call to sb->allocator.realloc
#define sb_al_reserve(sb, al, expected_capacity) do {                                                     \
  if ((expected_capacity) > (al)->capacity) {                                                             \
    if ((al)->capacity == 0) (al)->capacity = 32;                                                         \
    while ((expected_capacity) > (al)->capacity) (al)->capacity *= 2;                                     \
    (al)->items = (__typeof__((al)->items))(sb)->allocator.realloc(NULL, (al)->items, 0, (al)->capacity); \
    str_al_assert((al)->items != NULL);                                                                   \
  }                                                                                                       \
} while (0)

#define STRUCT(...) { __VA_ARGS__ }
#define sb_al_append(sb, al, item) do {                                                                   \
  sb_al_reserve((sb), (al), (al)->count + 1);                                                             \
  (al)->items[(al)->count++] = item;                                                                      \
} while (0)

#define sb_al_free(sb, al) do {                                                                           \
  (sb)->allocator.free(STR_NULL, (al)->items, (al)->capacity);                                            \
  (al)->items = NULL;                                                                                     \
} while(0)

str_bool_t sb_append_fmtf(StrBuilder *sb, str_size_t maxlen, const char * const fmt, ...) {
  SbFmtList fmt_list = {0};

  for(str_size_t i = 0; i < str_len(fmt); i++)
    if('%' == fmt[i]) {
      SbFmtElement e = {
        .type = SB_TYPE_ANY,
        .allignment = SB_ALLIGN_NONE,
        .padding = 0,
        .padding_element = 0,
        .dyn_padding = false,
        .precision = 0
      };
      while(true) {
        switch(fmt[++i]) {
          case 's': { e.type = SB_TYPE_STRING; } goto exit_loop;
          case 'c': { e.type = SB_TYPE_CHAR; } goto exit_loop;
          case 'f': { e.type = SB_TYPE_FLOAT; } goto exit_loop;
          case 'i':
          case 'd': { e.type = SB_TYPE_INT; } goto exit_loop;
          case '0': { e.padding_element = '0'; } break;
          case '-': { e.allignment = SB_ALLIGN_RIGHT; } break;
          case '.': {
            switch(fmt[++i]) {
              case '*': { e.dyn_padding = true; } break;
              default: {
                if(fmt[i] >= '0' && fmt[i] <= '9') {
                  str_size_t j = i + 1;
                  while(fmt[j] >= '0' && fmt[j] <= '9') j++;
                  StrView number_rep = sv_from_cstr(fmt+i);
                  number_rep.len = j - i;
                  sv_to_uint(number_rep, (unsigned long long*)&e.padding);
                }
              }
            }
          } break;
        }
      }
      exit_loop: 
      sb_al_append(sb, &fmt_list, e);
    }

  va_list args;
  va_start(args, fmt);

  // int total = 0;
  // for (int i = 0; i < count; i++) {
  //   total += va_arg(args, int);
  // }

  va_end(args);

  sb_al_free(sb, &fmt_list);

  return true;
}

void sb_clear(StrBuilder *sb) { sb->len = 0; }

StrView sb_view(const StrBuilder *sb) {
  StrView sv;
  sv.data = sb->data;
  sv.len = sb->len;
  return sv;
}

char *sb_build_cstr(StrBuilder *sb) {
  if (!str__sb_grow(sb, sb->len + 1))
    return STR_NULL;
  sb->data[sb->len] = '\0';
  return sb->data;
}

#endif // STR_IMPLEMENTATION

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__STR_H__
