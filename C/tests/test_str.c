/*! \file test_str.c
 *  \brief Comprehensive tests for str.h.
 *
 *  Build and run:
 *      cc -std=c99 -Wall -Wextra -o test_str test_str.c && ./test_str
 */

#define STR_IMPLEMENTATION
#include "codebase/str.h"


// LIBC
extern int   printf  (const char * const fmt, ...);
extern void* malloc  (str_size_t __size);
extern void* realloc (void *__ptr, str_size_t __size);
extern void  free    (void *__ptr);


/* ==========================================================================
 * Minimal test framework
 * ========================================================================== */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *expr, const char *file, int line) {
  if (cond) {
    g_pass++;
  } else {
    g_fail++;
    printf("FAIL: [%s:%d]  %s\n", file, line, expr);
  }
}

#define CHECK(expr) check(!!(expr), #expr, __FILE__, __LINE__)

static void group(const char *name) { printf("\n-- %s\n", name); }

/* ==========================================================================
 * Allocators
 * ========================================================================== */

/* System allocator — wraps malloc/realloc/free. */
static void *sys_alloc(void *ctx, str_size_t size) {
  (void)ctx;
  return malloc((str_size_t)size);
}

static void *sys_realloc(void *ctx, void *ptr, str_size_t old_size,
                         str_size_t new_size) {
  (void)ctx;
  (void)old_size;
  return realloc(ptr, (str_size_t)new_size);
}

static void sys_free(void *ctx, void *ptr, str_size_t size) {
  (void)ctx;
  (void)size;
  free(ptr);
}

static StrAllocator make_sys_allocator(void) {
  StrAllocator a;
  a.ctx = STR_NULL;
  a.alloc = sys_alloc;
  a.realloc = sys_realloc;
  a.free = sys_free;
  return a;
}

/* Failing allocator — always returns STR_NULL.  Used for OOM path tests. */
static void *fail_alloc(void *ctx, str_size_t size) {
  (void)ctx;
  (void)size;
  return STR_NULL;
}

static void *fail_realloc(void *ctx, void *ptr, str_size_t old_size, str_size_t new_size) {
  (void)ctx;
  (void)ptr;
  (void)old_size;
  (void)new_size;
  return STR_NULL;
}

static void fail_free(void *ctx, void *ptr, str_size_t size) {
  (void)ctx;
  (void)ptr;
  (void)size;
}

static StrAllocator make_fail_allocator(void) {
  StrAllocator a;
  a.ctx = STR_NULL;
  a.alloc = fail_alloc;
  a.realloc = fail_realloc;
  a.free = fail_free;
  return a;
}

/* Counting allocator — fails after a configurable number of successful
 * allocations.  Delegates to the system allocator when the budget is > 0. */
typedef struct CountAllocCtx {
  int budget; /* Decremented on each call. Fails when it reaches 0. */
} CountAllocCtx;

static void *count_alloc(void *ctx, str_size_t size) {
  CountAllocCtx *c = (CountAllocCtx *)ctx;
  if (c->budget <= 0)
    return STR_NULL;
  c->budget--;
  return malloc((str_size_t)size);
}

static void *count_realloc(void *ctx, void *ptr, str_size_t old_size, str_size_t new_size) {
  CountAllocCtx *c = (CountAllocCtx *)ctx;
  (void)old_size;
  if (c->budget <= 0)
    return STR_NULL;
  c->budget--;
  return realloc(ptr, (str_size_t)new_size);
}

static void count_free(void *ctx, void *ptr, str_size_t size) {
  (void)ctx;
  (void)size;
  free(ptr);
}

static StrAllocator make_count_allocator(CountAllocCtx *ctx) {
  StrAllocator a;
  a.ctx = ctx;
  a.alloc = count_alloc;
  a.realloc = count_realloc;
  a.free = count_free;
  return a;
}

static int view_matches(StrView sv, const char *expected) {
  str_size_t i, exp_len = 0;
  while (expected[exp_len])
    exp_len++;
  if (sv.len != exp_len)
    return 0;
  for (i = 0; i < sv.len; ++i)
    if (sv.data[i] != expected[i])
      return 0;
  return 1;
}

/* ==========================================================================
 * str_len
 * ========================================================================== */

static void test_str_len(void) {
  group("str_len");
  CHECK(str_len("hello") == 5);
  CHECK(str_len("") == 0);
  CHECK(str_len("a") == 1);
  CHECK(str_len(STR_NULL) == 0);
  CHECK(str_len("abc\0def") == 3); /* stops at first '\0' */
  CHECK(str_len("12345678901234567890") == 20);
}

/* ==========================================================================
 * str_cmp
 * ========================================================================== */
static void test_str_cmp(void) {
  group("str_cmp");
  CHECK(str_cmp("abc", "abc") == 0);
  CHECK(str_cmp("", "") == 0);
  CHECK(str_cmp("abc", "abd") < 0);
  CHECK(str_cmp("abd", "abc") > 0);
  CHECK(str_cmp("abc", "ab") > 0); /* longer > shorter prefix */
  CHECK(str_cmp("ab", "abc") < 0); /* shorter < longer prefix */
  CHECK(str_cmp("A", "a") < 0);    /* uppercase < lowercase in ASCII */
  CHECK(str_cmp("z", "a") > 0);
  CHECK(str_cmp(STR_NULL, STR_NULL) == 0);
  CHECK(str_cmp(STR_NULL, "a") < 0);
  CHECK(str_cmp("a", STR_NULL) > 0);
}

/* ==========================================================================
 * str_ncmp
 * ========================================================================== */
static void test_str_ncmp(void) {
  group("str_ncmp");
  CHECK(str_ncmp("abc", "abc", 3) == 0);
  CHECK(str_ncmp("abcX", "abcY", 3) == 0); /* mismatch after n */
  CHECK(str_ncmp("abcX", "abcY", 4) < 0);  /* X < Y */
  CHECK(str_ncmp("abd", "abc", 3) > 0);
  CHECK(str_ncmp("abc", "abc", 0) == 0); /* n=0 always equal */
  CHECK(str_ncmp("abc", "abc", 1) == 0); /* n=1 first char equal */
  CHECK(str_ncmp("b", "a", 1) > 0);
  CHECK(str_ncmp(STR_NULL, STR_NULL, 5) == 0);
  CHECK(str_ncmp(STR_NULL, "a", 1) < 0);
  CHECK(str_ncmp("a", STR_NULL, 1) > 0);
}

/* ==========================================================================
 * str_copy
 * ========================================================================== */
static void test_str_copy(void) {
  char buf[16];
  str_size_t n;

  group("str_copy");

  /* Normal copy. */
  n = str_copy(buf, "hello", sizeof(buf));
  CHECK(n == 5);
  CHECK(buf[0] == 'h' && buf[4] == 'o' && buf[5] == '\0');

  /* Truncation: dst_cap=4 means at most 3 chars + '\0'. */
  n = str_copy(buf, "hello", 4);
  CHECK(n == 3);
  CHECK(buf[3] == '\0');

  /* dst_cap=1 writes only the null terminator. */
  buf[0] = 'X';
  n = str_copy(buf, "hello", 1);
  CHECK(n == 0);
  CHECK(buf[0] == '\0');

  /* dst_cap=0: nothing written, return 0. */
  n = str_copy(buf, "hello", 0);
  CHECK(n == 0);

  /* NULL src: produces empty string. */
  n = str_copy(buf, STR_NULL, sizeof(buf));
  CHECK(n == 0);
  CHECK(buf[0] == '\0');

  /* Exact fit: src length == dst_cap - 1. */
  n = str_copy(buf, "abcd", 5);
  CHECK(n == 4);
  CHECK(buf[4] == '\0');

  /* Empty src. */
  n = str_copy(buf, "", sizeof(buf));
  CHECK(n == 0);
  CHECK(buf[0] == '\0');
}

/* ==========================================================================
 * str_find_char
 * ========================================================================== */
static void test_str_find_char(void) {
  const char *s = "abcabc";
  const char *p;

  group("str_find_char");

  /* First occurrence. */
  p = str_find_char(s, 'b');
  CHECK(p != STR_NULL && p == s + 1);

  /* Character at position 0. */
  p = str_find_char(s, 'a');
  CHECK(p != STR_NULL && p == s);

  /* Character at last position. */
  p = str_find_char(s, 'c');
  CHECK(p != STR_NULL && p == s + 2);

  /* Not found. */
  p = str_find_char(s, 'z');
  CHECK(p == STR_NULL);

  /* Null terminator: returns pointer to '\0'. */
  p = str_find_char(s, '\0');
  CHECK(p != STR_NULL && *p == '\0' && p == s + 6);

  /* NULL input. */
  CHECK(str_find_char(STR_NULL, 'a') == STR_NULL);

  /* Single-char string. */
  p = str_find_char("x", 'x');
  CHECK(p != STR_NULL && *p == 'x');
}

/* ==========================================================================
 * str_rfind_char
 * ========================================================================== */
static void test_str_rfind_char(void) {
  const char *s = "abcabc";
  const char *p;

  group("str_rfind_char");

  /* Last occurrence. */
  p = str_rfind_char(s, 'b');
  CHECK(p != STR_NULL && p == s + 4);

  p = str_rfind_char(s, 'a');
  CHECK(p != STR_NULL && p == s + 3);

  p = str_rfind_char(s, 'c');
  CHECK(p != STR_NULL && p == s + 5);

  /* Not found. */
  p = str_rfind_char(s, 'z');
  CHECK(p == STR_NULL);

  /* Null terminator. */
  p = str_rfind_char(s, '\0');
  CHECK(p != STR_NULL && *p == '\0');

  /* NULL input. */
  CHECK(str_rfind_char(STR_NULL, 'a') == STR_NULL);

  /* Single-char string — first and last are the same. */
  p = str_rfind_char("x", 'x');
  CHECK(p != STR_NULL && *p == 'x');

  /* Unique character: only one occurrence. */
  p = str_rfind_char("abcde", 'e');
  CHECK(p != STR_NULL && p[0] == 'e' && p[1] == '\0');
}

/* ==========================================================================
 * str_find
 * ========================================================================== */
static void test_str_find(void) {
  const char *haystack = "hello world hello";
  const char *p;

  group("str_find");

  /* Found at the start. */
  p = str_find(haystack, "hello");
  CHECK(p == haystack);

  /* Found in the middle. */
  p = str_find(haystack, "world");
  CHECK(p == haystack + 6);

  /* Second occurrence not returned (finds first). */
  p = str_find(haystack, "hello");
  CHECK(p == haystack);

  /* Not found. */
  p = str_find(haystack, "xyz");
  CHECK(p == STR_NULL);

  /* Empty needle: returns haystack. */
  p = str_find(haystack, "");
  CHECK(p == haystack);

  /* Needle longer than haystack. */
  p = str_find("hi", "hello world");
  CHECK(p == STR_NULL);

  /* Exact match. */
  p = str_find("abc", "abc");
  CHECK(p != STR_NULL && *p == 'a');

  /* Needle at the very end. */
  p = str_find("foobar", "bar");
  CHECK(p != STR_NULL && p[0] == 'b');

  /* NULL inputs. */
  CHECK(str_find(STR_NULL, "x") == STR_NULL);
  CHECK(str_find("x", STR_NULL) == STR_NULL);

  /* Partial overlap that should NOT match. */
  p = str_find("aaab", "aab");
  CHECK(p != STR_NULL && p == &"aaab"[1]);
}

/* ==========================================================================
 * str_is_empty
 * ========================================================================== */
static void test_str_is_empty(void) {
  group("str_is_empty");
  CHECK(str_is_empty(STR_NULL) == STR_TRUE);
  CHECK(str_is_empty("") == STR_TRUE);
  CHECK(str_is_empty("a") == STR_FALSE);
  CHECK(str_is_empty(" ") == STR_FALSE); /* space is not empty */
  CHECK(str_is_empty("0") == STR_FALSE);
}

/* ==========================================================================
 * sv_from_cstr
 * ========================================================================== */
static void test_sv_from_cstr(void) {
  StrView sv;

  group("sv_from_cstr");

  sv = sv_from_cstr("hello");
  CHECK(sv.len == 5 && sv.data[0] == 'h');

  sv = sv_from_cstr("");
  CHECK(sv.len == 0);

  sv = sv_from_cstr(STR_NULL);
  CHECK(sv.len == 0 && sv.data == STR_NULL);
}

/* ==========================================================================
 * sv_from_parts
 * ========================================================================== */
static void test_sv_from_parts(void) {
  const char *buf = "hello world";
  StrView sv;

  group("sv_from_parts");

  sv = sv_from_parts(buf, 5);
  CHECK(sv.len == 5 && sv.data == buf);

  /* Zero length — data pointer is preserved. */
  sv = sv_from_parts(buf, 0);
  CHECK(sv.len == 0 && sv.data == buf);

  /* Slice into the middle of a buffer. */
  sv = sv_from_parts(buf + 6, 5);
  CHECK(sv.len == 5 && sv.data[0] == 'w');
}

/* ==========================================================================
 * sv_slice
 * ========================================================================== */
static void test_sv_slice(void) {
  StrView base = sv_from_cstr("hello");
  StrView s;

  group("sv_slice");

  /* Normal sub-range. */
  s = sv_slice(base, 1, 4);
  CHECK(s.len == 3 && s.data[0] == 'e' && s.data[2] == 'l');

  /* Full range. */
  s = sv_slice(base, 0, 5);
  CHECK(s.len == 5 && s.data == base.data);

  /* Empty sub-range [2,2). */
  s = sv_slice(base, 2, 2);
  CHECK(s.len == 0);

  /* start > end: clamped to empty. */
  s = sv_slice(base, 4, 2);
  CHECK(s.len == 0);

  /* start > len: clamped to len. */
  s = sv_slice(base, 10, 12);
  CHECK(s.len == 0);

  /* end > len: clamped to len. */
  s = sv_slice(base, 0, 100);
  CHECK(s.len == 5);

  /* Single character. */
  s = sv_slice(base, 0, 1);
  CHECK(s.len == 1 && s.data[0] == 'h');

  /* Last character. */
  s = sv_slice(base, 4, 5);
  CHECK(s.len == 1 && s.data[0] == 'o');
}

/* ==========================================================================
 * sv_cstr_slice
 * ========================================================================== */
static void test_sv_cstr_slice(void) {
  const char * const base = "hello";
  StrView s;

  group("sv_cstr_slice");

  /* Normal sub-range. */
  s = sv_cstr_slice(base, 1, 4);
  CHECK(s.len == 3 && s.data[0] == 'e' && s.data[2] == 'l');

  /* Full range. */
  s = sv_cstr_slice(base, 0, 5);
  CHECK(s.len == 5 && s.data == base);

  /* Empty sub-range [2,2). */
  s = sv_cstr_slice(base, 2, 2);
  CHECK(s.len == 0);

  /* start > end: clamped to empty. */
  s = sv_cstr_slice(base, 4, 2);
  CHECK(s.len == 0);

  /* start > len: clamped to len. */
  s = sv_cstr_slice(base, 10, 12);
  CHECK(s.len == 0);

  /* end > len: clamped to len. */
  s = sv_cstr_slice(base, 0, 100);
  CHECK(s.len == 5);

  /* Single character. */
  s = sv_cstr_slice(base, 0, 1);
  CHECK(s.len == 1 && s.data[0] == 'h');

  /* Last character. */
  s = sv_cstr_slice(base, 4, 5);
  CHECK(s.len == 1 && s.data[0] == 'o');
}

/* ==========================================================================
 * sv_cmp
 * ========================================================================== */
static void test_sv_cmp(void) {
  group("sv_cmp");
  CHECK(sv_cmp(sv_from_cstr("abc"), sv_from_cstr("abc")) == 0);
  CHECK(sv_cmp(sv_from_cstr(""), sv_from_cstr("")) == 0);
  CHECK(sv_cmp(sv_from_cstr("abc"), sv_from_cstr("abd")) < 0);
  CHECK(sv_cmp(sv_from_cstr("abd"), sv_from_cstr("abc")) > 0);

  /* Prefix relationship: shorter < longer when prefix matches. */
  CHECK(sv_cmp(sv_from_cstr("ab"), sv_from_cstr("abc")) < 0);
  CHECK(sv_cmp(sv_from_cstr("abc"), sv_from_cstr("ab")) > 0);

  /* Views from parts (no null terminator present). */
  CHECK(sv_cmp(sv_from_parts("abcX", 3), sv_from_parts("abcY", 3)) == 0);
  CHECK(sv_cmp(sv_from_parts("abcX", 4), sv_from_parts("abcY", 4)) < 0);
}

/* ==========================================================================
 * sv_eq
 * ========================================================================== */
static void test_sv_eq(void) {
  group("sv_eq");
  CHECK(sv_eq(sv_from_cstr("hello"), sv_from_cstr("hello")) == STR_TRUE);
  CHECK(sv_eq(sv_from_cstr(""), sv_from_cstr("")) == STR_TRUE);
  CHECK(sv_eq(sv_from_cstr("hello"), sv_from_cstr("world")) == STR_FALSE);

  /* Same prefix, different length. */
  CHECK(sv_eq(sv_from_cstr("abc"), sv_from_cstr("ab")) == STR_FALSE);

  /* Views of non-null-terminated buffers. */
  CHECK(sv_eq(sv_from_parts("abcX", 3), sv_from_parts("abcY", 3)) == STR_TRUE);
  CHECK(sv_eq(sv_from_parts("abcX", 4), sv_from_parts("abcY", 4)) == STR_FALSE);
}

/* ==========================================================================
 * sv_eq_cstr
 * ========================================================================== */
static void test_sv_eq_cstr(void) {
  group("sv_eq_cstr");
  CHECK(sv_eq_cstr(sv_from_cstr("hello"), "hello") == STR_TRUE);
  CHECK(sv_eq_cstr(sv_from_cstr(""), "") == STR_TRUE);
  CHECK(sv_eq_cstr(sv_from_cstr("hello"), "world") == STR_FALSE);
  CHECK(sv_eq_cstr(sv_from_cstr("hel"), "hello") == STR_FALSE);

  /* View from parts: length-bounded, does not see the 'X'. */
  CHECK(sv_eq_cstr(sv_from_parts("helloX", 5), "hello") == STR_TRUE);
}

/* ==========================================================================
 * sv_starts_with
 * ========================================================================== */
static void test_sv_starts_with(void) {
  StrView sv = sv_from_cstr("foobar");

  group("sv_starts_with");
  CHECK(sv_starts_with(sv, sv_from_cstr("foo")) == STR_TRUE);
  CHECK(sv_starts_with(sv, sv_from_cstr("foobar")) == STR_TRUE); /* exact */
  CHECK(sv_starts_with(sv, sv_from_cstr("")) == STR_TRUE); /* empty prefix */
  CHECK(sv_starts_with(sv, sv_from_cstr("bar")) == STR_FALSE);
  CHECK(sv_starts_with(sv, sv_from_cstr("foobarx")) ==
        STR_FALSE);                                         /* prefix longer */
  CHECK(sv_starts_with(sv, sv_from_cstr("f")) == STR_TRUE); /* single char */
  CHECK(sv_starts_with(sv, sv_from_cstr("x")) == STR_FALSE);
}

/* ==========================================================================
 * sv_ends_with
 * ========================================================================== */
static void test_sv_ends_with(void) {
  StrView sv = sv_from_cstr("foobar");

  group("sv_ends_with");
  CHECK(sv_ends_with(sv, sv_from_cstr("bar")) == STR_TRUE);
  CHECK(sv_ends_with(sv, sv_from_cstr("foobar")) == STR_TRUE); /* exact */
  CHECK(sv_ends_with(sv, sv_from_cstr("")) == STR_TRUE); /* empty suffix */
  CHECK(sv_ends_with(sv, sv_from_cstr("foo")) == STR_FALSE);
  CHECK(sv_ends_with(sv, sv_from_cstr("xfoobar")) ==
        STR_FALSE);                                       /* suffix longer */
  CHECK(sv_ends_with(sv, sv_from_cstr("r")) == STR_TRUE); /* single char */
  CHECK(sv_ends_with(sv, sv_from_cstr("x")) == STR_FALSE);
}

/* ==========================================================================
 * sv_find_char
 * ========================================================================== */
static void test_sv_find_char(void) {
  StrView sv = sv_from_cstr("abcabc");

  group("sv_find_char");
  CHECK(sv_find_char(sv, 'a') == 0); /* first occurrence */
  CHECK(sv_find_char(sv, 'b') == 1);
  CHECK(sv_find_char(sv, 'c') == 2);
  CHECK(sv_find_char(sv, 'z') == STR_NPOS); /* not found */

  /* Empty view: always STR_NPOS. */
  CHECK(sv_find_char(sv_from_parts("abc", 0), 'a') == STR_NPOS);

  /* View from parts: bounded search, does not see trailing chars. */
  CHECK(sv_find_char(sv_from_parts("abcX", 3), 'X') == STR_NPOS);
  CHECK(sv_find_char(sv_from_parts("abcX", 4), 'X') == 3);
}

/* ==========================================================================
 * sv_rfind_char
 * ========================================================================== */
static void test_sv_rfind_char(void) {
  StrView sv = sv_from_cstr("abcabc");

  group("sv_rfind_char");
  CHECK(sv_rfind_char(sv, 'a') == 3); /* last occurrence */
  CHECK(sv_rfind_char(sv, 'b') == 4);
  CHECK(sv_rfind_char(sv, 'c') == 5);
  CHECK(sv_rfind_char(sv, 'z') == STR_NPOS);

  /* Empty view. */
  CHECK(sv_rfind_char(sv_from_parts("abc", 0), 'a') == STR_NPOS);

  /* Single character. */
  CHECK(sv_rfind_char(sv_from_cstr("x"), 'x') == 0);
  CHECK(sv_rfind_char(sv_from_cstr("x"), 'y') == STR_NPOS);
}

/* ==========================================================================
 * sv_find
 * ========================================================================== */
static void test_sv_find(void) {
  StrView sv = sv_from_cstr("hello world hello");

  group("sv_find");

  /* First occurrence. */
  CHECK(sv_find(sv, sv_from_cstr("hello")) == 0);
  CHECK(sv_find(sv, sv_from_cstr("world")) == 6);

  /* Not found. */
  CHECK(sv_find(sv, sv_from_cstr("xyz")) == STR_NPOS);

  /* Empty needle: always returns 0. */
  CHECK(sv_find(sv, sv_from_cstr("")) == 0);
  CHECK(sv_find(sv_from_cstr(""), sv_from_cstr("")) == 0);

  /* Needle longer than haystack. */
  CHECK(sv_find(sv_from_cstr("hi"), sv_from_cstr("hello world")) == STR_NPOS);

  /* Needle equals haystack exactly. */
  CHECK(sv_find(sv_from_cstr("abc"), sv_from_cstr("abc")) == 0);

  /* Needle at the end. */
  CHECK(sv_find(sv_from_cstr("foobar"), sv_from_cstr("bar")) == 3);

  /* Partial overlap: "aaab" should find "aab" at index 1. */
  CHECK(sv_find(sv_from_cstr("aaab"), sv_from_cstr("aab")) == 1);

  /* Single character needle. */
  CHECK(sv_find(sv_from_cstr("abc"), sv_from_cstr("b")) == 1);
}

/* ==========================================================================
 * sv_trim_left
 * ========================================================================== */
static void test_sv_trim_left(void) {
  group("sv_trim_left");
  CHECK(view_matches(sv_trim_left(sv_from_cstr("  hello")), "hello"));
  CHECK(view_matches(sv_trim_left(sv_from_cstr("\t hello")), "hello"));
  CHECK(view_matches(sv_trim_left(sv_from_cstr("\n\rhello")), "hello"));
  CHECK(view_matches(sv_trim_left(sv_from_cstr("\f\vhello")), "hello"));
  CHECK(view_matches(sv_trim_left(sv_from_cstr("hello")), "hello")); /* no-op */
  CHECK(view_matches(sv_trim_left(sv_from_cstr("hello  ")),
                     "hello  "));                    /* only left */
  CHECK(sv_trim_left(sv_from_cstr("   ")).len == 0); /* all whitespace */
  CHECK(sv_trim_left(sv_from_cstr("")).len == 0);    /* empty */
}

/* ==========================================================================
 * sv_trim_right
 * ========================================================================== */
static void test_sv_trim_right(void) {
  group("sv_trim_right");
  CHECK(view_matches(sv_trim_right(sv_from_cstr("hello  ")), "hello"));
  CHECK(view_matches(sv_trim_right(sv_from_cstr("hello\t")), "hello"));
  CHECK(view_matches(sv_trim_right(sv_from_cstr("hello\r\n")), "hello"));
  CHECK( view_matches(sv_trim_right(sv_from_cstr("hello")), "hello")); /* no-op */
  CHECK(view_matches(sv_trim_right(sv_from_cstr("  hello")), "  hello")); /* only right */
  CHECK(sv_trim_right(sv_from_cstr("   ")).len == 0); /* all whitespace */
  CHECK(sv_trim_right(sv_from_cstr("")).len == 0); /* empty */
}

/* ==========================================================================
 * sv_trim
 * ========================================================================== */
static void test_sv_trim(void) {
  group("sv_trim");
  CHECK(view_matches(sv_trim(sv_from_cstr("  hello world  ")), "hello world"));
  CHECK(view_matches(sv_trim(sv_from_cstr("hello")), "hello")); /* no-op */
  CHECK(view_matches(sv_trim(sv_from_cstr("  hello")), "hello"));
  CHECK(view_matches(sv_trim(sv_from_cstr("hello  ")), "hello"));
  CHECK(sv_trim(sv_from_cstr("   ")).len == 0); /* all whitespace -> empty */
  CHECK(sv_trim(sv_from_cstr("")).len == 0);    /* empty -> empty */

  /* Internal whitespace is preserved. */
  {
    StrView t = sv_trim(sv_from_cstr("  a b  "));
    CHECK(t.len == 3 && t.data[1] == ' ');
  }
}

/* ==========================================================================
 * sv_split_next
 * ========================================================================== */
static void test_sv_split_next(void) {
  StrView line, rest, tok;

  group("sv_split_next");

  /* Basic three-field split. */
  line = sv_from_cstr("a,bb,ccc");
  tok = sv_split_next(line, ',', &rest);
  CHECK(tok.len == 1 && tok.data[0] == 'a');
  line = rest;
  tok = sv_split_next(line, ',', &rest);
  CHECK(tok.len == 2);
  line = rest;
  tok = sv_split_next(line, ',', &rest);
  CHECK(tok.len == 3 && rest.len == 0);

  /* No delimiter: returns whole view, rest is empty. */
  tok = sv_split_next(sv_from_cstr("hello"), ',', &rest);
  CHECK(tok.len == 5 && rest.len == 0);

  /* Leading delimiter: first token is empty. */
  tok = sv_split_next(sv_from_cstr(",b"), ',', &rest);
  CHECK(tok.len == 0 && rest.len == 1 && rest.data[0] == 'b');

  /* Trailing delimiter: last call yields empty token, rest is empty. */
  line = sv_from_cstr("a,");
  tok = sv_split_next(line, ',', &rest);
  CHECK(tok.len == 1);
  tok = sv_split_next(rest, ',', &rest);
  CHECK(tok.len == 0 && rest.len == 0);

  /* Consecutive delimiters: yields empty tokens. */
  tok = sv_split_next(sv_from_cstr("a,,b"), ',', &rest);
  CHECK(tok.len == 1);
  tok = sv_split_next(rest, ',', &rest);
  CHECK(tok.len == 0); /* empty field between commas */

  /* Single-char input == delimiter. */
  tok = sv_split_next(sv_from_cstr(","), ',', &rest);
  CHECK(tok.len == 0 && rest.len == 0);

  /* rest pointer may be STR_NULL. */
  tok = sv_split_next(sv_from_cstr("a,b"), ',', STR_NULL);
  CHECK(tok.len == 1 && tok.data[0] == 'a');

  /* Iterative pattern counts correct number of tokens. */
  {
    int count = 0;
    line = sv_from_cstr("one:two:three:four");
    while (!sv_is_empty(line)) {
      tok = sv_split_next(line, ':', &rest);
      line = rest;
      count++;
    }
    CHECK(count == 4);
  }

  /* Different delimiter: space. */
  tok = sv_split_next(sv_from_cstr("hello world"), ' ', &rest);
  CHECK(tok.len == 5 && rest.len == 5);
}

/* ==========================================================================
 * sv_is_empty
 * ========================================================================== */
static void test_sv_is_empty(void) {
  group("sv_is_empty");
  CHECK(sv_is_empty(sv_from_cstr("")) == STR_TRUE);
  CHECK(sv_is_empty(sv_from_parts("x", 0)) == STR_TRUE);
  CHECK(sv_is_empty(sv_from_cstr("a")) == STR_FALSE);
  CHECK(sv_is_empty(sv_from_cstr(" ")) == STR_FALSE);
}

/* ==========================================================================
 * sv_to_uint
 * ========================================================================== */
static void test_sv_to_uint(void) {
  unsigned long long v;

  group("sv_to_uint");

  CHECK(sv_to_uint(sv_from_cstr("0"), &v) == STR_TRUE && v == 0ULL);
  CHECK(sv_to_uint(sv_from_cstr("1"), &v) == STR_TRUE && v == 1ULL);
  CHECK(sv_to_uint(sv_from_cstr("12345"), &v) == STR_TRUE && v == 12345ULL);
  CHECK(sv_to_uint(sv_from_cstr("18446744073709551615"), &v) == STR_TRUE &&
        v == 18446744073709551615ULL); /* ULLONG_MAX */

  /* Leading zeros are accepted as plain digits. */
  CHECK(sv_to_uint(sv_from_cstr("007"), &v) == STR_TRUE && v == 7ULL);

  /* Failures: non-digit characters. */
  CHECK(sv_to_uint(sv_from_cstr(""), &v) == STR_FALSE);
  CHECK(sv_to_uint(sv_from_cstr("12a"), &v) == STR_FALSE);
  CHECK(sv_to_uint(sv_from_cstr("-1"), &v) == STR_FALSE);
  CHECK(sv_to_uint(sv_from_cstr(" 1"), &v) == STR_FALSE);

  /* Failure does not clobber *out. */
  v = 42ULL;
  sv_to_uint(sv_from_cstr("bad"), &v);
  CHECK(v == 42ULL);
}

/* ==========================================================================
 * sv_to_int
 * ========================================================================== */
static void test_sv_to_int(void) {
  long long v;

  group("sv_to_int");

  CHECK(sv_to_int(sv_from_cstr("0"), &v) == STR_TRUE && v == 0LL);
  CHECK(sv_to_int(sv_from_cstr("99"), &v) == STR_TRUE && v == 99LL);
  CHECK(sv_to_int(sv_from_cstr("-1"), &v) == STR_TRUE && v == -1LL);
  CHECK(sv_to_int(sv_from_cstr("-42"), &v) == STR_TRUE && v == -42LL);

  /* LLONG_MAX = 9223372036854775807 */
  CHECK(sv_to_int(sv_from_cstr("9223372036854775807"), &v) == STR_TRUE && v == 9223372036854775807LL);

  /* LLONG_MIN = -9223372036854775808 */
  CHECK(sv_to_int(sv_from_cstr("-9223372036854775808"), &v) == STR_TRUE && v == (-9223372036854775807LL - 1LL));

  /* Failures. */
  CHECK(sv_to_int(sv_from_cstr(""), &v) == STR_FALSE);
  CHECK(sv_to_int(sv_from_cstr("-"), &v) == STR_FALSE); /* lone minus */
  CHECK(sv_to_int(sv_from_cstr("1a"), &v) == STR_FALSE);
  CHECK(sv_to_int(sv_from_cstr(" 1"), &v) == STR_FALSE);

  /* Failure does not clobber *out. */
  v = 7LL;
  sv_to_int(sv_from_cstr("bad"), &v);
  CHECK(v == 7LL);
}

/* ==========================================================================
 * sb_init / sb_free
 * ========================================================================== */
static void test_sb_init_free(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb;

  group("sb_init / sb_free");

  /* Zero initial cap: defers allocation. */
  sb = sb_init(alloc, 0);
  CHECK(sb.data == STR_NULL && sb.len == 0 && sb.cap == 0);
  sb_free(&sb);
  CHECK(sb.data == STR_NULL && sb.len == 0 && sb.cap == 0); /* zeroed */

  /* Non-zero initial cap: allocates immediately. */
  sb = sb_init(alloc, 32);
  CHECK(sb.data != STR_NULL && sb.cap >= 32 && sb.len == 0);
  sb_free(&sb);

  /* sb_free on an already-freed (zeroed) builder is safe. */
  sb_free(&sb);
  CHECK(sb.data == STR_NULL);

  /* OOM on init: returns zeroed builder. */
  {
    StrAllocator fail = make_fail_allocator();
    sb = sb_init(fail, 16);
    CHECK(sb.data == STR_NULL && sb.len == 0 && sb.cap == 0);
  }
}

/* ==========================================================================
 * sb_append_char
 * ========================================================================== */
static void test_sb_append_char(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_char");

  CHECK(sb_append_char(&sb, 'h') == STR_TRUE);
  CHECK(sb_append_char(&sb, 'i') == STR_TRUE);
  CHECK(sb.len == 2);
  CHECK(view_matches(sb_view(&sb), "hi"));

  /* Appending many characters forces multiple growth cycles. */
  {
    int i;
    sb_clear(&sb);
    for (i = 0; i < 100; ++i)
      CHECK(sb_append_char(&sb, 'x') == STR_TRUE);
    CHECK(sb.len == 100 && sb.cap >= 101);
  }

  sb_free(&sb);

  /* OOM: returns STR_FALSE. */
  {
    StrAllocator fail = make_fail_allocator();
    StrBuilder fb = sb_init(fail, 0);
    CHECK(sb_append_char(&fb, 'a') == STR_FALSE);
    CHECK(fb.len == 0);
  }
}

/* ==========================================================================
 * sb_append_cstr
 * ========================================================================== */
static void test_sb_append_cstr(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_cstr");

  CHECK(sb_append_cstr(&sb, "hello") == STR_TRUE);
  CHECK(sb.len == 5);

  CHECK(sb_append_cstr(&sb, " world") == STR_TRUE);
  CHECK(sb.len == 11);
  CHECK(view_matches(sb_view(&sb), "hello world"));

  /* Empty string: no-op. */
  CHECK(sb_append_cstr(&sb, "") == STR_TRUE);
  CHECK(sb.len == 11);

  sb_free(&sb);
}

/* ==========================================================================
 * sb_append_sv
 * ========================================================================== */
static void test_sb_append_sv(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_sv");

  /* Normal append. */
  CHECK(sb_append_sv(&sb, sv_from_cstr("hello")) == STR_TRUE);
  CHECK(sb.len == 5);

  /* Empty view: no-op. */
  CHECK(sb_append_sv(&sb, sv_from_parts("x", 0)) == STR_TRUE);
  CHECK(sb.len == 5);

  /* Partial view. */
  CHECK(sb_append_sv(&sb, sv_from_parts(" world extra", 6)) == STR_TRUE);
  CHECK(sb.len == 11);
  CHECK(view_matches(sb_view(&sb), "hello world"));

  /* Self-append: appends current content onto itself.
   * This exercises the aliasing fix: realloc may move the buffer. */
  sb_clear(&sb);
  CHECK(sb_append_cstr(&sb, "ab") == STR_TRUE);
  CHECK(sb_append_sv(&sb, sb_view(&sb)) == STR_TRUE);
  CHECK(sb.len == 4);
  CHECK(view_matches(sb_view(&sb), "abab"));

  /* Self-append from a slice into the builder. */
  sb_clear(&sb);
  CHECK(sb_append_cstr(&sb, "hello") == STR_TRUE);
  {
    StrView slice = sv_slice(sb_view(&sb), 1, 4); /* "ell" */
    CHECK(sb_append_sv(&sb, slice) == STR_TRUE);
  }
  CHECK(sb.len == 8);
  CHECK(view_matches(sb_view(&sb), "helloell"));

  /* Repeated self-append grows correctly. */
  sb_clear(&sb);
  CHECK(sb_append_cstr(&sb, "x") == STR_TRUE);
  {
    int i;
    for (i = 0; i < 10; ++i)
      CHECK(sb_append_sv(&sb, sb_view(&sb)) == STR_TRUE);
    CHECK(sb.len == 1024); /* 2^10 */
  }

  sb_free(&sb);
}

/* ==========================================================================
 * sb_append_uint
 * ========================================================================== */
static void test_sb_append_uint(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_uint");

  CHECK(sb_append_uint(&sb, 0ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "0"));

  sb_clear(&sb);
  CHECK(sb_append_uint(&sb, 1ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "1"));

  sb_clear(&sb);
  CHECK(sb_append_uint(&sb, 12345ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "12345"));

  sb_clear(&sb);
  CHECK(sb_append_uint(&sb, 1000000000ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "1000000000"));

  /* ULLONG_MAX = 18446744073709551615 */
  sb_clear(&sb);
  CHECK(sb_append_uint(&sb, 18446744073709551615ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "18446744073709551615"));

  /* Multiple appends: build a CSV of numbers. */
  sb_clear(&sb);
  CHECK(sb_append_uint(&sb, 1ULL) == STR_TRUE);
  CHECK(sb_append_char(&sb, ',') == STR_TRUE);
  CHECK(sb_append_uint(&sb, 2ULL) == STR_TRUE);
  CHECK(sb_append_char(&sb, ',') == STR_TRUE);
  CHECK(sb_append_uint(&sb, 3ULL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "1,2,3"));

  sb_free(&sb);
}

/* ==========================================================================
 * sb_append_int
 * ========================================================================== */
static void test_sb_append_int(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_int");

  CHECK(sb_append_int(&sb, 0LL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "0"));

  sb_clear(&sb);
  CHECK(sb_append_int(&sb, 42LL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "42"));

  sb_clear(&sb);
  CHECK(sb_append_int(&sb, -1LL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "-1"));

  sb_clear(&sb);
  CHECK(sb_append_int(&sb, -99LL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "-99"));

  /* LLONG_MAX = 9223372036854775807 */
  sb_clear(&sb);
  CHECK(sb_append_int(&sb, 9223372036854775807LL) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "9223372036854775807"));

  /* LLONG_MIN = -9223372036854775808 — exercises the overflow-safe path. */
  sb_clear(&sb);
  CHECK(sb_append_int(&sb, (-9223372036854775807LL - 1LL)) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "-9223372036854775808"));

  sb_free(&sb);
}

/* ==========================================================================
 * sb_append_hex
 * ========================================================================== */
static void test_sb_append_hex(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_append_hex");

  /* Zero. */
  CHECK(sb_append_hex(&sb, 0ULL, STR_FALSE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "0"));

  /* Lowercase. */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 0xDEADBEEFULL, STR_FALSE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "deadbeef"));

  /* Uppercase. */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 0xDEADBEEFULL, STR_TRUE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "DEADBEEF"));

  /* Single nibble. */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 0xFULL, STR_TRUE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "F"));

  /* Two nibbles. */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 0xFFULL, STR_FALSE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "ff"));

  /* Boundary: 0x10. */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 0x10ULL, STR_FALSE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "10"));

  /* ULLONG_MAX = ffffffffffffffff */
  sb_clear(&sb);
  CHECK(sb_append_hex(&sb, 18446744073709551615ULL, STR_FALSE) == STR_TRUE);
  CHECK(view_matches(sb_view(&sb), "ffffffffffffffff"));

  sb_free(&sb);
}

/* ==========================================================================
 * sb_clear
 * ========================================================================== */
static void test_sb_clear(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_clear");

  CHECK(sb_append_cstr(&sb, "hello world") == STR_TRUE);
  CHECK(sb.len == 11);

  sb_clear(&sb);
  CHECK(sb.len == 0);
  CHECK(sb.cap > 0); /* allocation is kept */
  CHECK(sb.data != STR_NULL);

  /* After clear, new appends start from position 0. */
  CHECK(sb_append_cstr(&sb, "new") == STR_TRUE);
  CHECK(sb.len == 3);
  CHECK(view_matches(sb_view(&sb), "new"));

  /* Multiple clears. */
  sb_clear(&sb);
  sb_clear(&sb);
  CHECK(sb.len == 0);

  sb_free(&sb);
}

/* ==========================================================================
 * sb_view
 * ========================================================================== */
static void test_sb_view(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);

  group("sb_view");

  /* Empty builder. */
  {
    StrView v = sb_view(&sb);
    CHECK(v.len == 0);
  }

  CHECK(sb_append_cstr(&sb, "hello") == STR_TRUE);
  {
    StrView v = sb_view(&sb);
    CHECK(v.len == 5 && v.data == sb.data);
  }

  /* View is live: reflects changes. */
  CHECK(sb_append_char(&sb, '!') == STR_TRUE);
  {
    StrView v = sb_view(&sb);
    CHECK(v.len == 6);
  }

  sb_free(&sb);
}

/* ==========================================================================
 * sb_build_cstr
 * ========================================================================== */
static void test_sb_build_cstr(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);
  char *cs;

  group("sb_build_cstr");

  CHECK(sb_append_cstr(&sb, "hello") == STR_TRUE);
  cs = sb_build_cstr(&sb);
  CHECK(cs != STR_NULL);
  CHECK(cs[0] == 'h' && cs[4] == 'o' && cs[5] == '\0');

  /* Builder remains valid and length is unchanged after build_cstr. */
  CHECK(sb.len == 5);
  CHECK(view_matches(sb_view(&sb), "hello"));

  /* Calling again after another append is safe. */
  CHECK(sb_append_cstr(&sb, "!") == STR_TRUE);
  cs = sb_build_cstr(&sb);
  CHECK(cs != STR_NULL && cs[6] == '\0');

  /* Empty builder: builds a string of just '\0'. */
  sb_clear(&sb);
  cs = sb_build_cstr(&sb);
  CHECK(cs != STR_NULL && cs[0] == '\0');

  sb_free(&sb);

  /* OOM: returns STR_NULL. */
  {
    StrAllocator fail = make_fail_allocator();
    StrBuilder fb = sb_init(fail, 0);
    /* fb.data is NULL; sb_build_cstr needs at least 1 byte. */
    cs = sb_build_cstr(&fb);
    CHECK(cs == STR_NULL);
  }
}

/* ==========================================================================
 * Growth and OOM resilience
 * ========================================================================== */
static void test_sb_growth_and_oom(void) {
  group("sb growth / OOM resilience");

  /* Force many growth cycles by appending one char at a time. */
  {
    StrAllocator alloc = make_sys_allocator();
    StrBuilder sb = sb_init(alloc, 0);
    int i;
    for (i = 0; i < 512; ++i)
      CHECK(sb_append_char(&sb, 'a') == STR_TRUE);
    CHECK(sb.len == 512);
    CHECK(sb.cap >= 513);
    sb_free(&sb);
  }

  /* OOM on second allocation: first succeeds, second fails. */
  {
    CountAllocCtx ctx;
    ctx.budget = 1; /* one successful alloc, then fail */

    StrAllocator alloc = make_count_allocator(&ctx);
    StrBuilder sb = sb_init(alloc, 0);

    /* First append triggers allocation (budget: 1 -> 0). */
    CHECK(sb_append_cstr(&sb, "hello") == STR_TRUE);
    CHECK(sb.len == 5);

    /* Subsequent append that forces a realloc now fails. */
    {
      int i;
      str_bool_t result = STR_TRUE;
      /* Keep appending until we fill the current cap and need realloc. */
      for (i = 0; i < 256; ++i) {
        if (sb_append_char(&sb, 'x') == STR_FALSE) {
          result = STR_FALSE;
          break;
        }
      }
      CHECK(result == STR_FALSE); /* must have failed at some point */
    }

    sb_free(&sb);
  }
}

/* ==========================================================================
 * Integration: build a structured string using all three abstractions
 * ========================================================================== */
static void test_integration(void) {
  StrAllocator alloc = make_sys_allocator();
  StrBuilder sb = sb_init(alloc, 0);
  const char *csv = "name,age,score\nalice,30,98\nbob,25,72\ncarol,35,85";
  StrView doc = sv_from_cstr(csv);
  StrView rest, line, field, rest2;
  int row = 0;
  char *out;

  group("integration: CSV parse + rebuild");

  /* Skip header line. */
  sv_split_next(doc, '\n', &rest);
  doc = rest;

  /* Collect all scores into a comma-separated list. */
  while (!sv_is_empty(doc)) {
    int field_n = 0;
    line = sv_split_next(doc, '\n', &rest);
    doc = rest;

    while (!sv_is_empty(line)) {
      field = sv_split_next(line, ',', &rest2);
      line = rest2;
      if (field_n == 2) { /* score column */
        if (row > 0)
          sb_append_char(&sb, ',');
        sb_append_sv(&sb, field);
      }
      field_n++;
    }
    row++;
  }

  out = sb_build_cstr(&sb);
  CHECK(out != STR_NULL);
  CHECK(view_matches(sb_view(&sb), "98,72,85"));

  sb_free(&sb);

  /* Build a hex dump of a short byte sequence using str_ + sb_. */
  {
    const unsigned char bytes[] = {0xDE, 0xAD, 0xBE, 0xEF};
    StrBuilder hex = sb_init(alloc, 0);
    str_size_t i;
    for (i = 0; i < 4; ++i)
      sb_append_hex(&hex, bytes[i], STR_TRUE);
    CHECK(view_matches(sb_view(&hex), "DEADBEEF"));
    sb_free(&hex);
  }

  /* Round-trip: number -> string -> parse back. */
  {
    StrBuilder nb = sb_init(alloc, 0);
    unsigned long long original = 9876543210ULL;
    unsigned long long parsed = 0ULL;

    sb_append_uint(&nb, original);
    CHECK(sv_to_uint(sb_view(&nb), &parsed) == STR_TRUE);
    CHECK(parsed == original);
    sb_free(&nb);
  }
}

#include <codebase/ANSI.h>

/* ==========================================================================
 * main
 * ========================================================================== */
int main(void) {
  printf("str.h test suite\n");

  test_str_len();
  test_str_cmp();
  test_str_ncmp();
  test_str_copy();
  test_str_find_char();
  test_str_rfind_char();
  test_str_find();
  test_str_is_empty();

  test_sv_from_cstr();
  test_sv_from_parts();
  test_sv_slice();
  test_sv_cstr_slice();
  test_sv_cmp();
  test_sv_eq();
  test_sv_eq_cstr();
  test_sv_starts_with();
  test_sv_ends_with();
  test_sv_find_char();
  test_sv_rfind_char();
  test_sv_find();
  test_sv_trim_left();
  test_sv_trim_right();
  test_sv_trim();
  test_sv_split_next();
  test_sv_is_empty();
  test_sv_to_uint();
  test_sv_to_int();

  test_sb_init_free();
  test_sb_append_char();
  test_sb_append_cstr();
  test_sb_append_sv();
  test_sb_append_uint();
  test_sb_append_int();
  test_sb_append_hex();
  test_sb_clear();
  test_sb_view();
  test_sb_build_cstr();
  test_sb_growth_and_oom();

  test_integration();

  printf("\n==================================================\n");
  printf(FG_GREEN "%d passed    " FG_RED "%d failed\n" FG_RESET, g_pass, g_fail);
  printf("==================================================\n");

  return g_fail > 0 ? 1 : 0;
}
