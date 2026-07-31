# test_str.c — Test Suite Summary

Total: **935 checks, 0 failures**

Build and run:

    cc -std=c99 -Wall -Wextra -o test_str test_str.c && ./test_str

---

## str_ — Zero-terminated string functions

### str_len
- Normal string returns correct length
- Empty string returns 0
- NULL input returns 0
- Stops at first embedded null (`"abc\0def"` -> 3)
- 20-character string

### str_cmp
- Equal strings return 0
- Both empty return 0
- Less-than and greater-than pairs
- Prefix relationship: longer > shorter prefix, shorter < longer prefix
- Case sensitivity: uppercase < lowercase in ASCII
- NULL vs NULL, NULL vs string, string vs NULL

### str_ncmp
- Equal strings within n
- Mismatch past n is ignored
- Mismatch within n is detected
- n=0 always returns 0
- n=1 compares only first character
- NULL variants

### str_copy
- Normal copy: correct length returned, null terminator written
- Truncation: dst_cap=4 copies 3 chars + null
- dst_cap=1: writes only the null terminator
- dst_cap=0: returns 0, nothing written
- NULL src: produces empty string
- Exact fit: src length == dst_cap - 1
- Empty src

### str_find_char
- First occurrence returned when character repeats
- Character at position 0
- Character at last position
- Not found returns STR_NULL
- `'\0'` as needle returns pointer to null terminator
- NULL input returns STR_NULL
- Single-character string

### str_rfind_char
- Last occurrence returned when character repeats
- Not found returns STR_NULL
- `'\0'` as needle returns pointer to null terminator
- Single-character string (first == last)
- Unique character at the end

### str_find
- Match at start of haystack
- Match in the middle
- Finds first occurrence only (does not skip to second)
- Not found returns STR_NULL
- Empty needle returns haystack pointer
- Needle longer than haystack returns STR_NULL
- Exact full match
- Match at the very end
- NULL haystack or needle returns STR_NULL
- Partial overlap: "aaab" with needle "aab" matches at index 1

### str_is_empty
- NULL input returns STR_TRUE
- Empty string returns STR_TRUE
- Space character is not empty
- Single digit character is not empty

---

## sv_ — String view functions

### sv_from_cstr
- Normal string: correct data pointer and length
- Empty string: length 0
- NULL input: length 0, data STR_NULL

### sv_from_parts
- Normal slice: data pointer preserved, length set
- Zero length: data pointer preserved
- Slice into the middle of a buffer

### sv_slice
- Normal sub-range `[1, 4)`
- Full range `[0, len)`
- Empty range `[x, x)` produces length 0
- Inverted range `[4, 2)` clamped to empty
- start beyond length: clamped to length
- end beyond length: clamped to length
- Single-character slice at start
- Single-character slice at end

### sv_cmp
- Equal views return 0
- Both empty return 0
- Less-than and greater-than pairs
- Prefix relationship: shorter < longer when prefix matches
- Views from non-null-terminated buffers (length-bounded)

### sv_eq
- Equal views return STR_TRUE
- Both empty return STR_TRUE
- Different content returns STR_FALSE
- Same prefix but different lengths returns STR_FALSE
- Non-null-terminated buffers compared by length

### sv_eq_cstr
- Matching view and C string return STR_TRUE
- Both empty return STR_TRUE
- Mismatch returns STR_FALSE
- Length mismatch returns STR_FALSE
- Length-bounded view does not see characters past its length

### sv_starts_with
- Matching prefix
- Exact match (prefix == sv)
- Empty prefix always returns STR_TRUE
- Non-matching prefix
- Prefix longer than sv returns STR_FALSE
- Single-character prefix, matching and non-matching

### sv_ends_with
- Matching suffix
- Exact match (suffix == sv)
- Empty suffix always returns STR_TRUE
- Non-matching suffix
- Suffix longer than sv returns STR_FALSE
- Single-character suffix, matching and non-matching

### sv_find_char
- Returns index of first occurrence
- Not found returns STR_NPOS
- Empty view always returns STR_NPOS
- Length-bounded view does not search past its end

### sv_rfind_char
- Returns index of last occurrence
- Not found returns STR_NPOS
- Empty view always returns STR_NPOS
- Single-character view

### sv_find
- Match at start, middle, end
- Not found returns STR_NPOS
- Empty needle always returns 0
- Empty haystack with empty needle returns 0
- Needle longer than haystack returns STR_NPOS
- Needle equals haystack exactly
- Partial overlap: "aaab" finds "aab" at index 1
- Single-character needle

### sv_trim_left
- Strips leading spaces
- Strips leading tabs
- Strips leading CR and LF
- Strips leading FF and VT
- No leading whitespace: view unchanged
- Trailing whitespace is preserved
- All-whitespace view becomes empty
- Empty view stays empty

### sv_trim_right
- Strips trailing spaces
- Strips trailing tabs
- Strips trailing CR and LF
- No trailing whitespace: view unchanged
- Leading whitespace is preserved
- All-whitespace view becomes empty
- Empty view stays empty

### sv_trim
- Strips both sides
- Only left side needed
- Only right side needed
- All-whitespace view becomes empty
- Already-trimmed view is unchanged
- Empty view stays empty
- Internal whitespace is preserved

### sv_split_next
- Three-field split produces correct token lengths
- No delimiter: returns whole view, rest is empty
- Leading delimiter: first token is empty
- Trailing delimiter: last token is empty, rest is empty
- Consecutive delimiters produce empty tokens between them
- Single character equal to delimiter
- NULL rest pointer is safe (no write)
- Iterative pattern counts correct number of tokens (4 fields)
- Space as delimiter

### sv_is_empty
- Empty string view returns STR_TRUE
- Zero-length view from non-empty data returns STR_TRUE
- Non-empty view returns STR_FALSE
- Space character is not empty

### sv_to_uint
- Zero
- Single digit
- Multi-digit number
- ULLONG_MAX (18446744073709551615)
- Leading zeros accepted
- Empty view returns STR_FALSE
- Non-digit characters return STR_FALSE
- Negative sign returns STR_FALSE
- Leading space returns STR_FALSE
- `*out` is not modified on failure

### sv_to_int
- Zero
- Positive number
- Negative number
- LLONG_MAX (9223372036854775807)
- LLONG_MIN (-9223372036854775808)
- Empty view returns STR_FALSE
- Lone minus sign returns STR_FALSE
- Mixed digit/non-digit returns STR_FALSE
- Leading space returns STR_FALSE
- `*out` is not modified on failure

---

## sb_ — String builder functions

### sb_init / sb_free
- Zero initial capacity: defers allocation (data is NULL)
- Non-zero initial capacity: allocates immediately, cap >= requested
- sb_free zeroes all fields
- sb_free on an already-freed (zeroed) builder is safe
- OOM on init returns zeroed builder

### sb_append_char
- Single character appended correctly
- Multiple characters update length
- 100-character append forces multiple growth cycles
- OOM returns STR_FALSE, length unchanged

### sb_append_cstr
- Normal string appended, length updated
- Second append accumulates correctly
- Empty string is a no-op, length unchanged

### sb_append_sv
- Normal view appended
- Empty view is a no-op
- Partial view (length-bounded) appended correctly
- **Self-append (full view)**: appending sb_view(&sb) onto itself doubles content
  correctly even when realloc moves the buffer
- **Self-append (slice)**: appending a slice into the builder works correctly
- Repeated self-append (10x doubling): reaches 1024 characters

### sb_append_uint
- Zero
- One
- Multi-digit number
- 1,000,000,000
- ULLONG_MAX (18446744073709551615)
- Multiple appends build a comma-separated number list

### sb_append_int
- Zero
- Positive number
- -1
- Negative multi-digit
- LLONG_MAX (9223372036854775807)
- LLONG_MIN (-9223372036854775808): exercises the overflow-safe unsigned cast path

### sb_append_hex
- Zero produces "0"
- Lowercase output
- Uppercase output
- Single nibble (0xF)
- Two nibbles (0xFF)
- Boundary value 0x10
- ULLONG_MAX produces "ffffffffffffffff" (16 digits)

### sb_clear
- Resets length to 0
- Allocation is kept (cap unchanged, data non-NULL)
- Appends after clear start from position 0
- Multiple consecutive clears are safe

### sb_view
- Empty builder returns length-0 view
- After append, view data equals sb.data and length matches
- View reflects subsequent appends (live reference)

### sb_build_cstr
- Null-terminates the buffer in place
- Returns the internal data pointer
- Builder length is unchanged after the call
- View remains valid after the call
- Further appends followed by another build_cstr work correctly
- Empty builder produces a single-byte null-terminated string
- OOM returns STR_NULL

### Growth and OOM resilience
- 512 single-character appends: final length 512, cap >= 513
- Budget allocator (1 alloc allowed): first append succeeds, fill-to-realloc eventually returns STR_FALSE

---

## Integration tests

### CSV parse + score extraction
Parses a multi-line CSV string using `sv_split_next`, extracts the third
column (score) from each data row, and rebuilds the scores as a
comma-separated string using `sb_append_sv`. Verifies final output is
`"98,72,85"`.

### Hex dump
Encodes a four-byte array `{0xDE, 0xAD, 0xBE, 0xEF}` using `sb_append_hex`
per byte and verifies the result is `"DEADBEEF"`.

### Number round-trip
Converts `9876543210` to a string with `sb_append_uint`, parses it back with
`sv_to_uint`, and verifies the recovered value equals the original.
