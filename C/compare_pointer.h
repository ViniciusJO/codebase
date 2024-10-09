/*

Copyright (c) 2005-2008, Simon Howard

Permission to use, copy, modify, and/or distribute this software
for any purpose with or without fee is hereby granted, provided
that the above copyright notice and this permission notice appear
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

 */

/**
 * @file compare-pointer.h
 *
 * Comparison functions for generic (void) pointers.
 *
 * To find the difference between two pointers, use @ref pointer_compare.
 *
 * To find if two pointers are equal, use @ref pointer_equal.
 */

#ifndef COMPARE_POINTER_H
#define COMPARE_POINTER_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Compare two pointers.
 *
 * @param location1       The first pointer.
 * @param location2       The second pointer.
 * @return                A negative value if the first pointer is in a lower
 *                        memory address than the second, a positive value if
 *                        the first pointer is in a higher memory address than
 *                        the second, zero if they point to the same location.
 */
int pointer_compare(void *, void *);

#ifdef COMPARE_POINTER_IMPLEMENTATION

int pointer_compare(void *location1, void *location2) {
  if (location1 < location2) {
    return -1;
  } else if (location1 > location2) {
    return 1;
  } else {
    return 0;
  }
}

#endif // COMPARE_POINTER_IMPLEMENTATION

#ifdef __cplusplus
}
#endif

#endif // COMPARE_POINTER_H
